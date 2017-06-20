{
	'targets': [
	{
		'target_name': '<(module_name)',
		'sources': [
			'<!@(tools/genmoc.sh)',
			'src/brig.cpp',
			'src/utils.cpp',
			'src/eventdispatcher/eventdispatcher.cpp',
			'src/QApplication.cpp',
			'src/eventloop.cpp',
			'src/callback.cpp',
			'src/signal_handler.cpp',
			'src/QmlEngine.cpp',
			'src/QmlComponent.cpp',
			'src/QmlContext.cpp',
			'src/QuickItem.cpp',
			'src/DynamicQMetaObjectBuilder.cpp',
			'src/DynamicQObject.cpp',
			'src/QmlTypeBuilder.cpp'
		],
		'include_dirs': [
			"<!(node -e \"require('nan')\")"
		],
		'product_dir': '<(module_path)',
		'conditions': [
			['OS=="linux"', {
				'sources': [
					'<!@(tools/internal.sh)',
				],
				'cflags': [
					'-std=c++11',
					'-fPIC',
					'-I./src',
					'<!@(pkg-config --cflags Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				],
				'ldflags': [
					'<!@(pkg-config --libs-only-L --libs-only-other Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				],
				'libraries': [
					'<!@(pkg-config --libs-only-l Qt5Core Qt5Quick Qt5Qml Qt5Multimedia)'
				]
			}],      
			['OS=="mac"', {
				'sources': [
					'<!@(tools/mac-config.sh --internal)',
					'src/eventdispatcher/platform/mac.mm'
				],
				'xcode_settings': {
					'LD_RUNPATH_SEARCH_PATHS': [
						'@excutable_path/node_modules/qt-darwin/Frameworks',
						'@loader_path/node_modules/qt-darwin/Frameworks',
						'@excutable_path/../node_modules/qt-darwin/Frameworks',
						'@loader_path/../node_modules/qt-darwin/Frameworks',
						'@excutable_path/../../node_modules/qt-darwin/Frameworks',
						'@loader_path/../../node_modules/qt-darwin/Frameworks'
					],
					'OTHER_CPLUSPLUSFLAGS': [
						'-stdlib=libc++',
						'-std=c++11',
						'-mmacosx-version-min=10.7',
						'-Wno-inconsistent-missing-override',
						'-Woverloaded-virtual',
						'<!@(tools/mac-config.sh --cflags)'
					],
					'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
				},
				'defines': [
					'__MACOSX_CORE__'
				],
				'include_dirs': [
					'build/src',
					'<!@(tools/mac-config.sh --include-dirs QtGui QtCore QtQuick QtQml QtMultimedia QtWidgets QtQuickControls2)'
				],
				'libraries': [
					'-undefined dynamic_lookup',
					'<!@(tools/mac-config.sh --libs QtGui QtCore QtQuick QtQml QtMultimedia QtWidgets QtQuickControls2)'
				]
			}]
		]
	}
	]
}
