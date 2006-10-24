#!/bin/sh

NAME=studierstube-4.0

rm -f ../$NAME
ln -s `pwd` ../$NAME
pushd .. > /dev/null
tar cfjh $NAME.tar.bz2 \
--exclude "*.dll" \
--exclude "*.o" \
--exclude "*.os" \
--exclude "*.so" \
--exclude "*.vcproj" \
--exclude "*~" \
--exclude ".svn" \
--exclude "html" \
--exclude $NAME.tar.bz2 \
--exclude .sconsign \
--exclude config.opts \
--exclude stb-4.0 \
$NAME
popd > /dev/null
mv ../$NAME.tar.bz2 .
rm ../$NAME

#--exclude "build" \
