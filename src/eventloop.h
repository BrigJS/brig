#ifndef BRIG_EVENTLOOP_H
#define BRIG_EVENTLOOP_H

#include <node.h>
#include <QObject>
#include <QGuiApplication>
#include <QAbstractEventDispatcher>

namespace Brig {

	using namespace v8;
	using namespace node;

	class EventLoop : public QObject {
		Q_OBJECT

		public:
			EventLoop(QGuiApplication *);
			~EventLoop(void);

			QGuiApplication *GetApp() const { return app; };
			uv_async_t *GetMainLoop() const { return mainloop; };
			QAbstractEventDispatcher *eventDispatcher;
			void Main();

		private:

			static void PrepareHandle(uv_prepare_t *handle, int status);

			QGuiApplication *app;
			QEventLoop *eventloop;
			uv_async_t *mainloop;
	};

}

#endif
