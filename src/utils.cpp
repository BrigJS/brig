#include <nan.h>
#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QQmlComponent>
#include "utils.h"
#include "QuickItem.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	namespace Utils {

		Local<Value> QDataToV8(int type, void *value)
		{
			Nan::EscapableHandleScope scope;

			return scope.Escape(QVariantToV8(type, QVariant(type, value)));
		}

		Local<Value> QVariantToV8(int type, QVariant v)
		{
			Nan::EscapableHandleScope scope;

			Local<Object> obj = Nan::New<Object>();
			Nan::Set(obj, Nan::New("t").ToLocalChecked(), Nan::New<Uint32>(type));

			switch(type) {
			case QMetaType::Bool:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Boolean>(v.toBool()));
				break;
//				return scope.Escape(Nan::New<Boolean>(v.toBool()));

			case QMetaType::Int:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Int32>(v.toInt()));
				break;
//				return scope.Escape(Nan::New<Int32>(v.toInt()));

			case QMetaType::UInt:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Uint32>(v.toUInt()));
				break;
//				return scope.Escape(Nan::New<Uint32>(v.toUInt()));

			case QMetaType::Float:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Number>(v.toFloat()));
				break;
//				return scope.Escape(Nan::New<Number>(v.toFloat()));

			case QMetaType::Double:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Number>(v.toDouble()));
				break;
//				return scope.Escape(Nan::New<Number>(v.toDouble()));

			case QMetaType::LongLong:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Number>(v.toLongLong()));
				break;

//				return scope.Escape(Nan::New<Number>(v.toLongLong()));

			case QMetaType::ULongLong:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Number>(v.toULongLong()));
				break;
//				return scope.Escape(Nan::New<Number>(v.toULongLong()));

			case QMetaType::QString:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<String>(v.toString().toStdString()).ToLocalChecked());
				break;
				//return scope.Escape(Nan::New<String>(v.toString().toUtf8().constData()));
//				return scope.Escape(Nan::New<String>(v.toString().toStdString()).ToLocalChecked());

			case QMetaType::VoidStar:
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::Null());
				break;

			case QMetaType::User:
				Nan::Set(obj, Nan::New("t").ToLocalChecked(), Nan::New<Uint32>(QMetaType::QObjectStar));
			case QMetaType::QObjectStar:
			{
				Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Number>((long)qvariant_cast<QObject *>(v)));
				break;

//				return scope.Escape(Nan::New<Number>((long)qvariant_cast<QObject *>(v)));
			}
			default:

				if (type == qMetaTypeId<QQmlComponent::Status>()) {
					Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Number>(v.toInt()));
					//return scope.Escape(Nan::New<Number>(v.toInt()));
				} else if (type == qMetaTypeId<qreal>()) {
					Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::New<Number>(v.toDouble()));
					//return scope.Escape(Nan::New<Number>(v.toDouble()));
				} else {
					Nan::Set(obj, Nan::New("v").ToLocalChecked(), Nan::Undefined());
				}
			}

			return scope.Escape(obj);
