#ifndef BRIG_SIGNAL_HANDLER_H
#define BRIG_SIGNAL_HANDLER_H

#include <node.h>
#include <QObject>
#include "brig.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	class SignalHandler : public QObject {

		public:
			SignalHandler(QObject *);
			~SignalHandler();

			int qt_metacall(QMetaObject::Call call, int id, void **arguments);
			int addCallback(Handle<Value> cb);
			bool connectToSignal(char *signal, QObject *obj, char *slot);

		private:

			QObject *obj;

		protected:

			QVector<Callback *> callbacks;
	};

}

#endif
