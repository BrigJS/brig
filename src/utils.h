#ifndef BRIG_UTILS_H
#define BRIG_UTILS_H

#include <node.h>
#include <QObject>
#include <QQmlEngine>
#include <QVariant>
#include <QJSValue>

namespace Brig {

	using namespace v8;
	using namespace node;

	namespace Utils {

		struct ParamData {
			void *ptr = NULL;
			QVariant *qvariant = NULL;
			QString *qstring = NULL;

			union {
				float floatValue;
				double doubleValue;
				qint32 intValue;
				quint32 uintValue;
				long long longValue;
				unsigned long long ulongValue;
				bool boolValue;
			};

			ParamData() {
				// Null
				qstring = new QString();
				ptr = qstring;
			}

			ParamData(float val) {
				floatValue = val;
				ptr = &floatValue;
			}

			ParamData(double val) {
				doubleValue = val;
				ptr = &doubleValue;
			}

			ParamData(qint32 val) {
				intValue = val;
				ptr = &intValue;
			}

			ParamData(quint32 val) {
				uintValue = val;
				ptr = &uintValue;
			}

			ParamData(long long val) {
				longValue = val;
				ptr = &longValue;
			}

			ParamData(unsigned long long val) {
				ulongValue = val;
				ptr = &ulongValue;
			}

			ParamData(bool val) {
				boolValue = val;
				ptr = &boolValue;
			}

			ParamData(QVariant val) {
				qvariant = new QVariant(val);
				ptr = qvariant;
			}

			ParamData(const char *val) {
				qstring = new QString(val);
				ptr = qstring;
			}

			~ParamData() {
				if (qvariant != NULL)
					delete qvariant;
				else if (qstring != NULL)
					delete qstring;
			};
		};

		Local<Value> QDataToV8(int type, void *value);
		Local<Value> QVariantToV8(int type, QVariant v);
		QVariant V8ToQVariant(Local<Value> value);
		QJSValue V8ToQJSValue(QQmlEngine *engine, Local<Value> value);
		ParamData *MakeParameter(int type, Local<Value> value);
		QGenericArgument MakeArgument(int type, Local<Value> value);
	}
}

#endif
