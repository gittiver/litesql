#!/bin/sh

HOST_SYSTEM=`uname -s`
HOST_ARCH=`uname -m`

AM_BUILDDIR=build/autotools/$HOST_ARCH-$HOST_SYSTEM
CMAKE_BUILDDIR=build/cmake/$HOST_ARCH-$HOST_SYSTEM

mkdir -p $CMAKE_BUILDDIR
cd $CMAKE_BUILDDIR
cmake -D WITH_DOC:bool=ON -D WITH_MYSQL:bool=ON -D WITH_SQLITE:bool=ON -D WITH_TEST:bool=ON ../../..
make all test package
cd ../../..

mkdir -p $AM_BUILDDIR
cd $AM_BUILDDIR
../../../configure MYSQL_CONFIG=mysql_config5
make distcheck
