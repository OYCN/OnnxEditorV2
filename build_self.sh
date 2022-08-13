#!/bin/bash

set -e

basepath=$(cd `dirname $0`; pwd)
cd $basepath

cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -S . -B build
cmake --build build
