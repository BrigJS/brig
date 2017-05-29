#include <unistd.h>
#include <node.h>
#include <QObject>
#include "QmlTypeBuilder.h"
#include "DynamicQObject.h"

#define BRIG_QML_TYPE_IMPL(N) \
	template<> QMetaObject BrigQMLType<N>::staticMetaObject = QMetaObject(); \
	template<> DynamicQMetaObjectBuilder *BrigQMLType<N>::dynamicMetaObjectBuilder = 0;

#define BRIG_QML_TYPE_INIT(N) \
	BrigQMLType<N>::init(qmltype_builder->metaobject_builder); \
	qmlRegisterType<BrigQMLType<N>>(*uriStr, major, minor, qmltype_builder->metaobject_builder->getTypeName()); \
	break;

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
	BRIG_QML_TYPE_IMPL(3);
	BRIG_QML_TYPE_IMPL(4);
	BRIG_QML_TYPE_IMPL(5);
	BRIG_QML_TYPE_IMPL(6);
	BRIG_QML_TYPE_IMPL(7);
	BRIG_QML_TYPE_IMPL(8);
	BRIG_QML_TYPE_IMPL(9);
	BRIG_QML_TYPE_IMPL(10);
	BRIG_QML_TYPE_IMPL(11);
	BRIG_QML_TYPE_IMPL(12);
	BRIG_QML_TYPE_IMPL(13);
	BRIG_QML_TYPE_IMPL(14);
	BRIG_QML_TYPE_IMPL(15);
	BRIG_QML_TYPE_IMPL(16);
	BRIG_QML_TYPE_IMPL(17);
	BRIG_QML_TYPE_IMPL(18);
	BRIG_QML_TYPE_IMPL(19);
	BRIG_QML_TYPE_IMPL(20);
	BRIG_QML_TYPE_IMPL(21);
	BRIG_QML_TYPE_IMPL(22);
	BRIG_QML_TYPE_IMPL(23);
	BRIG_QML_TYPE_IMPL(24);
	BRIG_QML_TYPE_IMPL(25);
	BRIG_QML_TYPE_IMPL(26);
	BRIG_QML_TYPE_IMPL(27);
	BRIG_QML_TYPE_IMPL(28);
	BRIG_QML_TYPE_IMPL(29);
	BRIG_QML_TYPE_IMPL(30);

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
		Nan::SetPrototypeMethod(tpl, "addProperty", QmlTypeBuilder::addProperty);
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

		// Initialize and register QML Type class
		switch(type_nr) {
		case 0:
			BRIG_QML_TYPE_INIT(0);
		case 1:
			BRIG_QML_TYPE_INIT(1);
		case 2:
			BRIG_QML_TYPE_INIT(2);
		case 3:
			BRIG_QML_TYPE_INIT(3);
		case 4:
			BRIG_QML_TYPE_INIT(4);
		case 5:
			BRIG_QML_TYPE_INIT(5);
		case 6:
			BRIG_QML_TYPE_INIT(6);
		case 7:
			BRIG_QML_TYPE_INIT(7);
		case 8:
			BRIG_QML_TYPE_INIT(8);
		case 9:
			BRIG_QML_TYPE_INIT(9);
		case 10:
			BRIG_QML_TYPE_INIT(10);
		case 11:
			BRIG_QML_TYPE_INIT(11);
		case 12:
			BRIG_QML_TYPE_INIT(12);
		case 13:
			BRIG_QML_TYPE_INIT(13);
		case 14:
			BRIG_QML_TYPE_INIT(14);
		case 15:
			BRIG_QML_TYPE_INIT(15);
		case 16:
			BRIG_QML_TYPE_INIT(16);
		case 17:
			BRIG_QML_TYPE_INIT(17);
		case 18:
			BRIG_QML_TYPE_INIT(18);
		case 19:
			BRIG_QML_TYPE_INIT(19);
		case 20:
			BRIG_QML_TYPE_INIT(20);
		case 21:
			BRIG_QML_TYPE_INIT(21);
		case 22:
			BRIG_QML_TYPE_INIT(22);
		case 23:
			BRIG_QML_TYPE_INIT(23);
		case 24:
			BRIG_QML_TYPE_INIT(24);
		case 25:
			BRIG_QML_TYPE_INIT(25);
		case 26:
			BRIG_QML_TYPE_INIT(26);
		case 27:
			BRIG_QML_TYPE_INIT(27);
		case 28:
			BRIG_QML_TYPE_INIT(28);
		case 29:
			BRIG_QML_TYPE_INIT(29);
		case 30:
			BRIG_QML_TYPE_INIT(30);
		}

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

	NAN_METHOD(QmlTypeBuilder::addProperty) {

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		String::Utf8Value name(info[0]->ToString());

		qmltype_builder->metaobject_builder->addProperty(*name, info[1], info[2]);

		info.GetReturnValue().SetUndefined();
	}
}
