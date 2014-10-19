import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

ApplicationWindow {
	width: 300;
	height: 300;
	color: "red";
	visible: true;

	signal heartBeated();
	signal touched(var timestamp, var msg, int num);
	signal testInt(int num1, int num2);
	signal testVariant(variant str);

	onHeartBeated: {
		console.log('Listener in QML scope');
	}

	onTouched: {
		console.log('Touched', timestamp, msg, num);
	}

	onTestInt: {
		console.log('TestInteger', num1, num2);
	}

	onTestVariant: {
		console.log('TestVariant', str);
	}
}
