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
	}

	QmlContextWrap::QmlContextWrap(QQmlContext *context) : ObjectWrap()
	{
		obj = new QQmlContext(context);
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

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QmlContextWrap::New(const Arguments& args)
	{
		HandleScope scope;

		if (args[0]->ToObject()->InternalFieldCount() >= 2) {
			BrigContainerType type = static_cast<BrigContainerType>(args[0]->ToObject()->GetInternalField(1)->ToInteger()->Value());

			if (type == BRIG_CONTAINER_NATIVE) {
				QQmlContext *context = ObjectWrap::Unwrap<QQmlContext>(args[0]->ToObject());
				QmlContextWrap *obj_wrap = new QmlContextWrap(context);
				obj_wrap->Wrap(args.This());
			}
		} else {
			QmlContextWrap *context_wrap = ObjectWrap::Unwrap<QmlContextWrap>(args[0]->ToObject());
			QmlContextWrap *obj_wrap = new QmlContextWrap(context_wrap);
			obj_wrap->Wrap(args.This());
		}

		return args.This();
	}

	Handle<Value> QmlContextWrap::NewInstance(QQmlContext *context)
	{
		HandleScope scope;

		Handle<ObjectTemplate> object_template = ObjectTemplate::New();
		object_template->SetInternalFieldCount(2);
		Persistent<ObjectTemplate> object_instance = Persistent<ObjectTemplate>::New(object_template);
		Local<Object> obj = object_instance->NewInstance();
		obj->SetInternalField(0, External::New(context));
		obj->SetInternalField(1, Integer::New(BRIG_CONTAINER_NATIVE));

		const unsigned argc = 1;
		Handle<Value> argv[argc] = { obj };

		Local<Object> instance = constructor->NewInstance(argc, argv);

		return scope.Close(instance);
	}
}
