import QtQuick 2.3
import QtQuick.Controls 1.0
import Brig.MyItem 1.0
import Brig.Second 1.0

ApplicationWindow {
	visible: true;
	color: 'black';
	title: 'Brig Demo';
	width: 640;
	height: 480;

	Second {
		onTest: {
			console.log('Second: Got SIGNAL!');
		}

		Component.onCompleted: {
			console.log('Second: Ready');
			console.log('Second:' + this.prop1);
			var ret = this.sum(3, 5);
			console.log(ret);
		}
	}

	MyItem {
		onTest: {
			console.log('Got SIGNAL!');
		}

		onTest2: {
			console.log('Got SIGNAL 2222!');
		}

		function xxx(X) {
			console.log('xxx', X);
		}

		Component.onCompleted: {
			console.log('============ MyItem');
			console.log('=================> test()');
			this.test(123);
			console.log('=================> test2()');
			this.test2();
			console.log('=================> methodTest()');
			//var x = this.methodTest(123, 222, 333, 444, 555, 666, 777, 888);
			var x = this.methodTest(123, 222, 333, 444, 555, 666);
			console.log('QQ', x);
			this.xxx(999);
//			console.log('=================> readFile()');
			var content = this.readFile('./sample.txt');
			console.log('MyItem:' + content);
			console.log('============ MyItem COMPLETE');
		}
	}

	Text {
		anchors.centerIn: parent;
		text: 'Brig';
		font.family: 'Helvetica';
		font.bold: true;
		font.pointSize: 72;
		color: '#00ffcc';
		scale: 0;
		opacity: 0;

		Text {
			anchors.topMargin: 10;
			anchors.top: parent.bottom; 
			anchors.horizontalCenter: parent.horizontalCenter;
			text: 'QML Application in Node.js';
			font.family: 'Helvetica';
			font.pointSize: 16;
			color: '#e6fffa';
		}

		Behavior on opacity {
			NumberAnimation {
				duration: 800;
				easing.type: Easing.OutCubic;
			}
		}

		Behavior on scale {
			NumberAnimation {
				duration: 1000;
				easing.type: Easing.OutBack;
			}
		}

		Component.onCompleted: {
			opacity = 1.0;
			scale = 1.0;
		}
	}
}
