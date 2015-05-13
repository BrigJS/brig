"use strict";

var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {

	var component = brig.createComponent();
	component.setData('import QtQuick 2.0\nimport QtQuick.Window 2.0\nWindow {\nwidth: 300\n height: 300\n color: \"red\"\ntitle: \"Hello\" }');

	var window = component.create();
	window.setProperty('visible', true);
});
