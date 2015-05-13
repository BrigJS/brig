#include <unistd.h>
#include <node.h>
#include <QObject>
#include "QmlTypeBuilder.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QmlTypeBuilder::constructor;

	QmlTypeBuilder::QmlTypeBuilder(const char *typeName) : ObjectWrap()
	{
		metaobject_builder = new DynamicQMetaObjectBuilder(typeName);
		obj = NULL;
	}

	QmlTypeBuilder::~QmlTypeBuilder()
	{
		delete metaobject_builder;
	}

	void QmlTypeBuilder::Initialize(Handle<Object> target)
	{
		NanScope();

		Local<String> name = NanNew("QmlTypeBuilder");

		/* Constructor template */
		Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(QmlTypeBuilder::New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "addSignal", QmlTypeBuilder::addSignal);
		NODE_SET_PROTOTYPE_METHOD(tpl, "build", QmlTypeBuilder::build);

		NanAssignPersistent(constructor, tpl->GetFunction());

		target->Set(name, NanNew(constructor));
	}

	NAN_METHOD(QmlTypeBuilder::New) {
		NanScope();

		String::Utf8Value typeName(args[0]->ToString());

		QmlTypeBuilder *obj_wrap = new QmlTypeBuilder(*typeName);
		obj_wrap->Wrap(args.This());

		NanReturnThis();
	}

	NAN_METHOD(QmlTypeBuilder::build) {
		NanScope();

		String::Utf8Value uriStr(args[0]->ToString());
		int major = static_cast<int>(args[1]->Int32Value());
		int minor = static_cast<int>(args[2]->Int32Value());

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(args.This());

		// Generate a new QMetaObject
		QMetaObject *metaobject = qmltype_builder->metaobject_builder->build();

		// Create a new QObject
		qmltype_builder->obj = new DynamicQObject(metaobject);

		qmlRegisterType<QObject>(*uriStr, major, minor, qmltype_builder->metaobject_builder->getTypeName());

		NanReturnUndefined();
	}


	NAN_METHOD(QmlTypeBuilder::addSignal) {
		NanScope();

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(args.This());

		String::Utf8Value signature(args[0]->ToString());

		QStringList parameterNames;

		qmltype_builder->metaobject_builder->addSignal(*signature, parameterNames, args[2]);

		NanReturnUndefined();
	}
}
