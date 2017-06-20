var Brig = require('../../');

var brig = new Brig();

brig.on('ready', function(brig) {
	brig.app.context.setContextProperty('helloGlobal', 'Test');
	brig.open('application.qml', function(err, window) {
	});
});
