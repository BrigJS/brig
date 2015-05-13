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
			SignalHandler(void);
			SignalHandler(QObject *);
			~SignalHandler(void);

			QObject *GetObject() const { return obj; };

			virtual int qt_metacall(QMetaObject::Call call, int id, void **arguments);
			virtual int findSignalId(const char *signal);
			virtual bool setObject(QObject *_obj);
			virtual int addCallback(const char *signal, Handle<Value> cb);

		private:

			QObject *obj;

		protected:

			QVector<Callback *> callbacks;
	};

}

#endif
