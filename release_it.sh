#!/bin/sh

HOST_SYSTEM=`uname -o`
HOST_ARCH=`uname -m`

svn checkout https://litesql.svn.sourceforge.net/svnroot/litesql/branches/stable release 

AM_BUILDDIR=build/autotools/$HOST_ARCH-$HOST_SYSTEM
CMAKE_BUILDDIR=build/cmake/$HOST_ARCH-$HOST_SYSTEM

mkdir -p $CMAKE_BUILDDIR
cd $CMAKE_BUILDDIR
cmake ../../../release
make test package

mkdir -p $AM_BUILDDIR
cd $AM_BUILDDIR
../../../release/configure
make distcheck


