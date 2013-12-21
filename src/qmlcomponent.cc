#include <node.h>
#include <QtGui>
#include <QObject>
#include "qmlcomponent.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QmlComponentWrap::constructor;

	QmlComponentWrap::QmlComponentWrap(QmlEngineWrap *engine_wrap) : ObjectWrap()
	{
		obj = new QQmlComponent(engine_wrap->GetObject());
	}

	QmlComponentWrap::~QmlComponentWrap()
	{
		delete obj;
	}

	void QmlComponentWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QmlComponent");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QmlComponentWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "create", QmlComponentWrap::create);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QmlComponentWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QmlEngineWrap *engine_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(args[0]->ToObject());
		QmlComponentWrap *obj_wrap = new QmlComponentWrap(engine_wrap);
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QmlComponentWrap::create(const Arguments& args)
	{
		HandleScope scope;

		QmlComponentWrap *obj_wrap = ObjectWrap::Unwrap<QmlComponentWrap>(args.This());
		QmlContextWrap *context_wrap = ObjectWrap::Unwrap<QmlContextWrap>(args[0]->ToObject());

		QQmlContext *context = context_wrap->GetObject();
		QObject *obj = obj_wrap->GetObject()->create(context);

		// TODO: wrap and return object

		return scope.Close(Undefined());
	}
}
