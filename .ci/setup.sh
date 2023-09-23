#!/bin/bash

source "$PWD/.ci/config.sh" ${1:-build}

echo "---- setup.sh ----"
echo "BUILD_DIR: $BUILD_DIR"
echo "CC: $CC"
echo "CXX: $CXX"
echo "CXXFLAGS: $CXXFLAGS"
echo "--------------------------"

cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -B $BUILD_DIR