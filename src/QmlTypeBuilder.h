#ifndef BRIG_QMLTYPEBUILDER_H
#define BRIG_QMLTYPEBUILDER_H

#include <node.h>
//#include <qqmlprivate.h>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	struct TypeInstance {

		int id;
		DynamicQObject *instance = NULL;

		~TypeInstance() {
			delete instance;
		}
	};

	class QmlTypeBuilder : public ObjectWrap {

		public:
			QmlTypeBuilder(const char *_typeName);
			~QmlTypeBuilder();

			DynamicQMetaObjectBuilder *metaObjectBuilder() const { return metaobject_builder; };
			void createInstance(DynamicQObject *instance);
			TypeInstance *findInstance(int _id);

			static NAN_MODULE_INIT(Initialize);

		private:

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(build);
			static NAN_METHOD(addMethod);
			static NAN_METHOD(addSignal);
			static NAN_METHOD(addProperty);
			static NAN_METHOD(addSignalListener);
			static NAN_METHOD(setListener);
			static NAN_METHOD(emitSignal);
			static NAN_METHOD(invokeMethod);

			QObject *obj;
			DynamicQMetaObjectBuilder *metaobject_builder;
			QMap<unsigned long, QObject *> qml_types;
			static QQmlPrivate::RegisterType types[2];

			const char *typeName;
			int instCounter = 0;
			QVector<TypeInstance *> instances;
			Nan::Callback *listener = NULL;

			static Nan::Persistent<Function> constructor;
	};

}

#endif
