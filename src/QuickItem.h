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

			QQuickItem *obj;
	};

}

#endif
