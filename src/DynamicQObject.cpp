#include <QObject>
#include "DynamicQObject.h"

namespace Brig {

	DynamicQObject::DynamicQObject(QMetaObject *metaobject) : _metaObject(NULL)
	{
		obj = NULL;
		_builder = new QMetaObjectBuilder();
		_metaObject = metaobject;
	}
/*
	DynamicQObject::DynamicQObject(QObject *_obj)
	{
		obj = _obj;
		_builder = new QMetaObjectBuilder();
	}
*/
	DynamicQObject::~DynamicQObject()
	{
		// Disconnect all
		QMetaObject::disconnect(obj, 0, 0, 0);
#if 0
		// Release all callbacks
		for (Callback *callback : callbacks) {
			delete callback;
		}

		callbacks.clear();
#endif
		delete _builder;
		delete _metaObject;
	}


	int DynamicQObject::qt_metacall(QMetaObject::Call call, int id, void **arguments)
	{
printf("%d\n", call);
		// Call default handlers in QObject first
		id = QObject::qt_metacall(call, id, arguments);
		if (id == -1 || call != QMetaObject::InvokeMetaMethod)
			return id;

#if 0
		Q_ASSERT(id < callbacks.count());
		Callback *callback = callbacks[id];

		int methodId = findSignalId(callback->signal);

		// Convert parameters
		HandleScope scope;

		const QMetaObject *meta = obj->metaObject();
		QMetaMethod method = meta->method(methodId);
		int argc = method.parameterCount();

		Handle<Value> *argv = new Handle<Value>[argc];
		for (int i = 0; i < method.parameterCount(); ++i) {
			int type = method.parameterType(i);

			// Convert
			argv[i] = Utils::QDataToV8(type, arguments[i + 1]);
		}

		// Invoke
		MakeCallback(callback->handler, callback->handler, argc, argv);

		// Release
		delete [] argv;
#endif
		return -1;
	}

	int DynamicQObject::findSignalId(const char *signal)
	{
#if 0
		const QMetaObject *meta = obj->metaObject();

		// Finding signal id
		for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
			QMetaMethod method = meta->method(i);
			const char *methodName = method.name().data();

			if (strcmp(signal, methodName) == 0)
				return i;
		}
#endif
		return -1;
	}

	bool DynamicQObject::setObject(QObject *_obj)
	{
#if 0
		// It's already set up
		if (obj != NULL)
			return false;

		this->obj = _obj;

		// Apply all callbacks if callbacks were already set up
		for (int i = 0; i < callbacks.count(); ++i) {
			int id = findSignalId(callbacks[i]->signal);
			if (id == -1)
				continue;

			QMetaObject::connect(obj, id, this, id + QObject::metaObject()->methodCount());
		}

#endif
		return true;
	}

	int DynamicQObject::addCallback(const char *signal, Handle<Value> cb)
	{
		NanScope();
#if 0
		int slotId = callbacks.count();

		// Create a new callback
		Callback *callback = new Callback();
		callback->signal = strdup(signal);
		callback->handler = Persistent<Function>::New(Handle<Function>::Cast(cb));
		callbacks.append(callback);

		// No object can be hooked yet
		if (obj == NULL)
			return slotId;

		// Connect to signal
		int signalId = findSignalId(callback->signal);
		if (signalId == -1)
			return -1;

		QMetaObject::connect(obj, signalId, this, slotId + QObject::metaObject()->methodCount());

		return signalId;
#endif
	}
}
