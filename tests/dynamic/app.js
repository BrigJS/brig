"use strict";

var Brig = require('../../');

var brig = new Brig();

var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

var component = new brig.QmlComponent(engine);
component.setData('import QtQuick 2.0\nimport QtQuick.Window 2.0\nWindow {\nwidth: 300\n height: 300\n color: \"red\"\ntitle: \"Hello\" }');
var qObject = component.create(context);

var window = qObject.toQuickWindow();
window.show();
