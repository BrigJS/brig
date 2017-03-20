#include <unistd.h>
#include <node.h>
#include <QObject>
#include "QmlTypeBuilder.h"
#include "DynamicQObject.h"

#define BRIG_QML_TYPE_IMPL(N) \
		template<> QMetaObject BrigQMLType<N>::staticMetaObject = QMetaObject(); \
		template<> DynamicQMetaObjectBuilder *BrigQMLType<N>::dynamicMetaObjectBuilder = 0;

namespace Brig {

	using namespace v8;
	using namespace node;

	template <int N>
	class BrigQMLType : public DynamicQObject {
		public:
			BrigQMLType() : DynamicQObject(dynamicMetaObjectBuilder, &staticMetaObject, 0) {};

			static void init(DynamicQMetaObjectBuilder *metaObjectBuilder) {
				dynamicMetaObjectBuilder = metaObjectBuilder;
				static_cast<QMetaObject &>(staticMetaObject) = *reinterpret_cast<QMetaObject *>(metaObjectBuilder->build());
			}

			static DynamicQMetaObjectBuilder *dynamicMetaObjectBuilder;
			static QMetaObject staticMetaObject;
	};

	// Reserved type objects
	BRIG_QML_TYPE_IMPL(0);
	BRIG_QML_TYPE_IMPL(1);
	BRIG_QML_TYPE_IMPL(2);
	BRIG_QML_TYPE_IMPL(4);
	BRIG_QML_TYPE_IMPL(5);

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
		Nan::SetPrototypeMethod(tpl, "addMethod", QmlTypeBuilder::addMethod);
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
		int type_nr = static_cast<int>(info[3]->Int32Value());

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		//TODO: acorrding to type_nr to select class to use for new type

		// Initializing QML Type class
		BrigQMLType<0>::init(qmltype_builder->metaobject_builder);

		// Register QML type
		qmlRegisterType<BrigQMLType<0>>(*uriStr, major, minor, qmltype_builder->metaobject_builder->getTypeName());

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QmlTypeBuilder::addSignal) {

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		String::Utf8Value name(info[0]->ToString());
		String::Utf8Value signature(info[1]->ToString());
		Handle<Array> parameters = Handle<Array>::Cast(info[2]);

		// Convert parameters
		QList<QByteArray> arguments;
		Handle<Value> val;
		for (unsigned int i = 0; i < parameters->Length(); i++) {
			val = parameters->Get(i);
			arguments << QByteArray(*String::Utf8Value(val));
		}

		qmltype_builder->metaobject_builder->addSignal(*name, *signature, arguments, info[3]);

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QmlTypeBuilder::addMethod) {

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		String::Utf8Value name(info[0]->ToString());
		String::Utf8Value signature(info[1]->ToString());
		Handle<Array> parameters = Handle<Array>::Cast(info[2]);

		// Convert parameters
		QList<QByteArray> arguments;
		Handle<Value> val;
		for (unsigned int i = 0; i < parameters->Length(); i++) {
			val = parameters->Get(i);
			arguments << QByteArray(*String::Utf8Value(val));
		}

		qmltype_builder->metaobject_builder->addMethod(*name, *signature, arguments, info[3]);

		info.GetReturnValue().SetUndefined();
	}
}
