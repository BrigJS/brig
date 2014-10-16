#ifndef BRIG_QUICKITEM_WRAP_H
#define BRIG_QUICKITEM_WRAP_H

#include <node.h>
#include <QQuickItem>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QuickItemWrap : public ObjectWrap {

		public:
			QuickItemWrap();
			QuickItemWrap(Handle<Value> qobject);
			~QuickItemWrap();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);
			static Handle<Value> NewInstance(QObject *);
			static Handle<Value> NewInstance(Handle<Value>);

			QQuickItem *GetObject() const { return obj; };

		private:

			static Handle<Value> New(const Arguments& args);

			/* Methods */
			static Handle<Value> setParentItem(const Arguments& args);
			static Handle<Value> setVisible(const Arguments& args);

			Handle<Value> prototype_object;
			QQuickItem *obj;
	};

}

#endif
