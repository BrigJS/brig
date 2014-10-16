#ifndef BRIG_UTILS_H
#define BRIG_UTILS_H

#include <node.h>
#include <QObject>
#include <QVariant>

namespace Brig {

	using namespace v8;
	using namespace node;

	namespace Utils {
		QVariant V8ToQVariant(Handle<Value> value);
	}
}

#endif
