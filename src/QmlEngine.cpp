#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "QmlEngine.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Nan::Persistent<Function> QmlEngineWrap::constructor;

	QmlEngineWrap::QmlEngineWrap() : ObjectWrap()
	{
		obj = new QQmlEngine();
		obj->setOutputWarningsToStandardError(true);
		signal = new SignalHandler(obj);
#if 0
		QStringList importPaths;
		importPaths << "../../node_modules/qt-darwin/Resources/qml";
		importPaths << "qrc:/qt-project.org/imports";
		importPaths << "/Users/fred/Qt/5.7/clang_64/qml";
		obj->setImportPathList(importPaths);
#endif

		obj->addImportPath("../../node_modules/qt-darwin/Resources/qml");
		obj->addImportPath("node_modules/qt-darwin/Resources/qml");
		obj->addImportPath("node_modules/brig/node_modules/qt-darwin/Resources/qml");
#if 0
		qDebug() << "importPathList";
		QStringList paths = obj->importPathList();
		for (int i = 0; i < paths.count(); ++i) {
			const QString &pluginDir = paths.at(i);
			qDebug() << pluginDir;
		}
#endif
#if 1
		QStringList pluginPaths;
		pluginPaths << ".";
		pluginPaths << "../../node_modules/qt-darwin/PlugIns";
		pluginPaths << "node_modules/qt-darwin/PlugIns";
		pluginPaths << "node_modules/brig/node_modules/qt-darwin/PlugIns";
		obj->setPluginPathList(pluginPaths);
#endif

#if 0
		qDebug() << "pluginPathList";
		QStringList _paths = obj->pluginPathList();
		for (int i = 0; i < _paths.count(); ++i) {
			const QString &pluginDir = _paths.at(i);
			qDebug() << pluginDir;
		}
#endif
	}

	QmlEngineWrap::~QmlEngineWrap()
	{
		delete signal;
		delete obj;
	}

	NAN_MODULE_INIT(QmlEngineWrap::Initialize) {

		Local<String> name = Nan::New("QmlEngine").ToLocalChecked();

		/* Constructor template */
		Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(QmlEngineWrap::New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		Nan::SetPrototypeMethod(tpl, "on", QmlEngineWrap::on);
		Nan::SetPrototypeMethod(tpl, "addImportPath", QmlEngineWrap::addImportPath);
		Nan::SetPrototypeMethod(tpl, "addPluginPath", QmlEngineWrap::addPluginPath);
//		Nan::SetPrototypeMethod(tpl, "rootContext", QmlEngineWrap::rootContext);

		//constructor = Nan::Persistent<Function>::New(tpl->GetFunction());
		constructor.Reset(tpl->GetFunction());
		//NanAssignPersistent(constructor, tpl->GetFunction());

		target->Set(name, Nan::New(constructor));
	}

	NAN_METHOD(QmlEngineWrap::New) {

		QmlEngineWrap *obj_wrap = new QmlEngineWrap();
		obj_wrap->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QmlEngineWrap::on) {

		QmlEngineWrap *obj_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(info.This());

		// Signal name
		String::Utf8Value name(info[0]->ToString());

		obj_wrap->signal->addCallback(*name, info[1]);

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QmlEngineWrap::addImportPath) {

		QmlEngineWrap *obj_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(info.This());

		String::Utf8Value path(info[0]->ToString());

		obj_wrap->obj->addImportPath(*path);

		info.GetReturnValue().Set(info.This());
	}

	NAN_METHOD(QmlEngineWrap::addPluginPath) {

		QmlEngineWrap *obj_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(info.This());

		String::Utf8Value path(info[0]->ToString());

		obj_wrap->obj->addPluginPath(*path);

		info.GetReturnValue().Set(info.This());
	}
/*
	Local<Value> QmlEngineWrap::rootContext(const Arguments& args)
	{
		HandleScope scope;

		QmlEngineWrap *obj_wrap = ObjectWrap::Unwrap<QmlEngineWrap>(info.This());

		QQmlEngine *engine = obj_wrap->GetObject();
		QQmlContext *context = engine->rootContext();

		Local<Value> instance = QmlContextWrap::NewInstance(context);

		return scope.Close(instance);
	}
*/
}
