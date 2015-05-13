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

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(create);
			static NAN_METHOD(getPropertyNames);
			static NAN_METHOD(getProperty);
			static NAN_METHOD(setProperty);
			static NAN_METHOD(setParent);
			static NAN_METHOD(invokeMethod);
			static NAN_METHOD(emitEvent);
			static NAN_METHOD(on);

			QQuickItem *obj;
			SignalHandler *signal;
	};

}

#endif
