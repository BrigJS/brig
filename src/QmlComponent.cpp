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
		signal = new SignalHandler();
	}

	QmlComponent::~QmlComponent()
	{
		delete signal;
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
		NODE_SET_PROTOTYPE_METHOD(tpl, "on", QmlComponent::on);

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

		obj_wrap->obj = new QQmlComponent(obj_wrap->engine->GetObject());
		obj_wrap->obj->loadUrl(QUrl(*url), QQmlComponent::Asynchronous);
		obj_wrap->signal->setObject(obj_wrap->obj);

		if (obj_wrap->obj->isLoading())
			printf("LOADING\n");
//		obj_wrap->signal->connect();

//		QObject::connect(obj_wrap->obj, SIGNAL(statusChanged(QQmlComponent::Status)), obj_wrap, SLOT(QmlComponent::continueLoading()));
#if 0
		if (obj_wrap->obj->isLoading())
			QObject::connect(obj_wrap->obj, SIGNAL(statusChanged(QQmlComponent::Status)), obj_wrap, SLOT(QmlComponent::continueLoading()));
		else
			QmlComponent::continueLoading();
#endif
		return args.This();
	}

	Handle<Value> QmlComponent::on(const Arguments& args)
	{
		HandleScope scope;

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		// Signal name
		String::Utf8Value url(args[0]->ToString());

		int id = obj_wrap->signal->addCallback(*url, args[1]->ToObject());
printf("COOL %d\n", id);

		return args.This();
	}
}
