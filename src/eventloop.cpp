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

	EventLoop::EventLoop(QGuiApplication *_app) : QObject()
	{
		app = _app;
	}

	EventLoop::~EventLoop()
	{
	}

	void EventLoop::Main()
	{
		uv_prepare_t *prepare = new uv_prepare_t;
		prepare->data = (void *)this;
		uv_prepare_init(uv_default_loop(), prepare);
		uv_prepare_start(prepare, EventLoop::PrepareHandle);
	}

	void EventLoop::PrepareHandle(uv_prepare_t *handle, int status)
	{
		EventLoop *eventloop = static_cast<EventLoop *>(handle->data);
		QGuiApplication *app = eventloop->GetApp();

//		if (app->hasPendingEvents())
//			app->processEvents();
	}
}
