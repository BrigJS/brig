#include <node.h>
#include <QtGui>
#include <QObject>
#include "qmlengine.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QmlEngineWrap::constructor;

	QmlEngineWrap::QmlEngineWrap() : ObjectWrap()
	{
/*
		Isolate *isolate = Isolate::New();
		Persistent<Context> context;
*/
		{
/*
			Locker locker(isolate);
			Isolate::Scope isolate_scope(isolate);
			isolate->Enter();

			HandleScope handle_scope;

			context = Context::New();
			Context::Scope context_scope(context);
			context->Enter();
*/
			obj = new QQmlEngine();
		}
	}

	QmlEngineWrap::~QmlEngineWrap()
	{
		delete obj;
	}

	void QmlEngineWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QmlEngine");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QmlEngineWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "rootContext", QmlEngineWrap::rootContext);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QmlEngineWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QmlEngineWrap *obj_wrap = new QmlEngineWrap();
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QmlEngineWrap::rootContext(const Arguments& args)
	{
		HandleScope scope;

		QmlEngineWrap *obj_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(args.This());

		QQmlContext *context = obj_wrap->GetObject()->rootContext();
		QmlContextWrap *context_wrap = new QmlContextWrap(context);

		return scope.Close(Undefined());
	}
}
