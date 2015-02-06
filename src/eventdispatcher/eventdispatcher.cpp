#include <QCoreApplication>
#include <QSocketNotifier>

#ifdef __MACOSX_CORE__
#include "platform/mac.h"
#endif

#include <internal/qwindowsysteminterface.h>
#include "eventdispatcher.h"

extern uint qGlobalPostedEventsCount();

namespace Brig {

	static void wakeup_handle(uv_async_t *wakeup, int status)
	{
		BrigEventDispatcher *dispatcher = static_cast<BrigEventDispatcher *>(wakeup->data);

		if (dispatcher->hasPendingEvents())
			dispatcher->processEvents(NULL);
	}

#ifdef __MACOSX_CORE__
void idle(uv_idle_t *idle, int status)
{
	wakeUpApplication();
}
#endif

BrigEventDispatcher::BrigEventDispatcher(QObject *parent) : QAbstractEventDispatcher(parent)
	{
		// Create a new mainloop
		//mainloop = uv_loop_new();
		mainloop = uv_default_loop();

		// Initializing handle
		wakeup = new uv_async_t;
		wakeup->data = (void *)this;
		uv_async_init(mainloop, wakeup, wakeup_handle);
		//printf("Initializing event dispatcher\n");

#ifdef __MACOSX_CORE__
		uv_idle_t *idler = new uv_idle_t;
		idler->data = (void *)this;
		uv_idle_init(mainloop, idler);
		uv_idle_start(idler, idle);

		initApplication();
#endif
	}

	BrigEventDispatcher::~BrigEventDispatcher(void)
	{
		//printf("RELEASE\n");
		uv_close((uv_handle_t *)&wakeup, NULL);
	}

	void BrigEventDispatcher::wakeUp(void)
	{
		//printf("wakeUp\n");
		uv_async_send(wakeup);
	}

	void BrigEventDispatcher::interrupt(void)
	{
		wakeUp();
		//printf("interrupt\n");
	}

	void BrigEventDispatcher::flush(void)
	{
//		printf("flush\n");
	}

	bool BrigEventDispatcher::processEvents(QEventLoop::ProcessEventsFlags flags)
	{
//printf("__ProcessEvents %d\n", qGlobalPostedEventsCount());
		emit awake();

#ifdef __MACOSX_CORE__
		wakeUpApplication();
#endif
		QCoreApplication::sendPostedEvents();
		QWindowSystemInterface::sendWindowSystemEvents(flags);

		emit aboutToBlock();
//printf("ProcessEvents %d\n", qGlobalPostedEventsCount());

		return (qGlobalPostedEventsCount()) ? true : false;
	}

	bool BrigEventDispatcher::hasPendingEvents(void)
	{
		return qGlobalPostedEventsCount();
	}

	void socket_watcher_handle(uv_poll_t *req, int status, int events)
	{
//printf("socket_watcher_handle\n");
		BrigWatcher *_watcher = static_cast<BrigWatcher *>(req->data);

		if (events & UV_READABLE) {
			_watcher->callback();
		}

		if (events & UV_WRITABLE) {
			_watcher->callback();
		}
	}

	void socket_watcher_close(uv_handle_t *handle)
	{
//printf("socket_watcher_close\n");
		uv_poll_t *watcher = (uv_poll_t *)handle;

		delete static_cast<BrigWatcher *>(watcher->data);
		delete watcher;
	}

	void BrigEventDispatcher::registerSocketNotifier(QSocketNotifier *notifier)
	{
//printf("registerSocketNotifier\n");
		int type = -1;
		if (notifier->type() == QSocketNotifier::Read) {
			type = UV_READABLE;
		} else if (notifier->type() == QSocketNotifier::Write) {
			type = UV_WRITABLE;
		} else {
			return;
		}

		// Initializing watcher
		uv_poll_t *watcher;
		std::map<int, uv_poll_t *>::iterator it = socket_watchers.find(notifier->socket());
		if (it == socket_watchers.end()) {
			// There is no watcher we found, so we create a new one
			watcher = new uv_poll_t;
			BrigWatcher *_watcher = new BrigWatcher;
			_watcher->callback = [notifier] {
//printf("Socket NOTIFIER CALLBACK\n");
				QEvent event(QEvent::SockAct);
				QCoreApplication::sendEvent(notifier, &event);
			};
			watcher->data = (void *)_watcher;
			
			socket_watchers.insert(std::make_pair(notifier->socket(), watcher));

			// Initializing socket polling
			uv_poll_init_socket(mainloop, watcher, notifier->socket());
		} else {
			watcher = it->second;
		}

		// Setting events
		BrigWatcher *_watcher = static_cast<BrigWatcher *>(watcher->data);
		_watcher->events |= type;

		// Starting socket polling
		uv_poll_start(watcher, type, socket_watcher_handle);
	}

