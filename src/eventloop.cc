#include <node.h>
#include <QtGui>
#include <QObject>
#include "eventloop.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	static void MainLoopHandle(uv_async_t *mainloop, int status);
	static void CheckHandle(uv_check_t *handle, int status);
	static void PrepareHandle(uv_prepare_t *handle, int status);

	EventLoop::EventLoop(QGuiApplication *_app) : QObject()
	{
		app = _app;

		eventDispatcher = QAbstractEventDispatcher::instance();

		// Initializing main loop
		mainloop = new uv_async_t;
		mainloop->data = (void *)this;
		uv_async_init(uv_default_loop(), mainloop, MainLoopHandle);
	}

	EventLoop::~EventLoop()
	{
	}

	void EventLoop::Main()
	{
//		uv_async_send(mainloop);
/*
		uv_check_t *check = new uv_check_t;
		check->data = (void *)this;
		uv_check_init(uv_default_loop(), check);
		uv_check_start(check, CheckHandle);
*/
		uv_prepare_t *prepare = new uv_prepare_t;
		prepare->data = (void *)this;
		uv_prepare_init(uv_default_loop(), prepare);
		uv_prepare_start(prepare, PrepareHandle);
	}

	void CheckHandle(uv_check_t *handle, int status)
	{
		EventLoop *eventloop = static_cast<EventLoop *>(handle->data);
		QGuiApplication *app = eventloop->GetApp();

		printf("123123123\n");

		app->processEvents();
	}

	void PrepareHandle(uv_prepare_t *handle, int status)
	{
		EventLoop *eventloop = static_cast<EventLoop *>(handle->data);
		QGuiApplication *app = eventloop->GetApp();

//		printf("PREPARE\n");

		//app->processEvents();
		if (app->hasPendingEvents())
			uv_async_send(eventloop->GetMainLoop());
	}

	static void MainLoopHandle(uv_async_t *mainloop, int status)
	{
		EventLoop *eventloop = static_cast<EventLoop *>(mainloop->data);
		QGuiApplication *app = eventloop->GetApp();

//		if (app->hasPendingEvents())
			app->processEvents();

//		uv_async_send(mainloop);
	}
}
