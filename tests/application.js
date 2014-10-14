"use strict";

var Brig = require('../');

var brig = new Brig();

// Create Engine
var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

// Loading QML file
var component = new brig.QmlComponent(engine, 'application.qml');
var item = component.create(context);

// Set window title
item.setProperty('title', 'Helloooooo Brig');

console.log(item.getProperty('width'));
