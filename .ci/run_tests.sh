#!/bin/bash

source "$PWD/.ci/config.sh" ${1:-build}

echo "---- run_tests.sh ----"
echo "BUILD_DIR: $BUILD_DIR"
echo "CC: $CC"
echo "CXX: $CXX"
echo "CXXFLAGS: $CXXFLAGS"
echo "--------------------------"

echo "---- run mem_gtests ----"
cmake --build ${BUILD_DIR} --target mem_gtests