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

		private:

			static Handle<Value> New(const Arguments& args);

			/* Methods */
			static Handle<Value> setEngine(const Arguments& args);
			static Handle<Value> loadUrl(const Arguments& args);
			static Handle<Value> setData(const Arguments& args);
			static Handle<Value> on(const Arguments& args);
			static Handle<Value> progress(const Arguments& args);
			static Handle<Value> status(const Arguments& args);

			QmlEngineWrap *engine;
			QQmlComponent *obj;
			SignalHandler *signal;
	};

}

#endif
