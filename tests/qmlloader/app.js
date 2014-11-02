"use strict";

var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {
	brig.open('application.qml', function(err, window) {

		// Set window title
		window.setProperty('title', 'Helloooooo Brig');

		// Get all properties
		var keys = window.getPropertyNames();
		for (var index in keys) {
			var key = keys[index];
			console.log(key, ":", window.getProperty(key));
		}

	});
});
/*
// Create Engine
var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

// Loading QML file
var component = new brig.QmlComponent(engine, 'application.qml');
var item = component.create(context);

// Set window title
item.setProperty('title', 'Helloooooo Brig');

*/
