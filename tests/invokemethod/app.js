"use strict";

var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {
	brig.open('application.qml', function(err, window) {

		// Invoke
		var result = window.invokeMethod('hello', 1, 2);
		console.log(result);

	});
});
