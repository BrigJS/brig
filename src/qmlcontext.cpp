#include <node.h>
#include <QtGui>
#include <QObject>
#include "qmlcontext.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QmlContextWrap::constructor;

	QmlContextWrap::QmlContextWrap(QmlContextWrap *context_wrap) : ObjectWrap()
	{
		obj = new QQmlContext(context_wrap->GetObject());
		prototype_object = QObjectWrap::NewInstance(obj);
	}

	QmlContextWrap::QmlContextWrap(QQmlContext *context) : ObjectWrap()
	{
		obj = new QQmlContext(context);
		prototype_object = QObjectWrap::NewInstance(obj);
	}

	QmlContextWrap::QmlContextWrap(Handle<Value> object) : ObjectWrap()
	{
		if (strcmp(*String::Utf8Value(object->ToObject()->GetConstructorName()), "QmlContext") == 0) {
			QmlContextWrap *wrap = ObjectWrap::Unwrap<QmlContextWrap>(object->ToObject());
			obj = wrap->GetObject();
		} else {
			QObjectWrap *wrap = ObjectWrap::Unwrap<QObjectWrap>(object->ToObject());
			obj = qobject_cast<QQmlContext *>(wrap->GetObject());
		}

		prototype_object = object;
	}

	QmlContextWrap::~QmlContextWrap()
	{
		delete obj;
	}

	void QmlContextWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QmlContext");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QmlContextWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "toObject", QmlContextWrap::toObject);
		NODE_SET_PROTOTYPE_METHOD(tpl, "contextProperty", QmlContextWrap::contextProperty);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QmlContextWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QmlContextWrap *obj_wrap = new QmlContextWrap(args[0]);
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QmlContextWrap::NewInstance(QQmlContext *context)
	{
		HandleScope scope;

		const unsigned argc = 1;
		Handle<Value> obj = QObjectWrap::NewInstance(context);
		Handle<Value> argv[argc] = { obj };
		Handle<Value> instance = constructor->NewInstance(argc, argv);

		return scope.Close(instance);
	}

	Handle<Value> QmlContextWrap::toObject(const Arguments& args)
	{
		HandleScope scope;
		
		QmlContextWrap *obj_wrap = QmlContextWrap::Unwrap<QmlContextWrap>(args.This());
		QObject *obj = obj_wrap->GetObject()->contextObject();

		return scope.Close(QObjectWrap::NewInstance(obj));
	}

	Handle<Value> QmlContextWrap::contextProperty(const Arguments& args)
	{
		HandleScope scope;
		
		QmlContextWrap *obj_wrap = QmlContextWrap::Unwrap<QmlContextWrap>(args.This());
		QQmlContext *obj = obj_wrap->GetObject();

		String::Utf8Value name(args[0]->ToString());

		QVariant v = obj->contextProperty(QString(*name));

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

		return Undefined();
	}

}
