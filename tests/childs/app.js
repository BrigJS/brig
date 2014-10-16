"use strict";

var Brig = require('../../');

var brig = new Brig();

var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

// Create a window
var component = new brig.QmlComponent(engine);
component.setData('import QtQuick 2.0\nimport QtQuick.Window 2.0\nWindow {\nwidth: 600\n height: 600\ntitle: \"Hello\" }');
var qObject = component.create(context);
var window = qObject.toQuickWindow();
window.show();

// Loading QML file
var Rect = new brig.QmlComponent(engine, 'rect.qml');
var rect = Rect.create(context).toQuickItem();
rect.setParentItem(qObject);

// Create an inline child
var rectangleComp = new brig.QmlComponent(engine);
rectangleComp.setData('import QtQuick 2.0\n Rectangle {\n width: 50\n height: 50\n color: \"orange\"\n }');
var rectangleObj = rectangleComp.create(context);
var item = rectangleObj.toQuickItem();
item.setParentItem(qObject);
