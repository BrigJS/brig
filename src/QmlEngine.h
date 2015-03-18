#ifndef BRIG_QMLENGINE_WRAP_H
#define BRIG_QMLENGINE_WRAP_H

#include <node.h>
#include <QQmlEngine>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QmlEngineWrap : public ObjectWrap {

		public:
			QmlEngineWrap();
			~QmlEngineWrap();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);

			QQmlEngine *GetObject() const { return obj; };

		private:

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(on);
			//static NAN_METHOD(rootContext);

			QQmlEngine *obj;
			SignalHandler *signal;
	};

}

#endif
