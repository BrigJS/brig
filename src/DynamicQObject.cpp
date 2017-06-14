#include "DynamicQObject.h"

namespace Brig {

	DynamicQObject::DynamicQObject(QmlTypeBuilder *_typeBuilder, QMetaObject *metaObject, QObject *parent) : QObject(parent)
	{
		setParent(parent);
//		obj = NULL;
		typeBuilder = _typeBuilder;
		_builder = typeBuilder->metaObjectBuilder();
		_metaObject = metaObject;

//		qDebug() << "DynamicQObject INIT";

		//QDynamicMetaObjectData *dynamicMetaObjectData = new QDynamicMetaObjectData;
		//d_ptr->metaObject = reinterpret_cast<QDynamicMetaObjectData *>(metaObject);
#if 0
		printf("====================== INIT\n");
		printf("original metaObject\n");
		printAllMeta(QObject::metaObject());
#endif

	}

	DynamicQObject::~DynamicQObject()
	{
		// Disconnect all
		QMetaObject::disconnect(this, 0, 0, 0);
#if 0
		// Release all callbacks
		for (Callback *callback : callbacks) {
			delete callback;
		}

		callbacks.clear();
#endif
//		delete _builder;
//		delete _metaObject;
	}

	int DynamicQObject::qt_metacall(QMetaObject::Call call, int id, void **arguments)
	{
		// TODO: support signals which is emitted by original meta object
#if 0
		printf("============================================================ qt_metacall\n");
		printf("ORIGINAL qt_metacall: call=%d, id=%d\n", call, id);
		printf("original metaObject\n");
		printAllMeta(QObject::metaObject());
		printf("customized metaObject\n");
		//printAllMeta(DynamicQObject::metaObject());
		printAllMeta(_metaObject);
#endif

#if 1
		int idx = id;
//		if (QObject::metaObject() != DynamicQObject::metaObject()) {
			idx = QObject::qt_metacall(call, id, arguments);
			if (idx == -1)
				return -1;
//		}
#endif
#if 0
		printf("AFTER qt_metacall: call=%d, id=%d\n", call, idx);
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
//		const QMetaObject *meta = DynamicQObject::metaObject();
		const QMetaObject *meta = _metaObject;

		switch(call) {
			case QMetaObject::ReadProperty:
			{
				QVector<BrigMetaProperty *> _properties = _builder->getProperties();
				Q_ASSERT(idx < _properties.count());
				BrigMetaProperty *property = _properties[idx];
				Nan::Callback *handler = property->readHandler;

				// Prepare arguments
				Nan::HandleScope scope;
				int argc = 2;
				Local<Value> argv[] = {
					Nan::New<Number>(getId()),
					Nan::New<String>(property->name).ToLocalChecked()
				};

				// Invoke
				Handle<Value> ret = handler->Call(argc, argv);

				QVariant value;
				value.setValue(Utils::V8ToQVariant(ret));
				*reinterpret_cast<QVariant *>(arguments[0]) = value;

				break;
			}
			case QMetaObject::WriteProperty:
			{
				QVector<BrigMetaProperty *> _properties = _builder->getProperties();
				Q_ASSERT(idx < _properties.count());
				BrigMetaProperty *property = _properties[idx];
				Nan::Callback *handler = property->writeHandler;

				// Prepare arguments
				Nan::HandleScope scope;
				int argc = 3;
				QVariant *qvar = reinterpret_cast<QVariant *>(arguments[0]);
				Local<Value> argv[] = {
					Nan::New<Number>(getId()),
					Nan::New<String>(property->name).ToLocalChecked(),
					Utils::QDataToV8((int)qvar->type(), arguments[0])
				};

				// Invoke
				handler->Call(argc, argv);

				// Prepare for signal
				QVector<BrigMetaSignal *> _signals = _builder->getSignals();
				Q_ASSERT(idx < _signals.count());
				BrigMetaSignal *signal = _signals[property->signalId];

				// Prepare arguments for signal
				Local<Value> signal_argv[] = {
					Nan::New<Number>(getId()),
					Nan::New<String>(signal->name).ToLocalChecked()
				};

				// Emit signal
				_builder->getSignalListener()->Call(2, signal_argv);
				QMetaObject::activate(this, _metaObject, property->signalId, 0);

				break;
			}
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
					paramCount = _signals[idx]->arguments.count();

					// Convert parameters
					Nan::HandleScope scope;

					int argc = paramCount + 2;
					Handle<Value> *argv = new Handle<Value>[argc];
					argv[0] = Nan::New<Number>(getId());
					argv[1] = Nan::New<String>(_signals[idx]->name).ToLocalChecked();
					for (int i = 1; i <= paramCount; i++) {
						QVariant *qvar = reinterpret_cast<QVariant *>(arguments[i]);
						argv[i + 1] = Utils::QDataToV8((int)qvar->type(), arguments[i]);
					}

					// Invoke
					_builder->getSignalListener()->Call(argc, argv);

					// Release
					delete [] argv;

					// Rely the signal to QML
					QMetaObject::activate(this, _metaObject, idx, arguments);

				} else {
					int methodIdx = idx - _signals.count();
					Q_ASSERT(methodIdx < _methods.count());
					handler = _methods[methodIdx]->handler;
					paramCount = _methods[methodIdx]->arguments.count();

					// Convert parameters
					Nan::HandleScope scope;

					int argc = paramCount + 2;
					Handle<Value> *argv = new Handle<Value>[argc];
					argv[0] = Nan::New<Number>(getId());
					argv[1] = Nan::New<String>(_methods[methodIdx]->name).ToLocalChecked();
					for (int i = 1; i <= paramCount; i++) {
						QVariant *qvar = reinterpret_cast<QVariant *>(arguments[i]);
						argv[i + 1] = Utils::QDataToV8((int)qvar->type(), arguments[i]);
					}

					// Invoke
					Handle<Value> ret = handler->Call(argc, argv);

					// Return value
					QVariant value;
					value.setValue(Utils::V8ToQVariant(ret));
					*reinterpret_cast<QVariant *>(arguments[0]) = value;

					// Release
					delete [] argv;

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

	void DynamicQObject::emitSignal()
	{
		printf("EMITTTTTTTTT SIGGGGGGGGGGGGGGG\n");
	}

	bool DynamicQObject::invokeMethod(const char *member,
					Qt::ConnectionType type,
					QGenericReturnArgument ret,
					QGenericArgument val0,
					QGenericArgument val1,
					QGenericArgument val2,
					QGenericArgument val3,
					QGenericArgument val4,
					QGenericArgument val5,
					QGenericArgument val6,
					QGenericArgument val7,
					QGenericArgument val8,
					QGenericArgument val9)
	{
		QVarLengthArray<char, 512> sig;

		int len = qstrlen(member);
		if (len <= 0)
			return false;

		sig.append(member, len);
		sig.append('(');

		const char *typeNames[] = {
			ret.name(),
			val0.name(),
			val1.name(),
			val2.name(),
			val3.name(),
			val4.name(),
			val5.name(),
			val6.name(),
			val7.name(),
			val8.name(),
			val9.name()
		};

		// Preparing parameters
		int paramCount;
		for (paramCount = 1; paramCount < 10; ++paramCount) {
			len = qstrlen(typeNames[paramCount]);
			if (len <= 0)
				break;

			sig.append(typeNames[paramCount], len);
			sig.append(',');
		}

		if (paramCount == 1) {
			sig.append(')'); // no parameters
		} else {
			sig[sig.size() - 1] = ')';
		}
		sig.append('\0');

		const QMetaObject *metaObj = DynamicQObject::metaObject();

		// Finding method
		int idx = metaObj->indexOfMethod(sig.constData());
		if (idx < 0) {
			QByteArray norm = QMetaObject::normalizedSignature(sig.constData());
			idx = metaObj->indexOfMethod(norm.constData());
		}

		if (idx < 0 || idx >= metaObject()->methodCount()) {

			metaObj = QObject::metaObject();

			// Finding method from QML
			idx = metaObj->indexOfMethod(sig.constData());
			if (idx < 0) {
				QByteArray norm = QMetaObject::normalizedSignature(sig.constData());
				idx = metaObj->indexOfMethod(norm.constData());
			}

			if (idx < 0 || idx >= metaObj->methodCount()) {
				return false;
			}
		}

		// Getting method to invoke
		QMetaMethod method = metaObj->method(idx);

		return method.invoke(this, type, ret,
		                     val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
	}
}
