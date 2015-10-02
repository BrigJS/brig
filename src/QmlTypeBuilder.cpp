#include <unistd.h>
#include <node.h>
#include <QObject>
#include "QmlTypeBuilder.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Nan::Persistent<Function> QmlTypeBuilder::constructor;

	QmlTypeBuilder::QmlTypeBuilder(const char *typeName) : ObjectWrap()
	{
		metaobject_builder = new DynamicQMetaObjectBuilder(typeName);
		obj = NULL;
	}

	QmlTypeBuilder::~QmlTypeBuilder()
	{
		delete metaobject_builder;
	}

	NAN_MODULE_INIT(QmlTypeBuilder::Initialize) {

		Local<String> name = Nan::New("QmlTypeBuilder").ToLocalChecked();

		/* Constructor template */
		Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(QmlTypeBuilder::New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		Nan::SetPrototypeMethod(tpl, "addSignal", QmlTypeBuilder::addSignal);
		Nan::SetPrototypeMethod(tpl, "build", QmlTypeBuilder::build);

		constructor.Reset(tpl->GetFunction());

		target->Set(name, Nan::New(constructor));
	}

	NAN_METHOD(QmlTypeBuilder::New) {

		String::Utf8Value typeName(info[0]->ToString());

		QmlTypeBuilder *obj_wrap = new QmlTypeBuilder(*typeName);
		obj_wrap->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QmlTypeBuilder::build) {

		String::Utf8Value uriStr(info[0]->ToString());
		int major = static_cast<int>(info[1]->Int32Value());
		int minor = static_cast<int>(info[2]->Int32Value());

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		// Generate a new QMetaObject
		QMetaObject *metaobject = qmltype_builder->metaobject_builder->build();

		// Create a new QObject
		qmltype_builder->obj = new DynamicQObject(metaobject);

		qmlRegisterType<QObject>(*uriStr, major, minor, qmltype_builder->metaobject_builder->getTypeName());

		info.GetReturnValue().SetUndefined();
	}


	NAN_METHOD(QmlTypeBuilder::addSignal) {

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		String::Utf8Value signature(info[0]->ToString());

		QStringList parameterNames;

		qmltype_builder->metaobject_builder->addSignal(*signature, parameterNames, info[2]);

		info.GetReturnValue().SetUndefined();
	}
}
