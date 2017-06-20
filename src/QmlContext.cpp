#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "QmlContext.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Nan::Persistent<Function> QmlContext::constructor;

	QmlContext::QmlContext() : ObjectWrap()
	{
		obj = NULL;
	}

	QmlContext::~QmlContext()
	{
		delete obj;
	}

	NAN_MODULE_INIT(QmlContext::Initialize) {

		Local<String> name = Nan::New("QmlContext").ToLocalChecked();

		/* Constructor template */
		Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(QmlContext::New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		//Nan::SetPrototypeMethod(tpl, "setEngine", QmlContext::setEngine);
		Nan::SetPrototypeMethod(tpl, "setContextProperty", QmlContext::setContextProperty);
		Nan::SetPrototypeMethod(tpl, "getContextProperty", QmlContext::getContextProperty);

		constructor.Reset(tpl->GetFunction());

		target->Set(name, Nan::New(constructor));
	}

	// Prototype Constructor
	NAN_METHOD(QmlContext::New) {

		if (info.Length() == 0)
			info.GetReturnValue().SetUndefined();

		// Using Engine to initialize QQmlContext
		QmlEngineWrap *engine_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(info[0]->ToObject());

		QmlContext *obj_wrap = new QmlContext();
		obj_wrap->obj = engine_wrap->GetObject()->rootContext();
		obj_wrap->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	// Method
	NAN_METHOD(QmlContext::setContextProperty) {

		QmlContext *context_wrap = ObjectWrap::Unwrap<QmlContext>(info.This());

		String::Utf8Value propName(info[0]->ToString());
		String::Utf8Value value(info[1]->ToString());

		context_wrap->obj->setContextProperty(QString(*propName), Utils::V8ToQVariant(info[1]));

		info.GetReturnValue().Set(Nan::Undefined());
	}

	NAN_METHOD(QmlContext::getContextProperty) {

		QmlContext *context_wrap = ObjectWrap::Unwrap<QmlContext>(info.This());

		String::Utf8Value propName(info[0]->ToString());

		QVariant v;
		v.setValue(context_wrap->obj->contextProperty(QString(*propName)));

		info.GetReturnValue().Set(Utils::QVariantToV8(v.userType(), v));
	}
}
