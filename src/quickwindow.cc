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

		QuickWindowWrap *obj_wrap = new QuickWindowWrap();
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QuickWindowWrap::show(const Arguments& args)
	{
		HandleScope scope;

		QuickWindowWrap *obj_wrap = ObjectWrap::Unwrap<QuickWindowWrap>(args.This());

		QQuickWindow *view = obj_wrap->GetObject();
		view->show();

		return scope.Close(Undefined());
	}
}
