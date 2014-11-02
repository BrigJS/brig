#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "QuickItem.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QuickItem::constructor;

	QuickItem::QuickItem() : ObjectWrap()
	{
		obj = NULL;
	}

	QuickItem::~QuickItem()
	{
		delete obj;
	}

	void QuickItem::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QuickItem");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QuickItem::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "create", QuickItem::create);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getPropertyNames", QuickItem::getPropertyNames);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getProperty", QuickItem::getProperty);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setProperty", QuickItem::setProperty);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	// Prototype Constructor
	Handle<Value> QuickItem::New(const Arguments& args)
	{
		HandleScope scope;

		QuickItem *obj_wrap = new QuickItem();
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	// Method
	Handle<Value> QuickItem::create(const Arguments& args)
	{
		HandleScope scope;

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(args.This());

		QmlComponent *component = ObjectWrap::Unwrap<QmlComponent>(args[0]->ToObject());

		// Create QuickItem with component
		obj_wrap->obj = static_cast<QQuickItem *>(component->GetObject()->create());

		return Undefined();
	}

	Handle<Value> QuickItem::getPropertyNames(const Arguments& args)
	{
		HandleScope scope;

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(args.This());

		Handle<Array> keys = Array::New();

		// Getting property names
		static const QMetaObject *meta = obj_wrap->GetObject()->metaObject();
		for (int i = 0; i < meta->propertyCount(); i++) {
			keys->Set(i, String::New(QString(meta->property(i).name()).toUtf8().constData()));
		}

		return scope.Close(keys);
	}

	Handle<Value> QuickItem::getProperty(const Arguments& args)
	{
		HandleScope scope;

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(args.This());

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		String::Utf8Value name(args[0]->ToString());

		// Get property
		QVariant v = obj_wrap->GetObject()->property(*name);

		// Convert Qvariant to V8 data type
		if (v.isNull())
			return scope.Close(Null());

		return scope.Close(Utils::QVariantToV8(v.userType(), v));
	}

	Handle<Value> QuickItem::setProperty(const Arguments& args)
	{
		HandleScope scope;

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(args.This());

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
}
