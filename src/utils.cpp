#include <QObject>
#include <QVariant>
#include "utils.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	namespace Utils {

		QVariant V8ToQVariant(Handle<Value> value)
		{
			QVariant v;

			// Check data type
			if (value->IsTrue() || value->IsFalse() || value->IsBoolean() ) {
				v.setValue(QVariant(value->ToBoolean()->Value()));
			} else if (value->IsNumber()) {
				v.setValue(QVariant(value->NumberValue()));
			} else if (value->IsInt32()) {
				v.setValue(QVariant(value->ToInt32()->Value()));
			} else if (value->IsString()) {
				String::Utf8Value _v(value->ToString());
				v.setValue(QVariant(static_cast<char *>(*_v)));
			}

			return v;
		}
	}
}
