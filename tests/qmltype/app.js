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

	qmlType.on('instance-created', function(instance) {
		console.log('[Main Process]', '[MyItem]', 'Instance Created', instance.id);

		setTimeout(() => {
			console.log('TEST METHOD====>');
			var ret = instance.invokeMethod('methodTest', 123, 222, 333, 444, 555);
			console.log('RESULT', ret);

			var x = instance.invokeMethod('xxx', 123);
			console.log('RESULT', x);
		}, 1000);

		// Signals
		instance.on('test', function(a) {
			console.log('[Main Process]', '[MyItem]', '[signal]', 'test', a);
		});

		instance.on('test2', function() {
			console.log('[Main Process]', '[MyItem]', '[signal]', 'test2');
		});

		instance.on('prop1Changed', function() {
			console.log('[Main Process]', '[MyItem]', '[signal]', 'prop1Changed');
		});
	});

	var qmlTypeSecond = brig.createType('Second', {
		property: {
			prop1: 123
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

	qmlTypeSecond.on('instance-created', function(instance) {
		console.log('[Main Process]', '[Second]', 'Instance Created', instance.id);

		// Signals
		instance.on('test', function(a) {
			console.log('[Main Process]', '[Second]', '[signal]', 'test', a);
		});

		instance.on('prop1Changed', function() {
			console.log('[Main Process]', '[Second]', '[signal]', 'prop1Changed');
		});
	});

	brig.open('application.qml', function(err, window) {
	});
});
