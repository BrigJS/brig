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
			DynamicQObject(DynamicQMetaObjectBuilder *dynamicMetaObjectBuilder, QMetaObject *metaObject, QObject *parent);
			~DynamicQObject(void);

			QObject *GetObject() const { return obj; };

			const QMetaObject *metaObject() const { return _metaObject; };
			virtual int printAllMeta(const QMetaObject *meta);
			virtual int qt_metacall(QMetaObject::Call call, int id, void **arguments);
			virtual int findSignalId(const char *signal);
			virtual bool setObject(QObject *_obj);
			virtual int addCallback(const char *signal, Handle<Value> cb);

		private:

			QObject *obj;
			QMetaObject *_metaObject;
			DynamicQMetaObjectBuilder *_builder;

		protected:

			QVector<Callback *> _methods;
			QVector<Callback *> _signals;
	};

}

#endif
