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
printf("RELEASE Component\n");
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
		NODE_SET_PROTOTYPE_METHOD(tpl, "setData", QmlComponent::setData);
		NODE_SET_PROTOTYPE_METHOD(tpl, "on", QmlComponent::on);
		NODE_SET_PROTOTYPE_METHOD(tpl, "progress", QmlComponent::progress);
		NODE_SET_PROTOTYPE_METHOD(tpl, "status", QmlComponent::status);
		NODE_SET_PROTOTYPE_METHOD(tpl, "errors", QmlComponent::errors);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	// Prototype Constructor
	NAN_METHOD(QmlComponent::New) {
		NanScope();

		QmlComponent *obj_wrap = new QmlComponent();
		obj_wrap->Wrap(args.This());

		NanReturnValue(args.This());
	}

	// Method
	NAN_METHOD(QmlComponent::setEngine) {
		NanScope();

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		QmlEngineWrap *engine_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(args[0]->ToObject());

		obj_wrap->engine = engine_wrap;

		NanReturnValue(args.This());
	}

	NAN_METHOD(QmlComponent::loadUrl) {
		NanScope();

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		String::Utf8Value url(args[0]->ToString());

		// Create component
		obj_wrap->obj = new QQmlComponent(obj_wrap->engine->GetObject());

		// Setup signal handler
		obj_wrap->signal->setObject(obj_wrap->obj);

		// Loading specific file
		obj_wrap->obj->loadUrl(QUrl(QString(*url)), QQmlComponent::Asynchronous);

		NanReturnValue(args.This());
	}

	NAN_METHOD(QmlComponent::setData) {
		NanScope();

		QUrl url;

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		if (args[1]->IsString()) {
			String::Utf8Value url_str(args[1]->ToString());
			url = QUrl::fromLocalFile(*url_str);
		} else {
			url = QUrl();
		}

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		if (!obj_wrap->obj) {
			obj_wrap->obj = new QQmlComponent(obj_wrap->engine->GetObject());
			obj_wrap->signal->setObject(obj_wrap->obj);
		}

		String::Utf8Value data_str(args[0]->ToString());
		obj_wrap->GetObject()->setData(*data_str, url); 

		NanReturnUndefined();
	}


	NAN_METHOD(QmlComponent::on) {
		NanScope();

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		// Signal name
		String::Utf8Value url(args[0]->ToString());

		int id = obj_wrap->signal->addCallback(*url, args[1]);

		NanReturnValue(args.This());
	}

	NAN_METHOD(QmlComponent::progress) {
		NanScope();

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		NanReturnValue(Number::New(obj_wrap->obj->progress()));
	}

	NAN_METHOD(QmlComponent::status) {
		NanScope();

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		NanReturnValue(Number::New(obj_wrap->obj->status()));
	}

	NAN_METHOD(QmlComponent::errors) {
		NanScope();

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(args.This());

		// Getting errors
		QList<QQmlError> errs = obj_wrap->GetObject()->errors();

		// Create an array
		Handle<Array> errArr = Array::New();

		for (int i = 0; i < errs.length(); i++) {
			errArr->Set(i, String::New(errs[i].toString().toUtf8().constData()));
		}

		NanReturnValue(errArr);
	}
}
