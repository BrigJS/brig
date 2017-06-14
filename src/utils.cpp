#include <nan.h>
#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QQmlComponent>
#include "utils.h"

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

			switch(type) {
			case QMetaType::Bool:
				//return scope.Escape(Boolean::New(v.toBool()));
				return scope.Escape(Nan::New<Boolean>(v.toBool()));

			case QMetaType::Int:
				//return scope.Escape(Nan::New<Number>(v.toInt()));
				return scope.Escape(Nan::New<Int32>(v.toInt()));

			case QMetaType::UInt:
				return scope.Escape(Nan::New<Uint32>(v.toUInt()));

			case QMetaType::Float:
				return scope.Escape(Nan::New<Number>(v.toFloat()));

			case QMetaType::Double:
				return scope.Escape(Nan::New<Number>(v.toDouble()));

			case QMetaType::LongLong:

				return scope.Escape(Nan::New<Number>(v.toLongLong()));

			case QMetaType::ULongLong:
				return scope.Escape(Nan::New<Number>(v.toULongLong()));

			case QMetaType::QString:
				//return scope.Escape(Nan::New<String>(v.toString().toUtf8().constData()));
				return scope.Escape(Nan::New<String>(v.toString().toStdString()).ToLocalChecked());

			default:

				if (type == qMetaTypeId<QQmlComponent::Status>()) {
					return scope.Escape(Nan::New<Number>(v.toInt()));
				} else if (type == qMetaTypeId<qreal>()) {
					return scope.Escape(Nan::New<Number>(v.toDouble()));
				}

//			case QMetaType::QVariant:
			}

			return scope.Escape(Nan::Undefined());
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
