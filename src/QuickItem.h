#ifndef BRIG_QUICKITEM_H
#define BRIG_QUICKITEM_H

#include <node.h>
#include <QQuickItem>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QuickItem : public ObjectWrap {

		public:
			QuickItem();
			~QuickItem();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);

			QQuickItem *GetObject() const { return obj; };

		private:

			static Handle<Value> New(const Arguments& args);

			/* Methods */
			static Handle<Value> create(const Arguments& args);
			static Handle<Value> getPropertyNames(const Arguments& args);
			static Handle<Value> getProperty(const Arguments& args);
			static Handle<Value> setProperty(const Arguments& args);
			static Handle<Value> setParent(const Arguments& args);
			static Handle<Value> invokeMethod(const Arguments& args);
			static Handle<Value> emitEvent(const Arguments& args);
			static Handle<Value> on(const Arguments& args);

			QQuickItem *obj;
			SignalHandler *signal;
	};

}

#endif
