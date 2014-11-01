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
		static const QMetaObject *meta = obj->metaObject();
		QMetaMethod method = meta->method(id);

printf("EVENT %s %d %d\n", method.methodSignature().data(), QObject::metaObject()->methodCount(), id);
		// Call default handles in QObject first
		id = QObject::qt_metacall(call, id, arguments);
		if (id == -1 || call != QMetaObject::InvokeMetaMethod)
			return id;

		Q_ASSERT(id - 1 < callbacks.count());

		// Convert parameters
		HandleScope scope;
		int argc = method.parameterCount();
		Handle<Value> argv[argc];
		for (int i = 0; i < method.parameterCount(); ++i) {
			int type = method.parameterType(i);

			// Convert
			argv[i] = Utils::QDataToV8(type, arguments[i + 1]);
		}

		// Invoke dynamic handlers
printf("PARAMS NUM %d\n", method.parameterCount());
		Callback *callback = callbacks[id - 1];
		MakeCallback(callback->handler, callback->handler, method.parameterCount(), argv);

		return -1;
	}

	int SignalHandler::findSignalId(const char *signal)
	{
		static const QMetaObject *meta = obj->metaObject();

		// Finding signal id
		for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
			QMetaMethod method = meta->method(i);
			const char *methodName = method.name().data();
			if (strcmp(signal, methodName) != 0)
				continue;

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

		// Apply all callbacks
		for (int i = 0; i < callbacks.count(); ++i) {
			int id = findSignalId(callbacks[i]->signal);
			if (id == -1)
				continue;

printf("setObject %d\n", i + 1 + QObject::metaObject()->methodCount());
			QMetaObject::connect(obj, id, this, i + 1 + QObject::metaObject()->methodCount());
		}

		return true;
	}

	int SignalHandler::addCallback(const char *signal, Handle<Value> cb)
	{
		// Callback
		Callback *callback = new Callback();
		callback->signal = strdup(signal);
		callback->handler = Persistent<Function>::New(Handle<Function>::Cast(cb));
		callbacks.push_back(callback);

		// No object can be hooked yet
		if (obj == NULL) {
			return callbacks.count() - 1;
		}

		// Connect to signal
		int id = findSignalId(callback->signal);
		if (id == -1)
			return -1;

		QMetaObject::connect(obj, id, this, callbacks.count() + QObject::metaObject()->methodCount());

		return id;
	}
}
