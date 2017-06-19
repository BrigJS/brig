
var child_process = require('child_process');

if (process.platform === 'darwin') {
	console.log('Downloading Darwin backend ...');
	var downloadBackend = child_process.spawn('npm', [ 'install', 'qt-darwin' ], { stdio: 'inherit' });
	downloadBackend.on('error', function() {
		console.error('Failed to download qt-darwin, using Qt installed on your system instead.');
	});

	downloadBackend.on('exit', function(code) {
		process.exit(code);
	});
}
