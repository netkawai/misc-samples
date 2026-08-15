#!/bin/sh

if [ -z "$1" ]; then
    echo "usage: $0 version" 1>&2
    exit 1
fi
target_priority=100
if [ -n "$2" ]; then
	target_priority=$2
fi

if [ ! -f "/usr/bin/gcc-$1" ] || [ ! -f "/usr/bin/g++-$1" ]; then
    echo "no such version gcc/g++ installed" 1>&2
    exit 1
fi

sudo update-alternatives --install /usr/bin/cc cc "/usr/bin/gcc-$1" $target_priority
sudo update-alternatives --install /usr/bin/gcc gcc "/usr/bin/gcc-$1" $target_priority
sudo update-alternatives --install /usr/bin/g++ g++ "/usr/bin/g++-$1" $target_priority
sudo update-alternatives --install /usr/bin/c++ c++ "/usr/bin/g++-$1" $target_priority
sudo update-alternatives --install /usr/bin/gcov gcov "/usr/bin/gcov-$1" $target_priority
sudo update-alternatives --install /usr/bin/gcov-tool gcov-tool "/usr/bin/gcov-tool-$1" $target_priority
sudo update-alternatives --install /usr/bin/gcc-ar gcc-ar "/usr/bin/gcc-ar-$1" $target_priority
sudo update-alternatives --install /usr/bin/gcc-nm gcc-nm "/usr/bin/gcc-nm-$1" $target_priority
sudo update-alternatives --install /usr/bin/gcc-ranlib gcc-ranlib "/usr/bin/gcc-ranlib-$1" $target_priority
