#include <unistd.h>
#include <node.h>
#include <QtGui>
#include <QObject>
#include "QuickItem.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Nan::Persistent<Function> QuickItem::constructor;

	QuickItem::QuickItem() : ObjectWrap()
	{
		obj = NULL;
		signal = NULL;
	}

	QuickItem::~QuickItem()
	{
printf("RELEASE QuickItem\n");
		delete signal;
		delete obj;
	}

	NAN_MODULE_INIT(QuickItem::Initialize) {

		Local<String> name = Nan::New("QuickItem").ToLocalChecked();

		/* Constructor template */
		Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(QuickItem::New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		Nan::SetPrototypeMethod(tpl, "create", QuickItem::create);
		Nan::SetPrototypeMethod(tpl, "getPropertyNames", QuickItem::getPropertyNames);
		Nan::SetPrototypeMethod(tpl, "getProperty", QuickItem::getProperty);
		Nan::SetPrototypeMethod(tpl, "setProperty", QuickItem::setProperty);
		Nan::SetPrototypeMethod(tpl, "setParent", QuickItem::setParent);
		Nan::SetPrototypeMethod(tpl, "invokeMethod", QuickItem::invokeMethod);
		Nan::SetPrototypeMethod(tpl, "emitEvent", QuickItem::emitEvent);
		Nan::SetPrototypeMethod(tpl, "on", QuickItem::on);

		constructor.Reset(tpl->GetFunction());

		target->Set(name, Nan::New(constructor));
	}

	// Prototype Constructor
	NAN_METHOD(QuickItem::New) {

		QuickItem *obj_wrap = new QuickItem();
		obj_wrap->Wrap(info.This());

		info.GetReturnValue().Set(info.This());
	}

	// Method
	NAN_METHOD(QuickItem::create) {

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(info.This());

		QmlComponent *component = ObjectWrap::Unwrap<QmlComponent>(info[0]->ToObject());

		// Create QuickItem with component
		obj_wrap->obj = static_cast<QQuickItem *>(component->GetObject()->create());
		obj_wrap->signal = new SignalHandler(qobject_cast<QObject *>(obj_wrap->GetObject()));
//		obj_wrap->signal->setObject(qobject_cast<QObject *>(obj_wrap->GetObject()));

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QuickItem::getPropertyNames) {

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(info.This());

		Handle<Array> keys = Nan::New<Array>();

		// Getting property names
		static const QMetaObject *meta = obj_wrap->GetObject()->metaObject();
		for (int i = 0; i < meta->propertyCount(); i++) {
			keys->Set(i, Nan::New(QString(meta->property(i).name()).toUtf8().constData()).ToLocalChecked());
		}

		info.GetReturnValue().Set(keys);
	}

	NAN_METHOD(QuickItem::getProperty) {

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(info.This());

		if (!info[0]->IsString())
			Nan::ThrowTypeError("First argument must be a string");

		String::Utf8Value name(info[0]->ToString());

		// Get property
		QVariant v = obj_wrap->GetObject()->property(*name);

		// Convert Qvariant to V8 data type
		if (v.isNull())
			info.GetReturnValue().SetNull();

		info.GetReturnValue().Set(Utils::QVariantToV8(v.userType(), v));
	}

	NAN_METHOD(QuickItem::setProperty) {

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(info.This());

		if (!info[0]->IsString())
			Nan::ThrowTypeError("First argument must be a string");

		String::Utf8Value name(info[0]->ToString());
		Local<Value> value(info[1]);
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

		// Set property
		obj_wrap->GetObject()->setProperty(*name, v);

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QuickItem::setParent) {

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(info.This());

		QuickItem *item = ObjectWrap::Unwrap<QuickItem>(info[0]->ToObject());

		if (!item->GetObject())
			info.GetReturnValue().SetUndefined();

		QQuickItem *parentObj = NULL;
		if (item->GetObject()->isWindowType()) {
			// It's a window, so we need to get inside content item
			parentObj = qobject_cast<QQuickWindow *>(item->GetObject())->contentItem();
		} else {
			parentObj = item->GetObject();
		}

		obj_wrap->GetObject()->setParentItem(parentObj);

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QuickItem::invokeMethod) {

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(info.This());
		QObject *qobj = qobject_cast<QObject *>(obj_wrap->GetObject());

		if (!info[0]->IsString())
			Nan::ThrowTypeError("First argument must be a string");

		// Method name
		String::Utf8Value methodSig(info[0]->ToString());

		QVariant returnedValue;
		int infoLen = info.Length() - 1;

		// It supports only 10 arguments with limitation of Qt
		QMetaObject::invokeMethod(qobj, *methodSig,
			Qt::AutoConnection,
			Q_RETURN_ARG(QVariant, returnedValue),
			(infoLen > 0) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[1])) : QGenericArgument(),
			(infoLen > 1) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[2])) : QGenericArgument(),
			(infoLen > 2) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[3])) : QGenericArgument(),
			(infoLen > 3) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[4])) : QGenericArgument(),
			(infoLen > 4) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[5])) : QGenericArgument(),
			(infoLen > 5) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[6])) : QGenericArgument(),
			(infoLen > 6) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[7])) : QGenericArgument(),
			(infoLen > 7) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[8])) : QGenericArgument(),
			(infoLen > 8) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[8])) : QGenericArgument(),
			(infoLen > 9) ? Q_ARG(QVariant, Utils::V8ToQVariant(info[9])) : QGenericArgument());

		// Convert Qvariant to V8 data type
		if (returnedValue.isNull())
			info.GetReturnValue().SetNull();

		switch(returnedValue.userType()) {
		case QMetaType::Bool:
			info.GetReturnValue().Set(Nan::New<Boolean>(returnedValue.toBool()));
		case QMetaType::Int:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toInt()));
		case QMetaType::UInt:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toUInt()));
		case QMetaType::Float:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toFloat()));
		case QMetaType::Double:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toDouble()));
		case QMetaType::LongLong:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toLongLong()));
		case QMetaType::ULongLong:
			info.GetReturnValue().Set(Nan::New<Number>(returnedValue.toULongLong()));
		case QMetaType::QString:
			info.GetReturnValue().Set(Nan::New(returnedValue.toString().toUtf8().constData()).ToLocalChecked());
		case QMetaType::QColor:
			info.GetReturnValue().Set(Nan::New(returnedValue.value<QColor>().name(QColor::HexArgb).toUtf8().constData()).ToLocalChecked());
		}

		info.GetReturnValue().SetUndefined();
	}

	NAN_METHOD(QuickItem::emitEvent) {

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(info.This());
		QObject *qobj = qobject_cast<QObject *>(obj_wrap->GetObject());

		if (!info[0]->IsString())
			Nan::ThrowTypeError("First argument must be a string");

		// Method name
		String::Utf8Value methodSig(info[0]->ToString());

		QVariant returnedValue;
		int infoLen = info.Length() - 1;

		static const QMetaObject *meta = qobj->metaObject();
//		int methodIndex = meta->indexOfMethod(*methodSig);
//		QMetaMethod method = meta->method(methodIndex);
//
		// Getting currect context
		QQmlContext *thisContext = QQmlEngine::contextForObject(qobj);

		// Preparing parameter and ensure QJSValue have individual memory
//		QJSValue val[infoLen];
		QMap<int, QJSValue> vals;
		QList<QGenericArgument> parameters;
		QList<Utils::ParamData> dataList;

		// Getting signature
		for (int i = meta->methodOffset(); i < meta->methodCount(); ++i) {
			QMetaMethod method = meta->method(i);
			const char *methodName = method.name().data();
			if (strcmp(*methodSig, methodName) != 0)
				continue;

			if (!method.isValid())
				break;

			// Getting parameter types
			for (int j = 0; j < method.parameterCount(); j++) {
				int type = method.parameterType(j);

				if (j >= info.Length())
					break;

				Local<Value> value = info[j + 1];

				// Type is "var" in QML, which is different from QVariant
				if (type == qMetaTypeId<QJSValue>()) {
					vals.insert(j, Utils::V8ToQJSValue(thisContext->engine(), value));
					//val[j] = Utils::V8ToQJSValue(thisContext->engine(), value);

					// Undefined
					//parameters << QGenericArgument("QJSValue", static_cast<const void *>(&val[j]));
					parameters << QGenericArgument("QJSValue", static_cast<const void *>(&vals[j]));
					continue;
				}

				// Making arguments
				Utils::ParamData *data = Utils::MakeParameter(type, value);
				if (data == NULL) {
					// Unknown type, set Undefined to this parameter
					parameters << QGenericArgument();
					continue;
				}

				dataList << data;
				parameters << QGenericArgument(QMetaType::typeName(type), data->ptr);
			}

			// Invoke
			method.invoke(qobj,
				Qt::QueuedConnection,
				(infoLen > 0) ? parameters[0] : QGenericArgument(),
				(infoLen > 1) ? parameters[1] : QGenericArgument(),
				(infoLen > 2) ? parameters[2] : QGenericArgument(),
				(infoLen > 3) ? parameters[3] : QGenericArgument(),
				(infoLen > 4) ? parameters[4] : QGenericArgument(),
				(infoLen > 5) ? parameters[5] : QGenericArgument(),
				(infoLen > 6) ? parameters[6] : QGenericArgument(),
				(infoLen > 7) ? parameters[7] : QGenericArgument(),
				(infoLen > 8) ? parameters[8] : QGenericArgument(),
				(infoLen > 9) ? parameters[9] : QGenericArgument());

			// Release
			vals.clear();
			dataList.clear();
			parameters.clear();

			//info.GetReturnValue().Set(Nan::New<Boolean>(True));
			info.GetReturnValue().Set(Nan::True());
		}

		info.GetReturnValue().Set(Nan::False());
	}

	NAN_METHOD(QuickItem::on) {

		QuickItem *obj_wrap = ObjectWrap::Unwrap<QuickItem>(info.This());

		// Signal name
		String::Utf8Value url(info[0]->ToString());
		int id = obj_wrap->signal->addCallback(*url, info[1]);

		info.GetReturnValue().Set(info.This());
	}
}
