#ifndef BRIG_QMLTYPEBUILDER_H
#define BRIG_QMLTYPEBUILDER_H

#include <node.h>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QmlTypeBuilder : public ObjectWrap {

		public:
			QmlTypeBuilder(const char *typeName);
			~QmlTypeBuilder();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);

		private:

			static Handle<Value> New(const Arguments& args);

			/* Methods */
			static Handle<Value> build(const Arguments& args);
			static Handle<Value> addMethod(const Arguments& args);
			static Handle<Value> addSignal(const Arguments& args);

			QObject *obj;
			unsigned long counter;
			DynamicQMetaObjectBuilder *metaobject_builder;
			QMap<unsigned long, QObject *> qml_types;
	};

}

#endif
