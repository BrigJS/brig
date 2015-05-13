#!/bin/bash

OUT=build/src
VERSIONLINE=`qmake -query | grep QT_VERSION`
QTVERSION=${VERSIONLINE##*:}
HEADERLINE=`qmake -query | grep QT_INSTALL_HEADERS`
FILES="
	${HEADERLINE##*:}/QtGui/$QTVERSION/QtGui/qpa/qwindowsysteminterface.h
	${HEADERLINE##*:}/QtCore/$QTVERSION/QtCore/private/qmetaobjectbuilder_p.h
"

mkdir -p $OUT/internal

for file in $FILES; do

	if [ -e "$file" ]; then
			basepath=$(basename $file)
			out=$OUT/internal/"$basepath"
			cp -a "$file" $out
			echo "$out"
	fi

done
