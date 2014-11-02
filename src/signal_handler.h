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
			SignalHandler();
			SignalHandler(QObject *);
			~SignalHandler();

			virtual int qt_metacall(QMetaObject::Call call, int id, void **arguments);
			int findSignalId(const char *signal);
			bool setObject(QObject *_obj);
			int addCallback(const char *signal, Handle<Value> cb);

		private:

			QObject *obj;

		protected:

			QVector<Callback *> callbacks;
	};

}

#endif
