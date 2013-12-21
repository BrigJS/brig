#include <node.h>
#include <QtGui>
#include <QObject>
#include "qmlcontext.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QmlContextWrap::constructor;

	QmlContextWrap::QmlContextWrap(QmlContextWrap *context_wrap) : ObjectWrap()
	{
		obj = new QQmlContext(context_wrap->GetObject());
	}

	QmlContextWrap::QmlContextWrap(QQmlContext *context) : ObjectWrap()
	{
		obj = new QQmlContext(context);
	}

	QmlContextWrap::~QmlContextWrap()
	{
		delete obj;
	}

	void QmlContextWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QmlContext");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QmlContextWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QmlContextWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QmlContextWrap *context_wrap = ObjectWrap::Unwrap<QmlContextWrap>(args[0]->ToObject());
		QmlContextWrap *obj_wrap = new QmlContextWrap(context_wrap);
		obj_wrap->Wrap(args.This());

		return args.This();
	}
}
