"use strict";

var fs = require('fs');
var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {

	var qmlType = brig.createType('MyItem', {
		property: {
			prop1: 123
		},
		method: {
			'methodTest(a,b,c,d,e)':  function(a, b, c, d, e) {
				console.log('Method Test', a, b, c, d, e);

				return 123;
			},
			'readFile(a)': function(filename) {
				return fs.readFileSync(filename).toString();
			}
		},
		signal: {
			'test(a)': function(a) {
				console.log('Signal TEST', a);
			},
			'test2()': function() {
				console.log('Signal TEST 2');
			}
		}
	});

	var qmlTypeSecond = brig.createType('Second', {
		property: {
			prop1: 'Sum'
		},
		method: {
			'sum(a,b)': function(a, b) {
				return a + b;
			}
		},
		signal: {
			'test(a)': function(a) {
				console.log('Signal TEST', a);
			}
		}
	});

	brig.open('application.qml', function(err, window) {

	});
});
