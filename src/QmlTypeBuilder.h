#ifndef BRIG_QMLTYPEBUILDER_H
#define BRIG_QMLTYPEBUILDER_H

#include <node.h>
#include <qqmlprivate.h>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QmlTypeBuilder : public ObjectWrap {

		public:
			QmlTypeBuilder(const char *typeName);
			~QmlTypeBuilder();

			static NAN_MODULE_INIT(Initialize);

		private:

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(build);
			static NAN_METHOD(addMethod);
			static NAN_METHOD(addSignal);

			QObject *obj;
			DynamicQMetaObjectBuilder *metaobject_builder;
			QMap<unsigned long, QObject *> qml_types;
			static QQmlPrivate::RegisterType types[2];

			static Nan::Persistent<Function> constructor;
	};

}

#endif
