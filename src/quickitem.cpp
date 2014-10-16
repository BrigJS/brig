#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "quickitem.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QuickItemWrap::constructor;

	QuickItemWrap::QuickItemWrap() : ObjectWrap()
	{
		obj = new QQuickItem();
	}

	QuickItemWrap::QuickItemWrap(Handle<Value> object) : ObjectWrap()
	{
		if (strcmp(*String::Utf8Value(object->ToObject()->GetConstructorName()), "QuickItem") == 0) {
			QuickItemWrap *wrap = ObjectWrap::Unwrap<QuickItemWrap>(object->ToObject());
			obj = wrap->GetObject();
		} else {
			QObjectWrap *wrap = ObjectWrap::Unwrap<QObjectWrap>(object->ToObject());
			obj = qobject_cast<QQuickItem *>(wrap->GetObject());
		}

		prototype_object = object;
	}

	QuickItemWrap::~QuickItemWrap()
	{
		delete obj;
	}

	void QuickItemWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QuickItem");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QuickItemWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "toObject", QuickItemWrap::toObject);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setParentItem", QuickItemWrap::setParentItem);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setVisible", QuickItemWrap::setVisible);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QuickItemWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QuickItemWrap *obj_wrap = new QuickItemWrap(args[0]);
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QuickItemWrap::NewInstance(QObject *object)
	{
		HandleScope scope;

		const unsigned argc = 1;
		Handle<Value> obj = QObjectWrap::NewInstance(object);
		Handle<Value> argv[argc] = { obj };
		Handle<Value> instance = constructor->NewInstance(argc, argv);

		return scope.Close(instance);
	}

	Handle<Value> QuickItemWrap::NewInstance(Handle<Value> object)
	{
		HandleScope scope;

		const unsigned argc = 1;
		Handle<Value> argv[argc] = { object };
		Handle<Value> instance = constructor->NewInstance(argc, argv);

		return scope.Close(instance);
	}

	Handle<Value> QuickItemWrap::toObject(const Arguments& args)
	{
		HandleScope scope;
		
		QuickItemWrap *obj_wrap = QuickItemWrap::Unwrap<QuickItemWrap>(args.This());
		QObject *obj = qobject_cast<QObject *>(obj_wrap->GetObject());

		return scope.Close(QObjectWrap::NewInstance(obj));
	}

	Handle<Value> QuickItemWrap::setParentItem(const Arguments& args)
	{
		HandleScope scope;

		QuickItemWrap *obj_wrap = ObjectWrap::Unwrap<QuickItemWrap>(args.This());
		QObjectWrap *wrap = ObjectWrap::Unwrap<QObjectWrap>(args[0]->ToObject());

		// Getting content item if it is window object
		QQuickItem *parentItem = NULL;
		if (wrap->GetObject()->isWindowType()) {
			parentItem = qobject_cast<QQuickWindow *>(wrap->GetObject())->contentItem();
		} else {
			parentItem = qobject_cast<QQuickItem *>(wrap->GetObject());
		}

		QQuickItem *item = obj_wrap->GetObject();
		item->setParentItem(parentItem);

		return scope.Close(Undefined());
	}

	Handle<Value> QuickItemWrap::setVisible(const Arguments& args)
	{
		HandleScope scope;

		QuickItemWrap *obj_wrap = ObjectWrap::Unwrap<QuickItemWrap>(args.This());

		QQuickItem *item = obj_wrap->GetObject();
		item->setVisible(args[0]->ToBoolean()->Value());

		return scope.Close(Undefined());
	}
}
