#include <QObject>
#include "DynamicQObject.h"

namespace Brig {
/*
	DynamicQObject::DynamicQObject(QMetaObject *metaobject) : _metaObject(NULL)
	{
		obj = NULL;
		_builder = new QMetaObjectBuilder();
		if (metaobject)
			_metaObject = metaobject;
	}
*/


	DynamicQObject::DynamicQObject(DynamicQMetaObjectBuilder *dynamicMetaObjectBuilder, QMetaObject *metaObject, QObject *parent)
	{
		setParent(parent);
		obj = NULL;
		_builder = dynamicMetaObjectBuilder;
		_metaObject = metaObject;
//		_builder = new QMetaObjectBuilder();
printf("======================================= cooool\n");
	}

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
		free(_metaObject);
	}

	int DynamicQObject::printAllMeta(const QMetaObject *meta)
	{

		// Properties
		for (int i = meta->propertyOffset(); i < meta->propertyCount(); ++i) {
			QMetaProperty property = meta->property(i);
			const char *propName = property.name();
printf("=== property name: %s\n", propName);
		}

		// Properties
		for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
			QMetaMethod method = meta->method(i);
			const char *methodName = method.name();
printf("=== method name: %s %s %d\n", methodName, method.methodSignature().data(), method.returnType());
/*
			for (QByteArray x : method.parameterNames()) {
				printf("=> %s\n", x.data());
			}
*/
		}
	}

	int DynamicQObject::qt_metacall(QMetaObject::Call call, int id, void **arguments)
	{
//printf("ORIGINAL qt_metacall: call=%d, id=%d\n", call, id);

		int idx = QObject::qt_metacall(call, id, arguments);
		if (idx == -1)
			return -1;
#if 0
		printAllMeta(_metaObject);
		printAllMeta(QObject::metaObject());
#endif

#if 0
		printAllMeta(_metaObject);
		printAllMeta(QObject::metaObject());

printf("qt_metacall: id=%d,\n\t_mo.methodOffset=%d,\n\tdqo.methodOffset=%d,\n\tqo.methodOffset=%d\n",
		idx,
		_metaObject->methodOffset(),
		DynamicQObject::metaObject()->methodOffset(),
		QObject::metaObject()->methodOffset());
printf("qt_metacall: _mo.propertyOffset=%d, dqo.propertyOffset=%d, qo.propertyOffset=%d\n",
		_metaObject->propertyOffset(),
		DynamicQObject::metaObject()->propertyOffset(),
		QObject::metaObject()->propertyOffset());
printf("after QObject::qt_metacall id=%x\n", idx);
#endif
		const QMetaObject *meta = DynamicQObject::metaObject();

		switch(call) {
			case QMetaObject::InvokeMetaMethod:
			{
#if 0
//printf("qt_metacall: InvokeMetaMethod id = %d\n", idx);
				QVector<Callback *> callbacks = _builder->getSignals();

//	printf("qt_metacall: idx = %d, count() = %d\n", idx, callbacks.count());
				Q_ASSERT(idx < callbacks.count());
				Callback *callback = callbacks[idx];

				//int methodId = findSignalId(callback->signature);
				int methodId = meta->indexOfMethod(callback->signature);
#endif
				int methodId = meta->methodOffset() + idx;

				QMetaMethod qmethod = meta->method(methodId);

				// Getting callback function
				QVector<BrigMetaSignal *> _signals = _builder->getSignals();
				QVector<BrigMetaMethod *> _methods = _builder->getMethods();
				Nan::Callback *handler = NULL;
				int paramCount = 0;

				if (qmethod.methodType() == QMetaMethod::Signal) {
					Q_ASSERT(idx < _signals.count());
					handler = _signals[idx]->handler;
					paramCount = _signals[idx]->arguments.count();
				} else {
					int methodIdx = idx - _signals.count();
					Q_ASSERT(methodIdx < _methods.count());
					handler = _methods[methodIdx]->handler;
					paramCount = _methods[methodIdx]->arguments.count();
				}

				// Convert parameters
				Nan::HandleScope scope;

				int argc = paramCount;
				Handle<Value> *argv = new Handle<Value>[argc];
				for (int i = 0; i < argc; i++) {
					QVariant *qvar = reinterpret_cast<QVariant *>(arguments[i + 1]);
					argv[i] = Utils::QDataToV8((int)qvar->type(), arguments[i + 1]);
				}

				// Invoke
				Handle<Value> ret = handler->Call(argc, argv);

				// Release
				delete [] argv;

				if (qmethod.methodType() == QMetaMethod::Signal) {
					// Rely the signal to QML
					QMetaObject::activate(this, _metaObject, idx, arguments);
				} else {
					QVariant value;
					value.setValue(Utils::V8ToQVariant(ret));
					*reinterpret_cast<QVariant *>(arguments[0]) = value;
				}

				break;
			}
		}

		return -1;
	}

	int DynamicQObject::findSignalId(const char *signal)
	{
		//const QMetaObject *meta = obj->metaObject();
		const QMetaObject *meta = _metaObject;

		// Finding signal id
		for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
			QMetaMethod method = meta->method(i);
			const char *methodName = method.name().data();
//printf("=== method name: %s\n", methodName);
			if (strcmp(signal, methodName) == 0)
				return i;
		}

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
