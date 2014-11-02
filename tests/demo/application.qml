import QtQuick 2.3
import QtQuick.Controls 1.0

ApplicationWindow {
	visible: true;
	color: 'black';
	title: 'OwaViwer';
	width: 640;
	height: 480;

	Text {
		anchors.centerIn: parent;
		text: 'OwaViewer';
		font.family: 'Helvetica';
		font.bold: true;
		font.pointSize: 48;
		color: '#00ffcc';
		scale: 0;
		opacity: 0;

		Text {
			anchors.top: parent.bottom; 
			anchors.horizontalCenter: parent.horizontalCenter;
			text: 'Show off your amazing UI here';
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
