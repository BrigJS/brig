"use strict";

var fs = require('fs');
var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {

	brig.on('window-all-closed', () => {
		process.exit();
	});

	var qmlType = brig.createType('MyItem', {
		property: {
			item: null
		}
	});

	qmlType.on('instance-created', function(instance) {
		var item = instance.getProperty('item');

		instance.on('itemChanged', function() {
			var item = instance.getProperty('item');
			if (item === null)
				return;

			item.setProperty('scale', 5);
		});
	});

	brig.open('application.qml', function(err, window) {
	});
});
