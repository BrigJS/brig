#include <QObject>
#include "signal_handler.h"

namespace Brig {

	SignalHandler::SignalHandler(QObject *_obj)
	{
		obj = _obj;
	}

	SignalHandler::~SignalHandler()
	{
		// Release all callbacks
		for (Callback *callback : callbacks) {
			delete callback;
		}

		callbacks.clear();
	}

	int SignalHandler::qt_metacall(QMetaObject::Call call, int id, void **arguments)
	{
		// Call default handles in QObject first
		id = QObject::qt_metacall(call, id, arguments);
		if (id == -1 || call != QMetaObject::InvokeMetaMethod)
			return id;

		Q_ASSERT(id < callbacks.count());

		// Invoke dynamic handlers
		callbacks[id]->invoke(arguments);

		return -1;
	}

	int SignalHandler::addCallback(Handle<Value> cb)
	{
		// Callback
		Callback *callback = new Callback();
		callback->handler = Persistent<Function>::New(Handle<Function>::Cast(cb));

//		callbacks.push_back();
	}
}
