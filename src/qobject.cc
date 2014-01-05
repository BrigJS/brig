#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "qobject.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QObjectWrap::constructor;

	QObjectWrap::QObjectWrap() : ObjectWrap()
	{
	}

	QObjectWrap::QObjectWrap(QObject *object) : ObjectWrap()
	{
		obj = object;
	}

	QObjectWrap::~QObjectWrap()
	{
		delete obj;
	}

	void QObjectWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QObject");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QObjectWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
//		NODE_SET_PROTOTYPE_METHOD(tpl, "show", QObjectWrap::show);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QObjectWrap::New(const Arguments& args)
	{
		HandleScope scope;

		if (args[0]->ToObject()->InternalFieldCount() >= 2) {

			BrigContainerType type = static_cast<BrigContainerType>(args[0]->ToObject()->GetInternalField(1)->ToInteger()->Value());

			if (type == BRIG_CONTAINER_NATIVE) {
				QObject *object = ObjectWrap::Unwrap<QObject>(args[0]->ToObject());
				QObjectWrap *obj_wrap = new QObjectWrap(object);
				obj_wrap->Wrap(args.This());
				return args.This();
			}
		}

		QObjectWrap *obj_wrap = new QObjectWrap();
		obj_wrap->Wrap(args.This());

		return scope.Close(args[0]);
	}

	Handle<Value> QObjectWrap::NewInstance(QObject *qobject)
	{
		HandleScope scope;

		// Create a container
		Handle<ObjectTemplate> object_template = ObjectTemplate::New();
		object_template->SetInternalFieldCount(2);
		Persistent<ObjectTemplate> object_instance = Persistent<ObjectTemplate>::New(object_template);
		Local<Object> obj = object_instance->NewInstance();

		// Wrap
		obj->SetInternalField(0, External::New(qobject));
		obj->SetInternalField(1, Integer::New(BRIG_CONTAINER_NATIVE));

		const unsigned argc = 1;
		Handle<Value> argv[argc] = { obj };

		Local<Object> instance = constructor->NewInstance(argc, argv);

		return scope.Close(instance);
	}

#if 0
	Handle<Value> QObjectWrap::show(const Arguments& args)
	{
		HandleScope scope;

		QObjectWrap *obj_wrap = ObjectWrap::Unwrap<QObjectWrap>(args.This());

		QObject *view = obj_wrap->GetObject();
		view->show();

		return scope.Close(Undefined());
	}
#endif
}
