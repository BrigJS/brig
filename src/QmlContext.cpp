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
		obj_wrap->obj = new QQmlContext(engine_wrap->GetObject()->rootContext());
		obj_wrap->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	// Method
}
