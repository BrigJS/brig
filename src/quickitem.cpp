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
		//NODE_SET_PROTOTYPE_METHOD(tpl, "show", QuickItemWrap::show);

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
#if 0
	Handle<Value> QuickItemWrap::show(const Arguments& args)
	{
		HandleScope scope;

		QuickItemWrap *obj_wrap = ObjectWrap::Unwrap<QuickItemWrap>(args.This());

		QQuickItem *view = obj_wrap->GetObject();
		view->setFormat(view->requestedFormat());
		view->show();

		return scope.Close(Undefined());
	}
#endif
}
