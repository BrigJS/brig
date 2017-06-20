#ifndef BRIG_QGUIAPPLICATION_WRAP_H
#define BRIG_QGUIAPPLICATION_WRAP_H

#include <node.h>
#include <QGuiApplication>
#include <QQuickView>
#include "eventdispatcher/eventdispatcher.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QApplicationWrap : public ObjectWrap {

		public:
			static NAN_MODULE_INIT(Initialize);

			QGuiApplication *GetApp() const { return app; };

		private:
			QApplicationWrap();
			~QApplicationWrap();

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(setStyle);
			static NAN_METHOD(setApplicationName);

			QGuiApplication *app;
			int app_argc;
			char **app_argv;
			BrigEventDispatcher *dispatcher;

			static Nan::Persistent<Function> constructor;
	};

}

#endif
