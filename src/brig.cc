#include <node.h>
#include "brig.h"
#include "qapplication.h"

namespace Brig {

	using namespace node;
	using namespace v8;

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
		}

		NODE_MODULE(brig, Init)
	}
}
