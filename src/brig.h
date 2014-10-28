#ifndef BRIG_H
#define BRIG_H

#include <node.h>

namespace Brig {

	using namespace v8;
	using namespace node;

	class QmlEngineWrap;

	typedef enum {
		BRIG_CONTAINER_NATIVE
	} BrigContainerType;
}

#include "utils.h"
#include "qapplication.h"
#include "qmlengine.h"
#include "QmlComponent.h"

#endif
