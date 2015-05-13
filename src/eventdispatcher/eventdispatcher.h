#ifndef BRIG_EVENTDISPATCHER_H
#define BRIG_EVENTDISPATCHER_H

#include <map>
#include <functional>
#include <QAbstractEventDispatcher>
#include <uv.h>

namespace Brig {

struct BrigWatcher {
	int events;
	std::function<void(void)> callback;
};

struct BrigHandle {
	std::function<void(void)> callback;
};

struct BrigTimer {
	int Id;
	uv_timer_t *handle;
	QList<QAbstractEventDispatcher::TimerInfo> info;
	unsigned long timestamp;
	int interval;
	QObject *object;
};

class BrigEventDispatcher: public QAbstractEventDispatcher {
    Q_OBJECT
public:
    explicit BrigEventDispatcher(QObject* parent = 0);
    virtual ~BrigEventDispatcher(void);

    virtual void wakeUp(void);
    virtual void interrupt(void);
    virtual void flush(void);

	// Process events
    virtual bool hasPendingEvents(void);
    virtual bool processEvents(QEventLoop::ProcessEventsFlags flags);

	// Socket notifier
    virtual void registerSocketNotifier(QSocketNotifier* notifier);
    virtual void unregisterSocketNotifier(QSocketNotifier* notifier);

	// Timer
    virtual void registerTimer(int timerId, int interval, Qt::TimerType timerType, QObject* object);
    virtual bool unregisterTimer(int timerId);

	// Timers
    virtual bool unregisterTimers(QObject* object);
    virtual QList<QAbstractEventDispatcher::TimerInfo> registeredTimers(QObject* object) const;
    virtual int remainingTime(int timerId);

	virtual void Run();
private:

	bool running = false;
	uv_loop_t *mainloop;
	uv_async_t *wakeup;
	uv_timer_t *keepalive;

	std::map<int, uv_poll_t *> socket_watchers;
	std::map<int, BrigTimer *> timers;
	std::map<QObject *, BrigTimer *> object_timers;

    Q_DISABLE_COPY(BrigEventDispatcher)
};

}

#endif
