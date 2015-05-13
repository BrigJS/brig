
var fs = require('fs');
var BrigCore = null;

try {
	BrigCore = require('../build/Release/brig.node');
} catch(err) {}

if (!BrigCore) {
	BrigCore = require('../native/' + process.platform + '-' + process.arch + '/brig.node');
}

module.exports = BrigCore;
