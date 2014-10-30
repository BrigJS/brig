#ifndef BRIG_CALLBACK_H
#define BRIG_CALLBACK_H

#include <node.h>
#include <QObject>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	struct Callback {
		Persistent<Function> handler;

		~Callback() {
			handler.Dispose();
			handler.Clear();
		}
	};

}

#endif
