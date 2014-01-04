"use strict";

var Brig = require('../');

var brig = new Brig();

var view = new brig.QuickView();
view.setSource('application.qml');
view.show();
/*
var engine = new brig.QmlEngine();
var rootContext = engine.rootContext();
var context = new brig.QmlContext(rootContext);

var component = new brig.QmlComponent(engine, 'application.qml');

component.create(context);
*/
//brig.main();
