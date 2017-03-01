
var fs = require('fs');
var os = require('os');

var BrigCore = null;
try {
	BrigCore = require(__dirname + '/../build/Release/brig.node');
} catch(err) {}

if (!BrigCore) {

	var apiVer = process.versions.modules;
	var arch = process.arch;

	// Support ARM architecture
	if (arch == 'arm') {
		arch += 'v' + process.config.variables.arm_version;
		if (os.endianness() == 'LE') {
			arch += 'l';
		} else {
			arch += 'b';
		}
	}

	if (!BrigCore) {
		try {
			// Try to use the last version
			BrigCore = require('../native/' + process.platform + '-' + arch + '-' + apiVer + '/brig.node');
		} catch(err) {
			throw new Error('Please use \'node-gyp rebuild\' to compile binary by yourself.');
		}
	}
}

module.exports = BrigCore;
