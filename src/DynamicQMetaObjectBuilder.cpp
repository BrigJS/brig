#include <QObject>
#include "DynamicQMetaObjectBuilder.h"

namespace Brig {

	DynamicQMetaObjectBuilder::DynamicQMetaObjectBuilder(const char *typeName)
	{
		_typeName = strdup(typeName);

		builder = new QMetaObjectBuilder();
		builder->setClassName(_typeName);
		builder->setSuperClass(&QObject::staticMetaObject);

//		property_builder = new QMetaPropertyBuilder();
//		method_builder = new QMetaMethodBuilder();
	}


	DynamicQMetaObjectBuilder::~DynamicQMetaObjectBuilder()
	{
		delete method_builder;
		delete property_builder;
		delete builder;
	}

	QMetaObject *DynamicQMetaObjectBuilder::build()
	{
		return builder->toMetaObject();
	}

	void DynamicQMetaObjectBuilder::addSignal(const char *signature, QStringList argumentNames, Handle<Value> cb)
	{
		int slotId = _signals.count();

		// Create a new callback
		Callback *callback = new Callback();
		callback->signal = strdup(signature);
		callback->handler = new NanCallback(cb.As<Function>());
		//callback->handler = Persistent<Function>::New(Handle<Function>::Cast(cb));
		_signals.append(callback);

		// Preparing arguments
		QList<QByteArray> arguments;
		for (auto name : argumentNames) {
			arguments.push_back(name.toLatin1());
		}

		// Add to method builder
		builder->addSignal(signature).setParameterNames(arguments);
	}
}
