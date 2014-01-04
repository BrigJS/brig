#ifndef BRIG_H
#define BRIG_H

#include <node.h>

namespace Brig {

	using namespace v8;
	using namespace node;

	class QmlEngineWrap;
	class QmlContextWrap;
	class QmlComponentWrap;
	class QuickViewWrap;

	typedef enum {
		BRIG_CONTAINER_NATIVE
	} BrigContainerType;
}

#include "qapplication.h"
#include "qmlengine.h"
#include "qmlcontext.h"
#include "qmlcomponent.h"
#include "quickview.h"

#endif
