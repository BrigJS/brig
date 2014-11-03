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
			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);

			QGuiApplication *GetApp() const { return app; };
			EventLoop *GetEventLoop() const { return eventloop; };

		private:
			QApplicationWrap();
			~QApplicationWrap();

			static Handle<Value> New(const Arguments& args);

			/* Methods */
			static Handle<Value> Exec(const Arguments& args);
			static Handle<Value> Test(const Arguments& args);

			QGuiApplication *app;
			int app_argc;
			char **app_argv;
			EventLoop *eventloop;
			BrigEventDispatcher *dispatcher;

			QQuickView *quickview;
	};

}

#endif
