#!/usr/bin/env bash
BUILDDIR="automaton_build"
mkdir $BUILDDIR
echo "Compiling ..."
GUILE_LIB_FLAGS='--static' make -B
echo "Copying docs into" $BUILDDIR
cp LICENSE $BUILDDIR
cp README.md $BUILDDIR
cp -r docs $BUILDDIR
echo "Copying data assets and Scheme code into" $BUILDDIR
cp -r rules $BUILDDIR
cp -r data $BUILDDIR
cp -r scm $BUILDDIR
echo "Copying executables into" $BUILDDIR
cp automaton $BUILDDIR
echo "Copying shell scripts into" $BUILDDIR
cp run_*.sh $BUILDDIR
echo "Compressiong" $BUILDDIR
tar -cvf automaton.tar $BUILDDIR
gzip automaton.tar
echo "Cleaning up"
rm -Rf $BUILDDIR
