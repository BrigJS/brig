#ifndef BRIG_CALLBACK_H
#define BRIG_CALLBACK_H

#include <node.h>
#include <QObject>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	struct Callback {
		char *signal;
		Nan::Callback *handler;

		~Callback() {
			delete handler;
			delete signal;
		}
	};

}

#endif
