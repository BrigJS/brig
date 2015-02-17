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
			DynamicQObject(QMetaObject *metaobject);
			~DynamicQObject(void);

			QObject *GetObject() const { return obj; };

			virtual int qt_metacall(QMetaObject::Call call, int id, void **arguments);
			virtual int findSignalId(const char *signal);
			virtual bool setObject(QObject *_obj);
			virtual int addCallback(const char *signal, Handle<Value> cb);

		private:

			QObject *obj;
			QMetaObject *_metaObject;
			QMetaObjectBuilder *_builder;

		protected:

			QVector<Callback *> _methods;
			QVector<Callback *> _signals;
	};

}

#endif
