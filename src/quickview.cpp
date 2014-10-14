#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "quickview.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QuickViewWrap::constructor;

	QuickViewWrap::QuickViewWrap() : ObjectWrap()
	{
		obj = new QQuickView();
	}

	QuickViewWrap::~QuickViewWrap()
	{
		delete obj;
	}

	void QuickViewWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QuickView");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QuickViewWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "show", QuickViewWrap::show);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setSource", QuickViewWrap::setSource);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QuickViewWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QuickViewWrap *obj_wrap = new QuickViewWrap();
		obj_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QuickViewWrap::show(const Arguments& args)
	{
		HandleScope scope;

		QuickViewWrap *obj_wrap = ObjectWrap::Unwrap<QuickViewWrap>(args.This());

		QQuickView *view = obj_wrap->GetObject();
		view->show();

		return scope.Close(Undefined());
	}

	Handle<Value> QuickViewWrap::setSource(const Arguments& args)
	{
		HandleScope scope;

		if (!args[0]->IsString())
			return ThrowException(Exception::Error(String::New("First argument must be a string")));

		String::Utf8Value filename_str(args[0]->ToString());
		QuickViewWrap *obj_wrap = ObjectWrap::Unwrap<QuickViewWrap>(args.This());

		QQuickView *view = obj_wrap->GetObject();
		view->setSource(QUrl::fromLocalFile(*filename_str));

		return scope.Close(Undefined());
	}
}
