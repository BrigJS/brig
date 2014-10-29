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
}
