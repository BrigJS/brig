#include <node.h>
#include <QtGui>
#include <QObject>
#include "qapplication.h"
#include "eventloop.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QApplicationWrap::constructor;

	QApplicationWrap::QApplicationWrap() : ObjectWrap()
	{
		app_argc = 0;
		app_argv = NULL;
		app = new QGuiApplication(app_argc, app_argv);

		// Initializing event loop
		eventloop = new EventLoop(app);
	}

	QApplicationWrap::~QApplicationWrap()
	{
		delete app;
	}

	void QApplicationWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QApplication");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QApplicationWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "exec", QApplicationWrap::Exec);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QApplicationWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QApplicationWrap *app_wrap = new QApplicationWrap();
		app_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QApplicationWrap::Exec(const Arguments& args)
	{
		HandleScope scope;

		QApplicationWrap *app_wrap = ObjectWrap::Unwrap<QApplicationWrap>(args.This());
		app_wrap->GetEventLoop()->Main();

		return scope.Close(Undefined());
	}
}
