#include <unistd.h>
#include <node.h>
#include "QmlTypeBuilder.h"
#include "DynamicQObject.h"

#define BRIG_QML_TYPE_IMPL(N) \
	template<> QMetaObject BrigQMLType<N>::staticMetaObject = QMetaObject(); \
	template<> QmlTypeBuilder *BrigQMLType<N>::typeBuilder = 0;

#define BRIG_QML_TYPE_INIT(N) \
	BrigQMLType<N>::init(qmltype_builder); \
	qmlRegisterType<BrigQMLType<N>>(*uriStr, major, minor, qmltype_builder->metaObjectBuilder()->getTypeName()); \
	break;

namespace Brig {

	using namespace v8;
	using namespace node;
	template <int N>
	class BrigQMLType : public DynamicQObject {
		public:
			BrigQMLType(QObject *parent = 0) : DynamicQObject(typeBuilder, &staticMetaObject, parent) {
				setParent(parent);
				typeBuilder->createInstance(this);
			};

			static void init(QmlTypeBuilder *_typeBuilder) {
				typeBuilder = _typeBuilder;
				static_cast<QMetaObject &>(staticMetaObject) = *reinterpret_cast<QMetaObject *>(_typeBuilder->metaObjectBuilder()->build());
			}

			const QMetaObject *metaObject() const { return &staticMetaObject; };

			static QmlTypeBuilder *typeBuilder;
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

	QmlTypeBuilder::QmlTypeBuilder(const char *_typeName) : ObjectWrap()
	{
		typeName = _typeName;
		metaobject_builder = new DynamicQMetaObjectBuilder(typeName);
		obj = NULL;
	}

	QmlTypeBuilder::~QmlTypeBuilder()
	{
		delete typeName;
		delete metaobject_builder;
		delete listener;
	}

	void QmlTypeBuilder::createInstance(DynamicQObject *instance)
	{
		Nan::HandleScope scope;

		instance->setId(instCounter++);

		// Store this instance
		TypeInstance *typeInstance = new TypeInstance();
		typeInstance->id = instance->getId();
		typeInstance->instance = instance;
		instances.append(typeInstance);

		// Prepare arguments
		int argc = 2;
		Handle<Value> *argv = new Handle<Value>[argc];
		argv[0] = Nan::New<String>("created").ToLocalChecked();
		argv[1] = Nan::New<Number>(typeInstance->id);

		// Invoke
		listener->Call(argc, argv);
	}

	TypeInstance *QmlTypeBuilder::findInstance(int _id)
	{
		// Finding instance
		for (int i = 0; i < instances.size(); ++i) {
			TypeInstance *instance = instances.at(i);
			if (instance->id == _id)
				return instance;
		}

		return NULL;
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
		Nan::SetPrototypeMethod(tpl, "addSignalListener", QmlTypeBuilder::addSignalListener);
		Nan::SetPrototypeMethod(tpl, "setListener", QmlTypeBuilder::setListener);
		Nan::SetPrototypeMethod(tpl, "emitSignal", QmlTypeBuilder::emitSignal);
		Nan::SetPrototypeMethod(tpl, "invokeMethod", QmlTypeBuilder::invokeMethod);

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

		qmltype_builder->metaobject_builder->addSignal(*name, *signature, arguments);

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

	NAN_METHOD(QmlTypeBuilder::addSignalListener) {

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		qmltype_builder->metaobject_builder->addSignalListener(info[0]);

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QmlTypeBuilder::setListener) {

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		qmltype_builder->listener = new Nan::Callback(info[0].As<Function>());

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QmlTypeBuilder::emitSignal) {

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

//		qmltype_builder->emitSignal();

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QmlTypeBuilder::invokeMethod) {

		QmlTypeBuilder *qmltype_builder = ObjectWrap::Unwrap<QmlTypeBuilder>(info.This());

		if (!info[0]->IsNumber())
			Nan::ThrowTypeError("First argument must be a number");

		if (!info[1]->IsString())
			Nan::ThrowTypeError("Second argument must be a string");

		// Getting instance
		TypeInstance *inst = qmltype_builder->findInstance(info[0]->NumberValue());
		if (inst == NULL) {

			// No such instance
			info.GetReturnValue().SetUndefined();
			return;
		}

		// Method name
		String::Utf8Value methodSig(info[1]->ToString());

		QVariant returnedValue;
		int infoLen = info.Length() - 2;

		// It supports only 10 arguments with limitation of Qt
		inst->instance->invokeMethod(*methodSig,
			Qt::AutoConnection,
			Q_RETURN_ARG(QVariant, returnedValue),
			(infoLen > 0) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[2])) : QGenericArgument(),
			(infoLen > 1) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[3])) : QGenericArgument(),
			(infoLen > 2) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[4])) : QGenericArgument(),
			(infoLen > 3) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[5])) : QGenericArgument(),
			(infoLen > 4) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[6])) : QGenericArgument(),
			(infoLen > 5) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[7])) : QGenericArgument(),
			(infoLen > 6) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[8])) : QGenericArgument(),
			(infoLen > 7) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[9])) : QGenericArgument(),
			(infoLen > 8) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[10])) : QGenericArgument(),
			(infoLen > 9) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[11])) : QGenericArgument());

		// Convert Qvariant to V8 data type
		if (returnedValue.isNull()) {
			info.GetReturnValue().SetNull();
			return;
		}

		switch(returnedValue.userType()) {
		case QMetaType::Bool:
			info.GetReturnValue().Set(Nan::New<Boolean>(returnedValue.toBool()));
			break;
		case QMetaType::Int:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toInt()));
			break;
		case QMetaType::UInt:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toUInt()));
			break;
		case QMetaType::Float:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toFloat()));
			break;
		case QMetaType::Double:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toDouble()));
			break;
		case QMetaType::LongLong:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toLongLong()));
			break;
		case QMetaType::ULongLong:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toULongLong()));
			break;
		case QMetaType::QString:
			info.GetReturnValue().Set(Nan::New(returnedValue.toString().toUtf8().constData()).ToLocalChecked());
			break;
		case QMetaType::QColor:
			info.GetReturnValue().Set(Nan::New(returnedValue.value<QColor>().name(QColor::HexArgb).toUtf8().constData()).ToLocalChecked());
			break;
		default:
			info.GetReturnValue().SetUndefined();
		}

	}
}
