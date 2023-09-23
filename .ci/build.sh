#!/bin/bash

source "$PWD/.ci/config.sh" ${1:-build}

echo "---- build.sh ----"
echo "BUILD_DIR: $BUILD_DIR"
echo "CC: $CC"
echo "CXX: $CXX"
echo "CXXFLAGS: $CXXFLAGS"
echo "--------------------------"

cmake --build $BUILD_DIR -j --target rvsim