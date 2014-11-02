#!/bin/bash

QT_LIBS_PATH=`qmake -query QT_INSTALL_LIBS`

if [ "$1" == "--include-dirs" ]; then
	shift 1

	for i in $@; do
		INCPATH=$QT_LIBS_PATH/${i}.framework/Versions/Current/Headers

		if [ -d "$INCPATH" ]; then
			echo $INCPATH
		fi
	done

elif [ "$1" == "--cflags" ]; then

	echo -F$QT_LIBS_PATH

elif [ "$1" == "--libs" ]; then
	shift 1

	for i in $@; do
		echo -framework $i
	done

elif [ "$1" == '--internal' ]; then
	OUT=build/src
	QTVERSION=`qmake -query QT_VERSION`
	FILES=$QT_LIBS_PATH/QtGui.framework/Versions/Current/Headers/$QTVERSION/QtGui/qpa/qwindowsysteminterface.h

	mkdir -p $OUT/internal

	for file in $FILES; do

		if [ -e "$file" ]; then
			basepath=$(basename $file)
			out=$OUT/internal/"$basepath"
			cp -a "$file" $out
			echo "$out"
		fi

	done
fi
