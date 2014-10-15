#include <node.h>
#include "brig.h"
#include "qapplication.h"

namespace Brig {

	using namespace node;
	using namespace v8;

	Handle<Value> Cast(const Arguments& args)
	{
		HandleScope scope;

		return Undefined();
	}

	extern "C" {
		static void Init(Handle<Object> target)
		{
			QObjectWrap::Initialize(target);
			QApplicationWrap::Initialize(target);
			QmlEngineWrap::Initialize(target);
			QmlContextWrap::Initialize(target);
			QmlComponentWrap::Initialize(target);
			QuickViewWrap::Initialize(target);
			QuickWindowWrap::Initialize(target);
			QuickItemWrap::Initialize(target);

			// Cast
			NODE_SET_METHOD(target, "cast", Cast);
		}

		NODE_MODULE(brig, Init)
	}
}
