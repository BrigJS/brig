"use strict";

var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {
	brig.open('./application.qml', function(err, window) {
		for (var index in err)
			console.log(err[index]);
	});
});
