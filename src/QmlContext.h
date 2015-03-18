#ifndef BRIG_QMLCONTEXT_H
#define BRIG_QMLCONTEXT_H

#include <node.h>
#include <QQmlContext>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QmlContext : public ObjectWrap {

		public:
			QmlContext();
			~QmlContext();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);

			QQmlContext *GetObject() const { return obj; };

		private:

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(setEngine);

			QQmlContext *obj;
	};

}

#endif
