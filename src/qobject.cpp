#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "qobject.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QObjectWrap::constructor;

	QObjectWrap::QObjectWrap() : ObjectWrap()
	{
	}

	QObjectWrap::QObjectWrap(QObject *object) : ObjectWrap()
	{
		obj = object;
	}

	QObjectWrap::~QObjectWrap()
	{
		delete obj;
	}

	void QObjectWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QObject");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QObjectWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "toQuickWindow", QObjectWrap::toQuickWindow);
		NODE_SET_PROTOTYPE_METHOD(tpl, "toQuickItem", QObjectWrap::toQuickItem);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setParent", QObjectWrap::setParent);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getProperty", QObjectWrap::getProperty);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setProperty", QObjectWrap::setProperty);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getPropertyNames", QObjectWrap::getPropertyNames);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getMethods", QObjectWrap::getMethods);
		NODE_SET_PROTOTYPE_METHOD(tpl, "emit", QObjectWrap::emitEvent);
		NODE_SET_PROTOTYPE_METHOD(tpl, "invokeMethod", QObjectWrap::invokeMethod);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QObjectWrap::New(const Arguments& args)
	{
		HandleScope scope;

		if (args[0]->ToObject()->InternalFieldCount() >= 2) {

			BrigContainerType type = static_cast<BrigContainerType>(args[0]->ToObject()->GetInternalField(1)->ToInteger()->Value());

			if (type == BRIG_CONTAINER_NATIVE) {
				QObject *object = ObjectWrap::Unwrap<QObject>(args[0]->ToObject());
				QObjectWrap *obj_wrap = new QObjectWrap(object);
				obj_wrap->Wrap(args.This());
				return args.This();
			}
		}

		QObjectWrap *obj_wrap = new QObjectWrap();
		obj_wrap->Wrap(args.This());

		return scope.Close(args[0]);
	}

	Handle<Value> QObjectWrap::NewInstance(QObject *qobject)
	{
		HandleScope scope;

		// Create a container
		Handle<ObjectTemplate> object_template = ObjectTemplate::New();
		object_template->SetInternalFieldCount(2);
		Persistent<ObjectTemplate> object_instance = Persistent<ObjectTemplate>::New(object_template);
		Local<Object> obj = object_instance->NewInstance();

		// Wrap
		obj->SetInternalField(0, External::New(qobject));
		obj->SetInternalField(1, Integer::New(BRIG_CONTAINER_NATIVE));

		const unsigned argc = 1;
		Handle<Value> argv[argc] = { obj };

		Local<Object> instance = constructor->NewInstance(argc, argv);

		return scope.Close(instance);
	}

	Handle<Value> QObjectWrap::setParent(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());
		QObjectWrap *wrap = ObjectWrap::Unwrap<QObjectWrap>(args[0]->ToObject());

		obj_wrap->GetObject()->setParent(wrap->GetObject());

		return scope.Close(Undefined());
	}

	Handle<Value> QObjectWrap::toQuickWindow(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		// It's not a window component
		if (!obj_wrap->GetObject()->isWindowType())
			return ThrowException(Exception::Error(String::New("Not a QuickWindow object")));

		return scope.Close(QuickWindowWrap::NewInstance(args.This()));
	}

	Handle<Value> QObjectWrap::toQuickItem(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		// It's not a window component
		if (!obj_wrap->GetObject()->inherits("QQuickItem"))
			return ThrowException(Exception::Error(String::New("Not a QuickItem object")));

		return scope.Close(QuickItemWrap::NewInstance(args.This()));
	}

	Handle<Value> QObjectWrap::getProperty(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		String::Utf8Value name(args[0]->ToString());

		// Get property
		QVariant v = obj_wrap->GetObject()->property(*name);

		// Convert Qvariant to V8 data type
		if (v.isNull())
			return scope.Close(Null());

		switch(v.userType()) {
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
		case QMetaType::QColor:
			return scope.Close(String::New(v.value<QColor>().name(QColor::HexArgb).toUtf8().constData()));
		}

		return scope.Close(Undefined());
	}

	Handle<Value> QObjectWrap::setProperty(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		String::Utf8Value name(args[0]->ToString());
		Handle<Value> value(args[1]);
		QVariant v;

		// Check data type
		if (value->IsTrue() || value->IsFalse() || value->IsBoolean() ) {
			v.setValue(QVariant(value->ToBoolean()->Value()));
		} else if (value->IsNumber()) {
			v.setValue(QVariant(value->NumberValue()));
		} else if (value->IsInt32()) {
			v.setValue(QVariant(value->ToInt32()->Value()));
		} else if (value->IsString()) {
			String::Utf8Value _v(value->ToString());
			v.setValue(QVariant(static_cast<char *>(*_v)));
		}

		// Set property
		obj_wrap->GetObject()->setProperty(*name, v);

		return scope.Close(Undefined());
	}

	Handle<Value> QObjectWrap::getPropertyNames(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		Handle<Array> keys = Array::New();

		// Getting property names
		static const QMetaObject *meta = obj_wrap->GetObject()->metaObject();
		for (int i = 0; i < meta->propertyCount(); i++) {
			keys->Set(i, String::New(QString(meta->property(i).name()).toUtf8().constData()));
		}

		return scope.Close(keys);
	}

	Handle<Value> QObjectWrap::getMethods(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		Handle<Object> obj = Object::New();

		static const QMetaObject *meta = obj_wrap->GetObject()->metaObject();
		for (int i = meta->methodOffset(); i < meta->methodCount(); ++i)
//		for(int i = 0; i < meta->methodCount(); ++i)
			obj->Set(i, String::New(meta->method(i).methodSignature().data()));

		return scope.Close(obj);		
	}

	Handle<Value> QObjectWrap::emitEvent(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		// Method name
		String::Utf8Value methodSig(args[0]->ToString());

		QVariant returnedValue;
		int argsLen = args.Length() - 1;

		static const QMetaObject *meta = obj_wrap->GetObject()->metaObject();
//		int methodIndex = meta->indexOfMethod(*methodSig);
//		QMetaMethod method = meta->method(methodIndex);
//
		// Getting currect context
		QQmlContext *thisContext = QQmlEngine::contextForObject(obj_wrap->GetObject());

		// Preparing parameter and ensure QJSValue have individual memory
		QJSValue val[argsLen];
		QList<QGenericArgument> parameters;
		QList<Utils::ParamData> dataList;

		// Getting signature
		for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
			const char *sig = meta->method(i).methodSignature().data();

			// Check method name
			if (strncmp(sig, *methodSig, args[0]->ToString()->Length()) != 0)
				continue;

			if (*(sig + args[0]->ToString()->Length()) != '(')
				continue;

			// Getting method object with signature
			int methodIndex = meta->indexOfMethod(sig);
			QMetaMethod method = meta->method(methodIndex);

			// Getting parameter types
			for (int j = 0; j < method.parameterCount(); j++) {
				int type = method.parameterType(j);

				if (j >= args.Length())
					break;

				Handle<Value> value = args[j + 1];

				// Type is "var" in QML, which is different from QVariant
				if (type == qMetaTypeId<QJSValue>()) {
					val[j] = Utils::V8ToQJSValue(thisContext->engine(), value);

					// Undefined
					parameters << QGenericArgument("QJSValue", static_cast<const void *>(&val[j]));
					continue;
				}

				// Making arguments
				Utils::ParamData *data = Utils::MakeParameter(type, value);
				if (data == NULL) {
					// Unknown type, set Undefined to this parameter
					parameters << QGenericArgument();
					continue;
				}

				dataList << data;
				parameters << QGenericArgument(QMetaType::typeName(type), data->ptr);
			}

			// Invoke
			QMetaObject::invokeMethod(obj_wrap->GetObject(), *methodSig,
				Qt::QueuedConnection,
				(argsLen > 0) ? parameters[0] : QGenericArgument(),
				(argsLen > 1) ? parameters[1] : QGenericArgument(),
				(argsLen > 2) ? parameters[2] : QGenericArgument(),
				(argsLen > 3) ? parameters[3] : QGenericArgument(),
				(argsLen > 4) ? parameters[4] : QGenericArgument(),
				(argsLen > 5) ? parameters[5] : QGenericArgument(),
				(argsLen > 6) ? parameters[6] : QGenericArgument(),
				(argsLen > 7) ? parameters[7] : QGenericArgument(),
				(argsLen > 8) ? parameters[8] : QGenericArgument(),
				(argsLen > 9) ? parameters[9] : QGenericArgument());

			// Release
			dataList.clear();

			break;
		}

		return Undefined();
	}

	Handle<Value> QObjectWrap::invokeMethod(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		// Method name
		String::Utf8Value methodSig(args[0]->ToString());

		QVariant returnedValue;
		int argsLen = args.Length() - 1;

		// It supports only 10 arguments with limitation of Qt
		QMetaObject::invokeMethod(obj_wrap->GetObject(), *methodSig,
			Qt::DirectConnection,
			Q_RETURN_ARG(QVariant, returnedValue),
			(argsLen > 0) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[1])) : QGenericArgument(),
			(argsLen > 1) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[2])) : QGenericArgument(),
			(argsLen > 2) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[3])) : QGenericArgument(),
			(argsLen > 3) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[4])) : QGenericArgument(),
			(argsLen > 4) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[5])) : QGenericArgument(),
			(argsLen > 5) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[6])) : QGenericArgument(),
			(argsLen > 6) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[7])) : QGenericArgument(),
			(argsLen > 7) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[8])) : QGenericArgument(),
			(argsLen > 8) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[8])) : QGenericArgument(),
			(argsLen > 9) ? Q_ARG(QVariant, Utils::V8ToQVariant(args[9])) : QGenericArgument());

		// Convert Qvariant to V8 data type
		if (returnedValue.isNull())
			return scope.Close(Null());

		switch(returnedValue.userType()) {
		case QMetaType::Bool:
			return scope.Close(Boolean::New(returnedValue.toBool()));
		case QMetaType::Int:
			return scope.Close(Number::New(returnedValue.toInt()));
		case QMetaType::UInt:
			return scope.Close(Number::New(returnedValue.toUInt()));
		case QMetaType::Float:
			return scope.Close(Number::New(returnedValue.toFloat()));
		case QMetaType::Double:
			return scope.Close(Number::New(returnedValue.toDouble()));
		case QMetaType::LongLong:
			return scope.Close(Number::New(returnedValue.toLongLong()));
		case QMetaType::ULongLong:
			return scope.Close(Number::New(returnedValue.toULongLong()));
		case QMetaType::QString:
			return scope.Close(String::New(returnedValue.toString().toUtf8().constData()));
		case QMetaType::QColor:
			return scope.Close(String::New(returnedValue.value<QColor>().name(QColor::HexArgb).toUtf8().constData()));
		}

		return scope.Close(Undefined());
	}
}
