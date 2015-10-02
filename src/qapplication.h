#ifndef BRIG_QGUIAPPLICATION_WRAP_H
#define BRIG_QGUIAPPLICATION_WRAP_H

#include <node.h>
#include <QGuiApplication>
#include <QQuickView>
#include "eventdispatcher/eventdispatcher.h"
#include "eventloop.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QApplicationWrap : public ObjectWrap {

		public:
			static NAN_MODULE_INIT(Initialize);

			QGuiApplication *GetApp() const { return app; };
			EventLoop *GetEventLoop() const { return eventloop; };

		private:
			QApplicationWrap();
			~QApplicationWrap();

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(Exec);
			static NAN_METHOD(Test);

			QGuiApplication *app;
			int app_argc;
			char **app_argv;
			EventLoop *eventloop;
			BrigEventDispatcher *dispatcher;

			QQuickView *quickview;

			static Nan::Persistent<Function> constructor;
	};

}

#endif
