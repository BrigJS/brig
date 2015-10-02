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

			static NAN_MODULE_INIT(Initialize);

			QQmlContext *GetObject() const { return obj; };

		private:

			static NAN_METHOD(New);

			/* Methods */
			static NAN_METHOD(setEngine);

			QQmlContext *obj;

			static Nan::Persistent<Function> constructor;
	};

}

#endif
