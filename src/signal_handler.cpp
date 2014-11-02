#include <QObject>
#include "signal_handler.h"

namespace Brig {

	SignalHandler::SignalHandler()
	{
		obj = NULL;
	}

	SignalHandler::SignalHandler(QObject *_obj)
	{
		obj = _obj;
	}

	SignalHandler::~SignalHandler()
	{
		// Disconnect all
		QMetaObject::disconnect(obj, 0, 0, 0);

		// Release all callbacks
		for (Callback *callback : callbacks) {
			delete callback;
		}

		callbacks.clear();
	}

	int SignalHandler::qt_metacall(QMetaObject::Call call, int id, void **arguments)
	{
		// Call default handlers in QObject first
		id = QObject::qt_metacall(call, id, arguments);
		if (id == -1 || call != QMetaObject::InvokeMetaMethod)
			return id;

		Q_ASSERT(id < callbacks.count());

		Callback *callback = callbacks[id];
		int methodId = findSignalId(callback->signal);

		// Convert parameters
		HandleScope scope;

		static const QMetaObject *meta = obj->metaObject();
		QMetaMethod method = meta->method(methodId);
		int argc = method.parameterCount();

		Handle<Value> argv[argc];
		for (int i = 0; i < method.parameterCount(); ++i) {
			int type = method.parameterType(i);

			// Convert
			argv[i] = Utils::QDataToV8(type, arguments[i + 1]);
		}

		// Invoke
		MakeCallback(callback->handler, callback->handler, argc, argv);

		return -1;
	}

	int SignalHandler::findSignalId(const char *signal)
	{
		static const QMetaObject *meta = obj->metaObject();

		// Finding signal id
		for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
			QMetaMethod method = meta->method(i);
			const char *methodName = method.name().data();

			if (strcmp(signal, methodName) == 0)
				return i;
		}

		return -1;
	}

	bool SignalHandler::setObject(QObject *_obj)
	{
		// It's already set up
		if (obj != NULL)
			return false;

		obj = _obj;

		// Apply all callbacks if callbacks were already set up
		for (int i = 0; i < callbacks.count(); ++i) {
			int id = findSignalId(callbacks[i]->signal);
			if (id == -1)
				continue;

			QMetaObject::connect(obj, id, this, id + QObject::metaObject()->methodCount());
		}

		return true;
	}

	int SignalHandler::addCallback(const char *signal, Handle<Value> cb)
	{
		HandleScope scope;

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
	}
}
