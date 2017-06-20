#include <node.h>
#include "brig.h"
#include "QApplication.h"

namespace Brig {

	using namespace node;
	using namespace v8;

	NAN_METHOD(Cast) {
		info.GetReturnValue().Set(Nan::Undefined());
	}

	extern "C" {
		static void Init(Handle<Object> target)
		{
//			QObjectWrap::Initialize(target);
			QApplicationWrap::Initialize(target);
			QmlEngineWrap::Initialize(target);
			QmlContext::Initialize(target);
			QmlComponent::Initialize(target);
			QuickItem::Initialize(target);
			QmlTypeBuilder::Initialize(target);

//			QmlContextWrap::Initialize(target);
//			QmlComponentWrap::Initialize(target);
//			QuickViewWrap::Initialize(target);
//			QuickWindowWrap::Initialize(target);
//			QuickItemWrap::Initialize(target);

			// Cast
//			NODE_SET_METHOD(target, "cast", Cast);
		}

		NODE_MODULE(brig, Init)
	}
}
