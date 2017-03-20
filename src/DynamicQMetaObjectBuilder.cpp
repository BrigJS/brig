#include <QObject>
#include "DynamicQMetaObjectBuilder.h"

namespace Brig {

	DynamicQMetaObjectBuilder::DynamicQMetaObjectBuilder(const char *typeName)
	{
		_typeName = strdup(typeName);
	}


	DynamicQMetaObjectBuilder::~DynamicQMetaObjectBuilder()
	{
		// Release all signals
		for (BrigMetaSignal *signal : _signals) {
			delete signal;
		}

		_signals.clear();

		// Release all methods
		for (BrigMetaMethod *method : _methods) {
			delete method;
		}

		_methods.clear();
	}

	QMetaObject *DynamicQMetaObjectBuilder::build()
	{
		// Initializing builder
		builder.setClassName(_typeName);
		builder.setSuperClass(&QObject::staticMetaObject);
		builder.setFlags(QMetaObjectBuilder::DynamicMetaObject);

		// Initializing signal hanglers
		for (int i = 0; i < _signals.count(); ++i) {
			BrigMetaSignal *signal = _signals[i];
			builder.addSignal(QMetaObject::normalizedSignature(signal->signature));
		}

		// Initializing method hanglers
		for (int i = 0; i < _methods.count(); ++i) {
			BrigMetaMethod *method = _methods[i];
			builder.addMethod(QMetaObject::normalizedSignature(method->signature), "QVariant");
		}

		return builder.toMetaObject();
	}

	void DynamicQMetaObjectBuilder::addSignal(const char *name, const char *signature, QList<QByteArray> arguments, Local<Value> cb)
	{
		Nan::HandleScope scope;

		BrigMetaSignal *signal = new BrigMetaSignal();
		signal->name = strdup(name);
		signal->signature = strdup(signature);
		signal->handler = new Nan::Callback(cb.As<Function>());
		signal->arguments = arguments;
		_signals.append(signal);
#if 0
		// Create a new callback
		Callback *callback = new Callback();
		callback->signature = strdup(name);
		callback->handler = new Nan::Callback(cb.As<Function>());
		_signals.append(callback);

		// Preparing arguments
		QList<QByteArray> arguments;
		for (auto argName : argumentNames) {
			arguments.push_back(argName.toLatin1());
		}

		// Add to signal builder
		builder->addSignal(signature).setParameterNames(arguments);
		printf("Added signal handler: %s\n", callback->signature);
#endif
	}

	void DynamicQMetaObjectBuilder::addMethod(const char *name, const char *signature, QList<QByteArray> arguments, Local<Value> cb)
	{
		Nan::HandleScope scope;

		BrigMetaMethod *method = new BrigMetaMethod();
		method->name = strdup(name);
		method->signature = strdup(signature);
		method->arguments = arguments;
		method->handler = new Nan::Callback(cb.As<Function>());
		_methods.append(method);
#if 0
		// Create a new callback
		Callback *callback = new Callback();
		callback->signature = strdup(name);
		callback->handler = new Nan::Callback(cb.As<Function>());
		_methods.append(callback);

		// Preparing arguments
		QList<QByteArray> arguments;
		for (auto argName : argumentNames) {
			arguments.push_back(argName.toLatin1());
		}

		// Add to method builder
		builder->addMethod(signature).setParameterNames(arguments);
		printf("Added method handler: %s\n", callback->signature);
#endif
	}
}