	void BrigEventDispatcher::unregisterSocketNotifier(QSocketNotifier *notifier)
	{
//printf("unregisterSocketNotifier\n");
		int type = -1;
		if (notifier->type() == QSocketNotifier::Read) {
			type = UV_READABLE;
		} else if (notifier->type() == QSocketNotifier::Write) {
			type = UV_WRITABLE;
		} else {
			return;
		}

		std::map<int, uv_poll_t *>::iterator it = socket_watchers.find(notifier->socket());

		// Notifier doesn't exist
		if (it == socket_watchers.end())
			return;

		// Stop polling
		uv_poll_t *watcher = it->second;
		uv_poll_stop(watcher);

		BrigWatcher *_watcher = static_cast<BrigWatcher *>(watcher->data);
		_watcher->events &= ~type;
		if (_watcher->events) {
			// Close socket handle
			uv_close((uv_handle_t *)watcher, socket_watcher_close);

			// Take off from list
			socket_watchers.erase(it);
			return;
		}

		// Keep polling for event left
		uv_poll_start(watcher, _watcher->events, socket_watcher_handle);
	}

	void timer_handle(uv_timer_t *handle, int status)
	{
//printf("timer_handle\n");
		BrigHandle *_timer = static_cast<BrigHandle *>(handle->data);

		_timer->callback();
	}

	void timer_close(uv_handle_t *handle)
	{
//printf("timer_close\n");
		uv_timer_t *timer = (uv_timer_t *)handle;

		delete static_cast<BrigHandle *>(timer->data);
		delete timer;
	}

	void BrigEventDispatcher::registerTimer(int timerId, int interval, Qt::TimerType timerType, QObject *object)
	{
		BrigTimer *timer;
//printf("registerTimer\n");

		std::map<int, BrigTimer *>::iterator it = timers.find(timerId);
		if (it == timers.end()) {
			// There is no timer we found, so we create a new one
			timer = new BrigTimer;
			timer->Id = timerId;
			timer->handle = new uv_timer_t;

			// Create timer handler
			BrigHandle *_timer = new BrigHandle;
			_timer->callback = [timer] {
//				printf("TIMER CALLBACK %d\n", timer->Id);
				timer->timestamp = uv_hrtime() / 1000000;
				QTimerEvent event(timer->Id);
				QCoreApplication::sendEvent(timer->object, &event);
//				printf("TIMER CALLBACK FINISHED %d\n", timer->Id);
			};
			timer->handle->data = (void *)_timer;
			
			// Register
			timers.insert(std::make_pair(timerId, timer));
			object_timers.insert(std::make_pair(object, timer));

			// Initializing timer
			uv_timer_init(mainloop, timer->handle);
		} else {
			timer = it->second;
		}

		// Update timer
		timer->info.append(QAbstractEventDispatcher::TimerInfo(timerId, interval, timerType));
		timer->timestamp = uv_hrtime() / 1000000;
		timer->interval = interval;
		timer->object = object;

		uv_timer_start(timer->handle, timer_handle, interval, interval);
	}

	bool BrigEventDispatcher::unregisterTimer(int timerId)
	{
//printf("unregisterTimer %d\n", timerId);

		std::map<int, BrigTimer *>::iterator it = timers.find(timerId);
		if (it == timers.end())
			return false;

		// Stop timer
		BrigTimer *timer = it->second;
		uv_timer_stop(timer->handle);
		uv_close((uv_handle_t *)timer->handle, timer_close);

		// Clear timer info
		timer->info.empty();

		// Remove
		timers.erase(it);

		// Find timer by object
		std::map<QObject *, BrigTimer *>::iterator oit = object_timers.find(timer->object);
		if (oit == object_timers.end())
			return true;

		// Remove
		object_timers.erase(oit);

		return true;
	}

	bool BrigEventDispatcher::unregisterTimers(QObject* object)
	{
		//printf("unregisterTimers\n");

		bool ret = true;
		for (auto info : registeredTimers(object)) {
			ret &= unregisterTimer(info.timerId);
		}
		return ret;
	}

	QList<QAbstractEventDispatcher::TimerInfo> BrigEventDispatcher::registeredTimers(QObject* object) const
	{
		//printf("registeredTimers\n");

		auto it = object_timers.find(object);
		if (it == object_timers.end())
			return QList<QAbstractEventDispatcher::TimerInfo>();

		BrigTimer *timer = it->second;

		return timer->info;
	}

	int BrigEventDispatcher::remainingTime(int timerId)
	{
		//printf("remainingTime\n");

		BrigTimer *timer = timers[timerId];
		return timer->interval + timer->timestamp - (uv_hrtime() / 1000000);
	}

	void BrigEventDispatcher::Run()
	{
		running = true;
	}

}
