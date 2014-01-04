#include <node.h>
#include <QtGui>
#include <QObject>
#include <QCoreApplication>

#if (QT_VERSION == QT_VERSION_CHECK(5, 1, 1))
#include <5.1.1/QtGui/qpa/qwindowsysteminterface.h>
#elif (QT_VERSION == QT_VERSION_CHECK(5, 2, 0))
#include <5.2.0/QtGui/qpa/qwindowsysteminterface.h>
#endif
#include "eventloop.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	static void MainLoopHandle(uv_async_t *mainloop, int status);
	static void CheckHandle(uv_check_t *handle, int status);
	static void PrepareHandle(uv_prepare_t *handle, int status);
	static void WorkHandle(uv_work_t *work);

	EventLoop::EventLoop(QGuiApplication *_app) : QObject()
	{
		app = _app;

		//eventDispatcher = QAbstractEventDispatcher::instance();

		// Initializing main loop
#if 0
		mainloop = new uv_async_t;
		mainloop->data = (void *)this;
		uv_async_init(uv_default_loop(), mainloop, MainLoopHandle);
#endif
	}

	EventLoop::~EventLoop()
	{
	}

	void EventLoop::Main()
	{
//		uv_async_send(mainloop);
#if 0
		uv_check_t *check = new uv_check_t;
		check->data = (void *)this;
		uv_check_init(uv_default_loop(), check);
		uv_check_start(check, CheckHandle);
#endif
#if 1
		uv_prepare_t *prepare = new uv_prepare_t;
		prepare->data = (void *)this;
		uv_prepare_init(uv_default_loop(), prepare);
		uv_prepare_start(prepare, PrepareHandle);
#endif
	}

	void CheckHandle(uv_check_t *handle, int status)
	{
		EventLoop *eventloop = static_cast<EventLoop *>(handle->data);
		QGuiApplication *app = eventloop->GetApp();

//		printf("123123123\n");
		printf("CHECK\n");

//		app->processEvents();
//
/*
		if (app->hasPendingEvents())
			app->processEvents();
*/
	}

	void PrepareHandle(uv_prepare_t *handle, int status)
	{
		EventLoop *eventloop = static_cast<EventLoop *>(handle->data);
		QGuiApplication *app = eventloop->GetApp();

		printf("PREPARE %d\n", status);

#if 1
//		if (app->hasPendingEvents())
			app->processEvents();
//			QCoreApplication::sendPostedEvents();
//			QWindowSystemInterface::sendWindowSystemEvents(NULL);
//			uv_async_send(eventloop->GetMainLoop());
#endif
	}

	static void MainLoopHandle(uv_async_t *mainloop, int status)
	{
		EventLoop *eventloop = static_cast<EventLoop *>(mainloop->data);
		QGuiApplication *app = eventloop->GetApp();

//		printf("123123123\n");
//		if (app->hasPendingEvents())
//			app->processEvents();

//		uv_async_send(mainloop);
	}
}
