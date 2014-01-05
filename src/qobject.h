#ifndef BRIG_QOBJECT_WRAP_H
#define BRIG_QOBJECT_WRAP_H

#include <node.h>
#include <QObject>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QObjectWrap : public ObjectWrap {

		public:
			QObjectWrap();
			QObjectWrap(QObject *object);
			~QObjectWrap();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);
			static Handle<Value> NewInstance(QObject *object);

			QObject *GetObject() const { return obj; };

		private:

			static Handle<Value> New(const Arguments& args);

			/* Methods */
//			static Handle<Value> show(const Arguments& args);

			QObject *obj;
	};

}

#endif
