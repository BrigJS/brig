"use strict";

var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {
	brig.open('application.qml', function(err, window) {

		window.on('heartBeated', function() {
			console.log('Listener in Node.js Scope');
		});

		setInterval(function() {
			window.emit('heartBeated');
		}, 1000);

		setTimeout(function() {
			var timestamp = Date.now();
			console.log('Correct data: ', timestamp, 'Hello', 123);
			window.emit('touched', timestamp, 'Hello', 123);
			window.emit('testInt', 99, 100);
			window.emit('testVariant', 'Variant String');
		}, 1000);
	});
});
