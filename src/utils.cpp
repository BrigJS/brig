#include <QObject>
#include <QVariant>
#include <QMetaType>
#include <QQmlComponent>
#include "utils.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	namespace Utils {

		Handle<Value> QDataToV8(int type, void *value)
		{
			HandleScope scope;

			return scope.Close(QVariantToV8(type, QVariant(type, value)));
		}

		Handle<Value> QVariantToV8(int type, QVariant v)
		{
			HandleScope scope;

			Handle<Value> result = Null();

			switch(type) {
			case QMetaType::Bool:
				return scope.Close(Boolean::New(v.toBool()));

			case QMetaType::Int:
				return scope.Close(Number::New(v.toInt()));

			case QMetaType::UInt:
				return scope.Close(Number::New(v.toUInt()));

			case QMetaType::Float:
				return scope.Close(Number::New(v.toFloat()));

			case QMetaType::Double:
				return scope.Close(Number::New(v.toDouble()));

			case QMetaType::LongLong:

				return scope.Close(Number::New(v.toLongLong()));

			case QMetaType::ULongLong:
				return scope.Close(Number::New(v.toULongLong()));

			case QMetaType::QString:
				return scope.Close(String::New(v.toString().toUtf8().constData()));

			default:

				if (type == qMetaTypeId<QQmlComponent::Status>()) {
					return scope.Close(Number::New(v.toInt()));
				} else if (type == qMetaTypeId<qreal>()) {
					return scope.Close(Number::New(v.toDouble()));
				}

//			case QMetaType::QVariant:
			}

			return scope.Close(Undefined());
		}

		QVariant V8ToQVariant(Handle<Value> value)
		{
			QVariant v;

			// Check data type
			if (value->IsNull()) {
				v.setValue(QVariant(QMetaType::VoidStar, 0));
			} else if (value->IsTrue() || value->IsFalse() || value->IsBoolean() ) {
				v.setValue(QVariant(value->ToBoolean()->Value()));
			} else if (value->IsNumber()) {
				v.setValue(QVariant(value->NumberValue()));
			} else if (value->IsInt32()) {
				v.setValue(QVariant(value->ToInt32()->Value()));
			} else if (value->IsString()) {
				String::Utf8Value _v(value->ToString());
				v.setValue(QVariant(static_cast<char *>(*_v)));
			}

			// Undefined
			return v;
		}

		QJSValue V8ToQJSValue(QQmlEngine *engine, Handle<Value> value)
		{
			// Check data type
			if (value->IsNull()) {
				return engine->evaluate("null");
			} else if (value->IsTrue() || value->IsFalse() || value->IsBoolean() ) {
				return engine->toScriptValue(value->ToBoolean()->Value());
			} else if (value->IsNumber()) {
				return engine->toScriptValue(value->NumberValue());
			} else if (value->IsInt32()) {
				return engine->toScriptValue(value->ToInt32()->Value());
			} else if (value->IsString()) {
				String::Utf8Value _v(value->ToString());
				return engine->toScriptValue(QString(*_v));
			}

			// TODO: support array and object
			return engine->evaluate("undefined");
		}

		ParamData *MakeParameter(int type, Handle<Value> value)
		{
			// Null
			if (value->IsNull()) {
				return new ParamData();
			}

			switch(type) {
			case QMetaType::Bool:
				return new ParamData(static_cast<bool>(value->ToBoolean()->Value()));
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
	}
}
