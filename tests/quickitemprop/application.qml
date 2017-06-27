import QtQuick 2.3
import QtQuick.Controls 1.0
import Brig.MyItem 1.0

ApplicationWindow {
	visible: true;
	color: 'black';
	title: 'Brig Demo';
	width: 640;
	height: 480;

	MyItem {
		item: title;
	}

	Text {
		id: title;
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
