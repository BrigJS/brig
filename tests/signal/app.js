"use strict";

var Brig = require('../../');

var brig = new Brig();

// Create Engine
var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

// Loading QML file
var component = new brig.QmlComponent(engine, 'application.qml');
var item = component.create(context);

//console.log(item.getMethods());

// Invoke
setTimeout(function() {
	var timestamp = Date.now();
	console.log('Correct data: ', timestamp, 'Hello', 123);
	item.emit('touched', timestamp, 'Hello', 123);
	item.emit('testInt', 99, 100);
	item.emit('testVariant', 'Variant String');
}, 1000);

setInterval(function() {
	item.emit('heartBeated');
}, 1000);
