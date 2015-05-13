
var fs = require('fs');
var compareVersion = require('compare-version');

var verTable = [
	'0.10.0',
	'0.11.0',
	'0.12.0',
	'1.0.0'
]

var BrigCore = null;
try {
	BrigCore = require('../build/Release/brig.node');
} catch(err) {}

if (!BrigCore) {

	var target = null;
	for (var index in verTable) {
		var version = verTable[index];
		if (compareVersion(version, process.versions.node) != 1) {
			target = version;
		}
	}

	BrigCore = require('../native/' + process.platform + '-' + process.arch + '-' + target + '/brig.node');
}

module.exports = BrigCore;
