"use strict";

var Brig = require('../../');

var brig = new Brig();

var view = new brig.QuickView();
//view.setSource('application.qml');
//view.show();

var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

var component = new brig.QmlComponent(engine);
component.setData('import QtQuick 2.0\nRectangle { width: 300; height: 300; color: \"red\"; }');
var comp = component.create(context);
