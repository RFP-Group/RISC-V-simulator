#!/bin/bash

source "$PWD/.ci/config.sh" ${1:-build}

echo "---- tidy_check.sh ----"
echo "BUILD_DIR: $BUILD_DIR"
echo "--------------------------"

cmake --build $BUILD_DIR -j --target run-clang-tidy

if [ $(git status --porcelain | wc -l) -eq "0" ]; then
    echo "OK"
else
    echo "BAD clang-tidy"
    git restore .
    exit 1
fi