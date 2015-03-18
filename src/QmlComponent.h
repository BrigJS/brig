#ifndef BRIG_QMLCOMPONENT_H
#define BRIG_QMLCOMPONENT_H

#include <node.h>
#include <QQmlComponent>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QmlComponent : public ObjectWrap {

		public:
			QmlComponent();
			~QmlComponent();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);

			QQmlComponent *GetObject() const { return obj; };
			QmlEngineWrap *GetEngine() const { return engine; };

		private:

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(setEngine);
			static NAN_METHOD(loadUrl);
			static NAN_METHOD(setData);
			static NAN_METHOD(on);
			static NAN_METHOD(progress);
			static NAN_METHOD(status);
			static NAN_METHOD(errors);

			QmlEngineWrap *engine;
			QQmlComponent *obj;
			SignalHandler *signal;
	};

}

#endif
