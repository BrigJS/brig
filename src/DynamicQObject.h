#ifndef BRIG_DYNAMIC_QOBJECT_H
#define BRIG_DYNAMIC_QOBJECT_H

#include <node.h>
#include <QObject>
#include <internal/qmetaobjectbuilder_p.h>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class DynamicQObject : public QObject {

		public:
			DynamicQObject(QmlTypeBuilder *_typeBuilder, QMetaObject *metaObject, QObject *parent);
			~DynamicQObject(void);

			void setId(int _id) { id = _id; };
			int getId() const { return id; };

			virtual int qt_metacall(QMetaObject::Call call, int id, void **arguments);
			virtual int findSignalId(const char *signal);
			virtual bool setObject(QObject *_obj);
			virtual int addCallback(const char *signal, Handle<Value> cb);
			virtual bool emitSignal(const char *member,
					Qt::ConnectionType type,
					QGenericArgument val0 = QGenericArgument( Q_NULLPTR ),
					QGenericArgument val1 = QGenericArgument(),
					QGenericArgument val2 = QGenericArgument(),
					QGenericArgument val3 = QGenericArgument(),
					QGenericArgument val4 = QGenericArgument(),
					QGenericArgument val5 = QGenericArgument(),
					QGenericArgument val6 = QGenericArgument(),
					QGenericArgument val7 = QGenericArgument(),
					QGenericArgument val8 = QGenericArgument(),
					QGenericArgument val9 = QGenericArgument());
			virtual bool invokeMethod(const char *member,
					Qt::ConnectionType type,
					QGenericReturnArgument ret,
					QGenericArgument val0 = QGenericArgument( Q_NULLPTR ),
					QGenericArgument val1 = QGenericArgument(),
					QGenericArgument val2 = QGenericArgument(),
					QGenericArgument val3 = QGenericArgument(),
					QGenericArgument val4 = QGenericArgument(),
					QGenericArgument val5 = QGenericArgument(),
					QGenericArgument val6 = QGenericArgument(),
					QGenericArgument val7 = QGenericArgument(),
					QGenericArgument val8 = QGenericArgument(),
					QGenericArgument val9 = QGenericArgument());

		private:

			int id;
			QMetaObject *_metaObject;
			DynamicQMetaObjectBuilder *_builder;
			QmlTypeBuilder *typeBuilder;

		protected:

			QVector<Callback *> _methods;
			QVector<Callback *> _signals;
	};

}

#endif
