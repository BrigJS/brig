#include <node.h>
#include <nan.h>
#include <QtGui>
#include <QObject>
#include <QTextCodec>
#include <QQuickStyle>
#include <uv.h>
#include "qapplication.h"
#include "eventloop.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Nan::Persistent<Function> QApplicationWrap::constructor;

	QApplicationWrap::QApplicationWrap() : ObjectWrap()
	{
		app_argc = 0;
		app_argv = NULL;
#if 0
		QStringList libPaths;
		libPaths << "../../node_modules/qt-darwin/PlugIns";
		libPaths << "../../node_modules/qt-darwin/Frameworks";
		libPaths << "../../node_modules/qt-darwin/Resources/qml";
		QCoreApplication::setLibraryPaths(libPaths);
#endif
#if 1
		QCoreApplication::addLibraryPath("../../node_modules/qt-darwin/Frameworks");
		QCoreApplication::addLibraryPath("../../node_modules/qt-darwin/PlugIns");
		QCoreApplication::addLibraryPath("../../node_modules/qt-darwin/Resources/qml");
		QCoreApplication::addLibraryPath("node_modules/qt-darwin/Frameworks");
		QCoreApplication::addLibraryPath("node_modules/brig/node_modules/qt-darwin/Frameworks");
		QCoreApplication::addLibraryPath("node_modules/qt-darwin/PlugIns");
		QCoreApplication::addLibraryPath("node_modules/brig/node_modules/qt-darwin/PlugIns");
		QCoreApplication::addLibraryPath("node_modules/qt-darwin/Resources/qml");
		QCoreApplication::addLibraryPath("node_modules/brig/node_modules/qt-darwin/Resources/qml");
#endif
		dispatcher = new BrigEventDispatcher;
		QGuiApplication::setEventDispatcher(dispatcher);
		QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

		app = new QGuiApplication(app_argc, app_argv);
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

#if 0
		qDebug() << QCoreApplication::applicationDirPath();
		QStringList paths = QCoreApplication::libraryPaths();
		for (int i = 0; i < paths.count(); ++i) {
			const QString &pluginDir = paths.at(i);
			qDebug() << pluginDir;
		}
#endif
//		QCoreApplication *app1 = QCoreApplication::instance();
//		QCoreApplication::removePostedEvents(app1, QEvent::Quit);

		// Initializing event loop
		//eventloop = new EventLoop(app);
		//eventloop->Main();
	}

	QApplicationWrap::~QApplicationWrap()
	{
		delete app;
		delete dispatcher;
	}

	NAN_MODULE_INIT(QApplicationWrap::Initialize) { 

		Local<String> name = Nan::New("QApplication").ToLocalChecked();

		/* Constructor template */
		Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(QApplicationWrap::New);
//		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QApplicationWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		Nan::SetPrototypeMethod(tpl, "setApplicationName", QApplicationWrap::setApplicationName);
		Nan::SetPrototypeMethod(tpl, "setStyle", QApplicationWrap::setStyle);
		Nan::SetPrototypeMethod(tpl, "exec", QApplicationWrap::Exec);
		Nan::SetPrototypeMethod(tpl, "test", QApplicationWrap::Test);
//		NODE_SET_PROTOTYPE_METHOD(tpl, "exec", QApplicationWrap::Exec);
//		NODE_SET_PROTOTYPE_METHOD(tpl, "test", QApplicationWrap::Test);

		constructor.Reset(tpl->GetFunction());

//		NanAssignPersistent(constructor, tpl->GetFunction());
//		constructor = Persistent<Function>::New(tpl->GetFunction());

		Nan::Set(target, name, Nan::New(constructor));
	}

	NAN_METHOD(QApplicationWrap::New) {

		QApplicationWrap *app_wrap = new QApplicationWrap();
		app_wrap->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QApplicationWrap::setStyle) {

		// style name
		String::Utf8Value style(info[0]->ToString());

		QQuickStyle::setStyle(QString(*style));

		info.GetReturnValue().Set(Nan::Undefined());
	}

	NAN_METHOD(QApplicationWrap::setApplicationName) {

		// application name
		String::Utf8Value name(info[0]->ToString());

		QCoreApplication::setApplicationName(QString(*name));

		info.GetReturnValue().Set(Nan::Undefined());
	}
/*
	Handle<Value> QApplicationWrap::Exec(const Arguments& args)
	{
*/
	NAN_METHOD(QApplicationWrap::Exec) {

		QApplicationWrap *app_wrap = ObjectWrap::Unwrap<QApplicationWrap>(info.This());
		app_wrap->quickview->show();
//		app_wrap->GetEventLoop()->Main();
//		app_wrap->app->processEvents();
//		app_wrap->app->exec();

//		while(1) {
//			app_wrap->app->processEvents(QEventLoop::WaitForMoreEvents);
//		}

printf("EXEC\n");

		//return scope.Close(Undefined());
		info.GetReturnValue().Set(Nan::Undefined());
	}

	NAN_METHOD(QApplicationWrap::Test) {

		QApplicationWrap *app_wrap = ObjectWrap::Unwrap<QApplicationWrap>(info.This());
		app_wrap->dispatcher->wakeUp();
#if 0
printf("4\n");
//		QQuickView view;
//		view.setSource(QUrl::fromLocalFile("application.qml"));
//		view.show();
		app_wrap->quickview = new QQuickView;
		app_wrap->quickview->setSource(QUrl::fromLocalFile("application.qml"));
printf("5\n");
#if 0
//		app_wrap->app->processEvents(QEventLoop::WaitForMoreEvents);

//		app_wrap->app->exec();
#endif
#endif
		//return scope.Close(Undefined());
		info.GetReturnValue().Set(Nan::Undefined());
	}
}
