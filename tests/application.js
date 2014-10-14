"use strict";

var Brig = require('../');

var brig = new Brig();

// Create Engine
var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

// Loading QML file
var component = new brig.QmlComponent(engine, 'application.qml');
component.create(context);
