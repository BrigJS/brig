#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "QmlComponent.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Nan::Persistent<Function> QmlComponent::constructor;

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

	NAN_MODULE_INIT(QmlComponent::Initialize) {

		Local<String> name = Nan::New("QmlComponent").ToLocalChecked();

		/* Constructor template */
		Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(QmlComponent::New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		Nan::SetPrototypeMethod(tpl, "setEngine", QmlComponent::setEngine);
		Nan::SetPrototypeMethod(tpl, "loadUrl", QmlComponent::loadUrl);
		Nan::SetPrototypeMethod(tpl, "setData", QmlComponent::setData);
		Nan::SetPrototypeMethod(tpl, "on", QmlComponent::on);
		Nan::SetPrototypeMethod(tpl, "progress", QmlComponent::progress);
		Nan::SetPrototypeMethod(tpl, "status", QmlComponent::status);
		Nan::SetPrototypeMethod(tpl, "errors", QmlComponent::errors);

		constructor.Reset(tpl->GetFunction());

		target->Set(name, Nan::New(constructor));
	}

	// Prototype Constructor
	NAN_METHOD(QmlComponent::New) {

		QmlComponent *obj_wrap = new QmlComponent();
		obj_wrap->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	// Method
	NAN_METHOD(QmlComponent::setEngine) {

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(info.This());

		QmlEngineWrap *engine_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(info[0]->ToObject());

		obj_wrap->engine = engine_wrap;

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QmlComponent::loadUrl) {

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(info.This());

		String::Utf8Value url(info[0]->ToString());

		// Create component
		obj_wrap->obj = new QQmlComponent(obj_wrap->engine->GetObject());

		// Setup signal handler
		obj_wrap->signal->setObject(obj_wrap->obj);

		// Loading specific file
		obj_wrap->obj->loadUrl(QUrl(QString(*url)), QQmlComponent::Asynchronous);

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QmlComponent::setData) {

		QUrl url;

		if (!info[0]->IsString())
			Nan::ThrowTypeError("First argument must be a string");

		if (info[1]->IsString()) {
			String::Utf8Value url_str(info[1]->ToString());
			url = QUrl::fromLocalFile(*url_str);
		} else {
			url = QUrl();
		}

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(info.This());

		if (!obj_wrap->obj) {
			obj_wrap->obj = new QQmlComponent(obj_wrap->engine->GetObject());
			obj_wrap->signal->setObject(obj_wrap->obj);
		}

		String::Utf8Value data_str(info[0]->ToString());
		obj_wrap->GetObject()->setData(*data_str, url); 

		info.GetReturnValue().SetUndefined();
	}


	NAN_METHOD(QmlComponent::on) {

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(info.This());

		// Signal name
		String::Utf8Value url(info[0]->ToString());

		int id = obj_wrap->signal->addCallback(*url, info[1]);

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QmlComponent::progress) {

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(info.This());

		info.GetReturnValue().Set(Nan::New<Number>(obj_wrap->obj->progress()));
	}

	NAN_METHOD(QmlComponent::status) {

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(info.This());

		info.GetReturnValue().Set(Nan::New<Number>(obj_wrap->obj->status()));
	}

	NAN_METHOD(QmlComponent::errors) {

		QmlComponent *obj_wrap = ObjectWrap::Unwrap<QmlComponent>(info.This());

		// Getting errors
		QList<QQmlError> errs = obj_wrap->GetObject()->errors();

		// Create an array
		//Handle<Array> errArr = Array::New();
		Handle<Array> errArr = Nan::New<Array>();

		for (int i = 0; i < errs.length(); i++) {
			errArr->Set(i, Nan::New(errs[i].toString().toUtf8().constData()).ToLocalChecked());
		}

		info.GetReturnValue().Set(errArr);
	}
}
