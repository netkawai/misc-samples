#!/bin/sh

if [ -z "$1" ]; then
    echo "usage: $0 version" 1>&2
    exit 1
fi

if [ ! -f "/usr/bin/gcc-$1" ] || [ ! -f "/usr/bin/g++-$1" ]; then
    echo "no such version gcc/g++ installed" 1>&2
    exit 1
fi

update-alternatives --set gcc "/usr/bin/gcc-$1"
update-alternatives --set g++ "/usr/bin/g++-$1"
update-alternatives --set cc  "/usr/bin/gcc-$1"
update-alternatives --set c++ "/usr/bin/g++-$1"
update-alternatives --set gcov "/usr/bin/gcov-$1"
update-alternatives --set gcov-tool "/usr/bin/gcov-tool-$1" 
update-alternatives --set gcc-ar "/usr/bin/gcc-ar-$1"
update-alternatives --set gcc-nm "/usr/bin/gcc-nm-$1" 
update-alternatives --set gcc-ranlib "/usr/bin/gcc-ranlib-$1" 
