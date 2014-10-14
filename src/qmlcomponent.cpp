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
		prototype_object = QObjectWrap::NewInstance(obj);
	}

	QmlComponentWrap::QmlComponentWrap(QmlEngineWrap *engine_wrap, Local<Value> filename) : ObjectWrap()
	{
		String::Utf8Value filename_str(filename->ToString());
		obj = new QQmlComponent(engine_wrap->GetObject(), *filename_str);
		prototype_object = QObjectWrap::NewInstance(obj);
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
		NODE_SET_PROTOTYPE_METHOD(tpl, "setData", QmlComponentWrap::setData);
		NODE_SET_PROTOTYPE_METHOD(tpl, "isReady", QmlComponentWrap::isReady);
		NODE_SET_PROTOTYPE_METHOD(tpl, "toObject", QmlComponentWrap::toObject);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QmlComponentWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QmlEngineWrap *engine_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(args[0]->ToObject());
		QmlComponentWrap *obj_wrap;
		if (args.Length() == 1) {
			obj_wrap = new QmlComponentWrap(engine_wrap);
		} else {
			obj_wrap = new QmlComponentWrap(engine_wrap, args[1]);
		}
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QmlComponentWrap::create(const Arguments& args)
	{
		HandleScope scope;

		QObject *obj;
		QmlComponentWrap *obj_wrap = ObjectWrap::Unwrap<QmlComponentWrap>(args.This());

		if (args[0]->IsObject()) {
			QmlContextWrap *context_wrap = ObjectWrap::Unwrap<QmlContextWrap>(args[0]->ToObject());
			QQmlContext *context = context_wrap->GetObject();
			obj = obj_wrap->GetObject()->create(context);
		} else {
			obj = obj_wrap->GetObject()->create();
		}

		return scope.Close(QObjectWrap::NewInstance(obj));
	}

	Handle<Value> QmlComponentWrap::setData(const Arguments& args)
	{
		HandleScope scope;

		QUrl url;

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		if (args[1]->IsString()) {
			String::Utf8Value url_str(args[1]->ToString());
			url = QUrl::fromLocalFile(*url_str);
		} else {
			url = QUrl();
		}

		String::Utf8Value data_str(args[0]->ToString());

		QmlComponentWrap *obj_wrap = ObjectWrap::Unwrap<QmlComponentWrap>(args.This());
		obj_wrap->GetObject()->setData(*data_str, url); 

		return scope.Close(Undefined());
	}

	Handle<Value> QmlComponentWrap::isReady(const Arguments& args)
	{
		HandleScope scope;

		QmlComponentWrap *obj_wrap = ObjectWrap::Unwrap<QmlComponentWrap>(args.This());

		return scope.Close(Boolean::New(obj_wrap->GetObject()->isReady()));
	}

	Handle<Value> QmlComponentWrap::toObject(const Arguments& args)
	{
		HandleScope scope;

		QmlComponentWrap *obj_wrap = ObjectWrap::Unwrap<QmlComponentWrap>(args.This());

		return scope.Close(obj_wrap->GetPrototypeObject());
	}
}