//			return scope.Escape(Nan::Undefined());
		}

		QVariant BrigValueToQVariant(Local<Value> value)
		{
			QVariant v;

			Local<Object> brigValue = value->ToObject();

			int type = brigValue->Get(Nan::New("t").ToLocalChecked())->Int32Value();
			Local<Value> _value = brigValue->Get(Nan::New("v").ToLocalChecked());

			//if (type == QMetaType::QObjectStar || type == QMetaType::User) {
			if (type == QMetaType::QObjectStar) {
				v.setValue(QVariant(type, (QObject *)brigValue->Get(Nan::New("v").ToLocalChecked())->Int32Value()));
			} else if (type == QMetaType::User) {
				v.setValue(QVariant(type == QMetaType::QObjectStar, (QObject *)brigValue->Get(Nan::New("v").ToLocalChecked())->Int32Value()));
			} else if (_value->IsNull()) {
				v.setValue(QVariant(QMetaType::VoidStar, 0));
			} else if (_value->IsTrue() || _value->IsFalse() || _value->IsBoolean() ) {
				v.setValue(QVariant(_value->BooleanValue()));
			} else if (_value->IsNumber()) {
				v.setValue(QVariant(_value->NumberValue()));
			} else if (_value->IsInt32()) {
				v.setValue(QVariant(_value->Int32Value()));
			} else if (_value->IsString()) {
				String::Utf8Value _v(_value->ToString());
				v.setValue(QVariant(static_cast<char *>(*_v)));
			}

			return v;
		}

		QVariant V8ToQVariant(Local<Value> value)
		{
			QVariant v;

			// Check data type
			if (value->IsNull()) {
				v.setValue(QVariant(QMetaType::VoidStar, 0));
			} else if (value->IsTrue() || value->IsFalse() || value->IsBoolean() ) {
				v.setValue(QVariant(value->BooleanValue()));
			} else if (value->IsNumber()) {
				v.setValue(QVariant(value->NumberValue()));
			} else if (value->IsInt32()) {
				v.setValue(QVariant(value->Int32Value()));
			} else if (value->IsString()) {
				String::Utf8Value _v(value->ToString());
				v.setValue(QVariant(static_cast<char *>(*_v)));
			} else if (value->IsUint8Array()) {
				Local<Uint8Array> array = Local<Uint8Array>::Cast(value);

				QByteArray bytes;
				bytes.resize(array->ByteLength());
//				printf("%d\n", array->ByteLength());

				for (int i = 0; i < array->ByteLength(); i++) {
					bytes[i] = array->Get(i)->IntegerValue();
//					printf("%d\n", array->Get(i)->IntegerValue());
				}
				v.setValue(bytes);
			}

			// Undefined
			return v;
		}

		QJSValue V8ToQJSValue(QQmlEngine *engine, Local<Value> value)
		{
			// Check data type
			if (value->IsNull()) {
				return engine->evaluate("null");
			} else if (value->IsTrue() || value->IsFalse() || value->IsBoolean() ) {
				return engine->toScriptValue(value->BooleanValue());
			} else if (value->IsNumber()) {
				return engine->toScriptValue(value->NumberValue());
			} else if (value->IsInt32()) {
				return engine->toScriptValue(value->Int32Value());
			} else if (value->IsString()) {
				String::Utf8Value _v(value->ToString());
				return engine->toScriptValue(QString(*_v));
			}

			// TODO: support array and object
			return engine->evaluate("undefined");
		}

		ParamData *MakeParameter(int type, Local<Value> value)
		{
			// Null
			if (value->IsNull()) {
				return new ParamData();
			}

			switch(type) {
			case QMetaType::Bool:
				return new ParamData(static_cast<bool>(value->BooleanValue()));
			case QMetaType::Int:
				return new ParamData(static_cast<qint32>(value->Int32Value()));
			case QMetaType::UInt:
				return new ParamData(static_cast<quint32>(value->Uint32Value()));
			case QMetaType::Float:
				return new ParamData(static_cast<float>(value->NumberValue()));
			case QMetaType::Double:
				return new ParamData(static_cast<double>(value->NumberValue()));
			case QMetaType::LongLong:
				return new ParamData(static_cast<long long>(value->IntegerValue()));
			case QMetaType::ULongLong:
				return new ParamData(static_cast<unsigned long long>(value->IntegerValue()));
			case QMetaType::QString:
				return new ParamData(*String::Utf8Value(value->ToString()));
			case QMetaType::QVariant:
				return new ParamData(V8ToQVariant(value));
			}

			return NULL;
		}

		void printAllMeta(const QMetaObject *meta)
		{

			printf("== Print Meta: %s\n", meta->className());

			// Properties
			for (int i = meta->propertyOffset(); i < meta->propertyCount(); ++i) {
				QMetaProperty property = meta->property(i);
				const char *propName = property.name();
				printf("=== property [%d]: %s\n", i, propName);
			}

			// Properties
			for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
				QMetaMethod method = meta->method(i);
				const char *methodName = method.name();

				if (method.methodType() == QMetaMethod::Signal) {
					printf("=== signal [%d]: %s %s %d\n", i, methodName, method.methodSignature().data(), method.returnType());
				} else {
					printf("=== method [%d]: %s %s %d\n", i, methodName, method.methodSignature().data(), method.returnType());
				}
			}
		}
	}
}
