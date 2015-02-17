#ifndef BRIG_DYNAMIC_QMETAOBJECT_BUILDER_H
#define BRIG_DYNAMIC_QMETAOBJECT_BUILDER_H

#include <node.h>
#include <QObject>
#include <internal/qmetaobjectbuilder_p.h>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class DynamicQMetaObjectBuilder {

		public:
			DynamicQMetaObjectBuilder(const char *typeName);
			~DynamicQMetaObjectBuilder(void);

			QMetaObject *build();
			void addSignal(const char *signature, QStringList argumentNames, Handle<Value> cb);

		private:

			QMetaObjectBuilder *builder;
			QMetaPropertyBuilder *property_builder;
			QMetaMethodBuilder *method_builder;

		protected:

			char *_typeName;
			QVector<Callback *> _signals;
			QVector<Callback *> _methods;
	};

}

#endif
