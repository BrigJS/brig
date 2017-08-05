<p align="center">
<a href="https://github.com/cfsghost/brig">
<img src="https://cloud.githubusercontent.com/assets/252072/26544332/0329d67c-4494-11e7-9dc3-7dbaccfc5bd8.png">
</a>
</p>

Brig
====

A user interface toolkit for Node.js, which is based on Qt for rendering. This project can be used to load and play QML file, make it possible to have a easy way to communicate between QML and Node.js for desktop application.

[![NPM](https://nodei.co/npm/brig.png?downloads=true&downloadRank=true&stars=true)](https://nodei.co/npm/brig/)
[![NPM](https://nodei.co/npm-dl/brig.png?months=9&height=2)](https://nodei.co/npm/brig/)

## The JavaScript in QML is limited, so `Brig`

The JavaScript in QML is limited, which is used for QML components control only. If developer need more functions that something's like system calls(FileSystem, Socket, crypto ...etc) or using external libraries, C/C++ is the only way to make it.

In order to make QML application in pure JavaScript possible, `Brig` provides a way to use Node.js to extends QML without C/C++. That means developer can do more thing in QML with NPM modules and Node.js APIs.

## Wht

The JavaScript in QML is limited, which is used for QML components control only. If developer need more functions that something's like system calls(FileSystem, Socket, crypto ...etc) or using external libraries, C/C++ is the only way to make it. Now, `Brig` provides a way to use Node.js to extends QML without C/C++, that means developer can do more thing in QML with NPM modules and Node.js APIs.

## Requirements

Ensure Qt 5+ tookits and Node.js 0.10+ are ready to go on your system.

For MacOSX user, no need to install Qt development environment for brig anymore. :-)

## Known Issues

* Failed to build brig for Qt 5.8+

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

## Customized Type

_(This is experimental feature, API might be changed in the next version)_

You can create a customized type in order to expose some APIs or functionality from Node.js to QML, see below:

```javascript
var myQmlType = brig.createType('MyItem', {
	property: {
		prop1: 123
	},
	method: {
		'readFile(a)': function(filename) {
			return require('fs').readFileSync(filename).toString();
		}
	},
	signal: [
		'test(a)'
	]
});

// Triggered when instance of customized type was created
myQmlType.on('instance-created', function(instance) {

	// Signals
	instance.on('test', function(a) {
		// test(a) signal was emitted
		console.log(a);
	});
});
```

In QML, we can import customized type with its type name and use it directly:

```qml
import Brig.MyItem 1.0
```

Usage:
```
MyItem {
	// attributes...
}
```

## Examples

Some exmaples you can found which used brig:

* [Example App](https://github.com/cfsghost/brig-example-app)
	* Example application for a demo
* [FishFarming](https://github.com/cfsghost/FishFarmingBrowser)
	* Web browser
* [Hankathon](https://github.com/HanGee/Hankathon)
	* A countdown timer for hackathon event

## Demonstration

Here is a great countdown timer with `Brig` for hackathon event to show off, you can click image to play YouTube video:

[![Countdown Timer](https://cloud.githubusercontent.com/assets/252072/7604018/44697ee8-f96f-11e4-9690-db826fccbc22.png)](http://www.youtube.com/watch?v=D6CnZfK723M)
(clicks to play video)

## License

Licensed under the MIT License

## Authors

Copyright(c) 2015-2017 Fred Chien <<cfsghost@gmail.com>>
