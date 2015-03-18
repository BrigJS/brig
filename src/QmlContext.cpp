#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "QmlContext.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QmlContext::constructor;

	QmlContext::QmlContext() : ObjectWrap()
	{
		obj = NULL;
	}

	QmlContext::~QmlContext()
	{
		delete obj;
	}

	void QmlContext::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QmlContext");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QmlContext::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		//NODE_SET_PROTOTYPE_METHOD(tpl, "setEngine", QmlContext::setEngine);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	// Prototype Constructor

	NAN_METHOD(QmlContext::New) {
		NanScope();

		if (args.Length() == 0)
			return Undefined();

		// Using Engine to initialize QQmlContext
		QmlEngineWrap *engine_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(args[0]->ToObject());

		QmlContext *obj_wrap = new QmlContext();
		obj_wrap->obj = new QQmlContext(engine_wrap->GetObject()->rootContext());
		obj_wrap->Wrap(args.This());

		NanReturnValue(args.This());
	}

	// Method
}
