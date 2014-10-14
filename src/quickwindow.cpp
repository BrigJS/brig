#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "quickwindow.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QuickWindowWrap::constructor;

	QuickWindowWrap::QuickWindowWrap() : ObjectWrap()
	{
		obj = new QQuickWindow();
	}

	QuickWindowWrap::QuickWindowWrap(Handle<Value> object) : ObjectWrap()
	{
		if (strcmp(*String::Utf8Value(object->ToObject()->GetConstructorName()), "QuickWindow") == 0) {
			QuickWindowWrap *wrap = ObjectWrap::Unwrap<QuickWindowWrap>(object->ToObject());
			obj = wrap->GetObject();
		} else {
			QObjectWrap *wrap = ObjectWrap::Unwrap<QObjectWrap>(object->ToObject());
			obj = qobject_cast<QQuickWindow *>(wrap->GetObject());
		}

		prototype_object = object;
	}

	QuickWindowWrap::~QuickWindowWrap()
	{
		delete obj;
	}

	void QuickWindowWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QuickWindow");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QuickWindowWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "show", QuickWindowWrap::show);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QuickWindowWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QuickWindowWrap *obj_wrap = new QuickWindowWrap(args[0]);
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QuickWindowWrap::NewInstance(QObject *object)
	{
		HandleScope scope;

		const unsigned argc = 1;
		Handle<Value> obj = QObjectWrap::NewInstance(object);
		Handle<Value> argv[argc] = { obj };
		Handle<Value> instance = constructor->NewInstance(argc, argv);

		return scope.Close(instance);
	}

	Handle<Value> QuickWindowWrap::NewInstance(Handle<Value> object)
	{
		HandleScope scope;

		const unsigned argc = 1;
		Handle<Value> argv[argc] = { object };
		Handle<Value> instance = constructor->NewInstance(argc, argv);

		return scope.Close(instance);
	}

	Handle<Value> QuickWindowWrap::show(const Arguments& args)
	{
		HandleScope scope;

		QuickWindowWrap *obj_wrap = ObjectWrap::Unwrap<QuickWindowWrap>(args.This());

		QQuickWindow *view = obj_wrap->GetObject();
		view->setFormat(view->requestedFormat());
		view->show();

		return scope.Close(Undefined());
	}
}
