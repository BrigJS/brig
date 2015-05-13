"use strict";

var fs = require('fs');
var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {

	brig.createType('MyItem', {
		methods: {
			readFile: function(filename) {
				return fs.readFileSync(filename);
			}
		},
		signals: {
			'test()': function() {
				console.log('Signal TEST');
			}
		}
	});

	brig.open('application.qml', function(err, window) {

	});
});
