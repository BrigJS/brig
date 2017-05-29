<p align="center">
<a href="https://github.com/cfsghost/brig">
<img src="https://cloud.githubusercontent.com/assets/252072/26544332/0329d67c-4494-11e7-9dc3-7dbaccfc5bd8.png">
</a>
</p>

Brig
====

A user interface engine for Node.js and io.js, which is based on Qt for rendering. This project can be used to load and play QML file, make it possible to have a easy way to communicate between QML and Node.js. That means developer is able to write desktop application in QML with pure JavaScript.

## Requirements

Ensure Qt 5+ tookits and Node.js 0.10+ are ready to go on your system.

## Installation

Install module via NPM
```
npm install brig
```

## Get Started

There is a simple way to go by loading existed QML file, so you can prepare a QML content like below:

__Application.qml__
```qml
import QtQuick 2.3
import QtQuick.Controls 1.0

ApplicationWindow {
	visible: true;
	color: 'black';
	title: 'Brig Demo';
	width: 640;
	height: 480;

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
```

Then using `Brig` to load file in Node.js:

```javascript
var Brig = require('brig');

var brig = new Brig();

brig.on('ready', function(brig) {

  // Loading QML file to play
  brig.open('Application.qml', function(err, window) {
    // window was opened
  });
});

```

## Demonstration

Here is a great countdown timer with `Brig` for hackathon event to show off, you can click image to play YouTube video:

[![Countdown Timer](https://cloud.githubusercontent.com/assets/252072/7604018/44697ee8-f96f-11e4-9690-db826fccbc22.png)](http://www.youtube.com/watch?v=D6CnZfK723M)
(clicks to play video)

## License

Licensed under the MIT License

## Authors

Copyright(c) 2015-2017 Fred Chien <<cfsghost@gmail.com>>
