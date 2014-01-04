#ifndef BRIG_QUICKVIEW_WRAP_H
#define BRIG_QUICKVIEW_WRAP_H

#include <node.h>
#include <QQuickView>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class QuickViewWrap : public ObjectWrap {

		public:
			QuickViewWrap();
			~QuickViewWrap();

			static Persistent<Function> constructor;
			static void Initialize(Handle<Object> target);

			QQuickView *GetObject() const { return obj; };

		private:

			static Handle<Value> New(const Arguments& args);

			/* Methods */
			static Handle<Value> setSource(const Arguments& args);
			static Handle<Value> show(const Arguments& args);

			QQuickView *obj;
	};

}

#endif
