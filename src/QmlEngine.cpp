#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "QmlEngine.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Nan::Persistent<Function> QmlEngineWrap::constructor;

	QmlEngineWrap::QmlEngineWrap() : ObjectWrap()
	{
		obj = new QQmlEngine();
		obj->setOutputWarningsToStandardError(false);
		signal = new SignalHandler(obj);
	}

	QmlEngineWrap::~QmlEngineWrap()
	{
printf("RELEASE ENGINE\n");
		delete signal;
		delete obj;
	}

	NAN_MODULE_INIT(QmlEngineWrap::Initialize) {

		Local<String> name = Nan::New("QmlEngine").ToLocalChecked();

		/* Constructor template */
		Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(QmlEngineWrap::New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		Nan::SetPrototypeMethod(tpl, "on", QmlEngineWrap::on);
//		Nan::SetPrototypeMethod(tpl, "rootContext", QmlEngineWrap::rootContext);

		//constructor = Nan::Persistent<Function>::New(tpl->GetFunction());
		constructor.Reset(tpl->GetFunction());
		//NanAssignPersistent(constructor, tpl->GetFunction());

		target->Set(name, Nan::New(constructor));
	}

	NAN_METHOD(QmlEngineWrap::New) {

		QmlEngineWrap *obj_wrap = new QmlEngineWrap();
		obj_wrap->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QmlEngineWrap::on) {

		QmlEngineWrap *obj_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(info.This());

		// Signal name
		String::Utf8Value url(info[0]->ToString());

		int id = obj_wrap->signal->addCallback(*url, info[1]);

		info.GetReturnValue().Set(info.This());
	}
/*
	Local<Value> QmlEngineWrap::rootContext(const Arguments& args)
	{
		HandleScope scope;

		QmlEngineWrap *obj_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(info.This());

		QQmlEngine *engine = obj_wrap->GetObject();
		QQmlContext *context = engine->rootContext();

		Local<Value> instance = QmlContextWrap::NewInstance(context);

		return scope.Close(instance);
	}
*/
}
