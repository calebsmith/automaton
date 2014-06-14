#!/usr/bin/env bash
BUILDDIR="game_of_life"
mkdir $BUILDDIR
echo "Compiling ..."
make
echo "Copying docs into" $BUILDDIR
cp LICENSE $BUILDDIR
cp README.md $BUILDDIR
cp -r docs $BUILDDIR
echo "Copying data assets and Scheme code into" $BUILDDIR
cp -r rules $BUILDDIR
cp -r data $BUILDDIR
cp -r scm $BUILDDIR
echo "Copying executables into" $BUILDDIR
cp gameoflife $BUILDDIR
echo "Copying shell scripts into" $BUILDDIR
cp run_*.sh $BUILDDIR
echo "Compressiong" $BUILDDIR
tar -cvf gameoflife.tar $BUILDDIR
gzip gameoflife.tar
echo "Cleaning up"
rm -Rf $BUILDDIR
