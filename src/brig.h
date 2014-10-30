#ifndef BRIG_H
#define BRIG_H

#include <node.h>

namespace Brig {

	using namespace v8;
	using namespace node;

	struct Callback;

	class SignalHandler;
	class QmlEngineWrap;
	class QmlComponent;
	class QuickItem;

	typedef enum {
		BRIG_CONTAINER_NATIVE
	} BrigContainerType;
}

#include "utils.h"
#include "qapplication.h"
#include "callback.h"
#include "signal_handler.h"
#include "qmlengine.h"
#include "QmlComponent.h"
#include "QuickItem.h"

#endif
