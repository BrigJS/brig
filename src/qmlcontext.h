#ifndef BRIG_QMLCONTEXT_WRAP_H
#define BRIG_QMLCONTEXT_WRAP_H

#include <node.h>
#include <QQmlContext>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QmlContextWrap : public ObjectWrap {

		public:
			QmlContextWrap(QmlContextWrap *);
			QmlContextWrap(QQmlContext *context);
			QmlContextWrap(Handle<Value> qobject);
			~QmlContextWrap();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);
			static Handle<Value> NewInstance(QQmlContext *);

			QQmlContext *GetObject() const { return obj; };

		private:

			static Handle<Value> New(const Arguments& args);
			static Handle<Value> toObject(const Arguments& args);
			static Handle<Value> contextProperty(const Arguments& args);
			static Handle<Value> setContextProperty(const Arguments& args);

			Handle<Value> prototype_object;
			QQmlContext *obj;
	};

}

#endif
