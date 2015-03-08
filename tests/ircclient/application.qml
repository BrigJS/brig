import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

ApplicationWindow {
	width: 300;
	height: 300;
	color: "black";
	visible: true;

	signal updatedIRC(var msg);

	Rectangle {
		color: '#cc000000';
		anchors.fill: parent;

		Text {
			id: ircBox;
			color: '#f0ffffff';	
			style: Text.Outline;
			styleColor: '#33ffffff';
			wrapMode: Text.WordWrap;
			textFormat: Text.RichText;
			text: 'Initializing ...';
		}
	}

	onUpdatedIRC: {
		ircBox.text = msg;
	}
}
