"use strict";

var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {

	var component = brig.createComponent();
	component.setData('import QtQuick 2.0\nimport QtQuick.Window 2.0\nWindow {\nwidth: 600\n height: 600\ntitle: \"Hello\" }');

	var window = component.create();
	window.setProperty('visible', true);

	// Create a new component
	var childComponent1 = brig.createComponent();
	childComponent1.setData('import QtQuick 2.0\n Rectangle {\n x: 100\n y: 100\n width: 50\n height: 50\n color: \"orange\"\n }');
	var child1 = childComponent1.create();
	child1.setParent(window);

	// Load a component file
	var externalComponent = brig.createComponent();
	externalComponent.loadQmlFile('rect.qml');
	externalComponent.on('loaded', function() {

		var rect = externalComponent.create();

		rect.setParent(window);
	});
});
