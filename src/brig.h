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
	class QmlContext;
	class QuickItem;
	class QmlTypeBuilder;
	class DynamicQObject;
	class DynamicQMetaObjectBuilder;

	typedef enum {
		BRIG_CONTAINER_NATIVE
	} BrigContainerType;
}

#include "utils.h"
#include "qapplication.h"
#include "callback.h"
#include "signal_handler.h"
#include "DynamicQMetaObjectBuilder.h"
#include "DynamicQObject.h"
#include "QmlEngine.h"
#include "QmlComponent.h"
#include "QmlContext.h"
#include "QuickItem.h"
#include "QmlTypeBuilder.h"

#endif
