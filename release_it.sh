#!/bin/sh

HOST_SYSTEM=`uname -s`
HOST_ARCH=`uname -m`

AM_BUILDDIR=build/autotools/$HOST_ARCH-$HOST_SYSTEM
CMAKE_BUILDDIR=build/cmake/$HOST_ARCH-$HOST_SYSTEM
STARTDIR=`pwd`

if [ "$HOST_SYSTEM" = "Linux" ] 
then MYSQL_CONFIG=mysql_config
else MYSQL_CONFIG=/opt/local/lib/mysql56/bin/mysql_config
fi

mkdir -p $CMAKE_BUILDDIR

cd $CMAKE_BUILDDIR
rm CMakeCache.txt
cmake -D WITH_DOCS:bool=ON -D WITH_MYSQL:bool=ON -D WITH_SQLITE:bool=ON -D LITESQL_WITH_TESTS:bool=ON -D LITESQL_WITH_UI:bool=ON $STARTDIR
cmake --build . --target all --target test 
cmake --build . --target package 
cmake --build . --target dist
#cmake --build . --target package_source
cd $STARTDIR

mkdir -p $AM_BUILDDIR
cd $AM_BUILDDIR
$STARTDIR/configure MYSQL_CONFIG=$MYSQL_CONFIG
make distcheck
cd $STARTDIR
