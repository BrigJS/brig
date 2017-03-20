"use strict";

var fs = require('fs');
var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {

	brig.createType('MyItem', {
		methods: {
			'methodTest(a,b,c,d,e)':  function(a, b, c, d, e) {
				console.log('Method Test', a, b, c, d, e);

				return 123;
			},
			'readFile(a)': function(filename) {
				return fs.readFileSync(filename).toString();
			}
		},
		signals: {
			'test(a)': function(a) {
				console.log('Signal TEST', a);
			},
			'test2()': function() {
				console.log('Signal TEST 2');
			}
		}
	});

	brig.open('application.qml', function(err, window) {

	});
});
