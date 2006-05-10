#!/bin/sh

NAME=studierstube-4.0

rm -f ../$NAME
ln -s `pwd` ../$NAME
pushd .. > /dev/null
tar cfjh $NAME.tar.bz2 \
--exclude ".svn" \
--exclude "*.dll" \
--exclude "*.vcproj" \
--exclude "*~" \
--exclude "build" \
--exclude "html" \
--exclude config.opts \
--exclude .sconsign \
--exclude $NAME.tar.bz2 \
$NAME
popd > /dev/null
mv ../$NAME.tar.bz2 .
rm ../$NAME
