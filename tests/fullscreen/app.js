"use strict";

var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {

	var component = brig.createComponent();
	component.setData('import QtQuick 2.0\nimport QtQuick.Controls 1.2\nimport QtQuick.Window 2.1\nApplicationWindow {\nvisibility: Window.FullScreen; color: \"red\"\n }');

	var window = component.create();
});
