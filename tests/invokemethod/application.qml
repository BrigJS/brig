import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.0

ApplicationWindow {
	width: 300;
	height: 300;
	color: "red";
	visible: true;

	function hello(x, y) {
		console.log('invoked method');
	}
}
