#!/bin/bash

source "$PWD/.ci/config.sh" ${1:-build}

echo "---- format_check.sh ----"
echo "BUILD_DIR: $BUILD_DIR"
echo "--------------------------"

cmake --build $BUILD_DIR -j --target run-clang-format

if [ $(git status --porcelain | wc -l) -eq "0" ]; then
    echo "OK"
else
    echo "BAD format"
    git restore .
    exit 1
fi