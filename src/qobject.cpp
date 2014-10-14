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
		NODE_SET_PROTOTYPE_METHOD(tpl, "setParent", QObjectWrap::setParent);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getProperty", QObjectWrap::getProperty);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setProperty", QObjectWrap::setProperty);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getPropertyNames", QObjectWrap::getPropertyNames);

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
			return ThrowException(Exception::Error(String::New("Not a QuickWindow")));

		return scope.Close(QuickWindowWrap::NewInstance(args.This()));
	}

	Handle<Value> QObjectWrap::getProperty(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		String::Utf8Value name(args[0]->ToString());

		// Set property
		QVariant v = obj_wrap->GetObject()->property(*name);
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
}
