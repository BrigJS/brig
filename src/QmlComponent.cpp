#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "QmlComponent.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QmlComponent::constructor;

	QmlComponent::QmlComponent() : ObjectWrap()
	{
		obj = NULL;
		engine = NULL;
	}

	QmlComponent::~QmlComponent()
	{
		delete obj;
	}

	void QmlComponent::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QmlComponent");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QmlComponent::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "setEngine", QmlComponent::setEngine);
		NODE_SET_PROTOTYPE_METHOD(tpl, "loadUrl", QmlComponent::loadUrl);
		NODE_SET_PROTOTYPE_METHOD(tpl, "create", QmlComponent::create);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	// Prototype Constructor
	Handle<Value> QmlComponent::New(const Arguments& args)
	{
		HandleScope scope;

		QmlComponent *obj_wrap = new QmlComponent();
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	// Method
	Handle<Value> QmlComponent::setEngine(const Arguments& args)
	{
		HandleScope scope;

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		QmlEngineWrap *engine_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(args[0]->ToObject());

		obj_wrap->engine = engine_wrap;

		return args.This();
	}

	Handle<Value> QmlComponent::loadUrl(const Arguments& args)
	{
		HandleScope scope;

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		String::Utf8Value url(args[0]->ToString());

		obj_wrap->obj = new QQmlComponent(obj_wrap->engine->GetObject(), *url);

		return args.This();
	}

	Handle<Value> QmlComponent::create(const Arguments& args)
	{
		HandleScope scope;

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		obj_wrap->GetObject()->create();

	}
}
