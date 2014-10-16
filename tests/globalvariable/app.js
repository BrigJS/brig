"use strict";

var Brig = require('../../');

var brig = new Brig();

// Create Engine
var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

context.setContextProperty('helloGlobal', 'Test');

// Loading QML file
var component = new brig.QmlComponent(engine, 'application.qml');
var item = component.create(context);

// Getting global variable
console.log(context.contextProperty('helloGlobal'));
