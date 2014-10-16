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

// Finding specific method
var methodSig = null;
var methods = item.getMethods();
for (var id in methods) {
	var method = methods[id];

	if (method.indexOf('hello(') == 0) {
		methodSig = method;
		break;
	}
}

// Invoke
item.invokeMethod(methodSig, 'xxx', 123);
