#ifndef BRIG_DYNAMIC_QMETAOBJECT_BUILDER_H
#define BRIG_DYNAMIC_QMETAOBJECT_BUILDER_H

#include <node.h>
#include <QObject>
#include <internal/qmetaobjectbuilder_p.h>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	struct BrigMetaMethod {
		char *name;
		char *signature;
		QList<QByteArray> arguments;
		Nan::Callback *handler;

		~BrigMetaMethod() {

			arguments.clear();

			delete name;
			delete handler;
			delete signature;
		}
	};

	struct BrigMetaSignal {
		char *name;
		char *signature;
		QList<QByteArray> arguments;
		Nan::Callback *handler;

		~BrigMetaSignal() {

			arguments.clear();

			delete name;
			delete handler;
			delete signature;
		}
	};

	struct BrigMetaProperty {
		char *name;
		char *signature;
		Nan::Callback *readHandler;
		Nan::Callback *writeHandler;

		~BrigMetaProperty() {

			delete name;
			delete signature;
			delete readHandler;
			delete writeHandler;
		}
	};

	class DynamicQMetaObjectBuilder {

		public:
			DynamicQMetaObjectBuilder(const char *typeName);
			~DynamicQMetaObjectBuilder(void);

			QMetaObject *build();
			char *getTypeName() { return _typeName; };
			QVector<BrigMetaSignal *> getSignals() { return _signals; };
			QVector<BrigMetaMethod *> getMethods() { return _methods; };
			QVector<BrigMetaProperty *> getProperties() { return _properties; };

			void addSignal(const char *name, const char *signature, QList<QByteArray> arguments, Local<Value> cb);
			void addMethod(const char *name, const char *signature, QList<QByteArray> arguments, Local<Value> cb);
			void addProperty(const char *name, Local<Value> readCallback, Local<Value> writeCallback);

		private:

			char *_typeName;
			QMetaObjectBuilder builder;
			QVector<BrigMetaMethod *> _methods;
			QVector<BrigMetaSignal *> _signals;
			QVector<BrigMetaProperty *> _properties;
	};

}

#endif
