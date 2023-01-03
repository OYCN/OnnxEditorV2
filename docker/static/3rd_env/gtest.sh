#!/bin/bash

set -e

cd /

git clone https://github.com/google/googletest.git googletest
cd googletest
git checkout 4c9a3bb62bf3ba1f1010bf96f9c8ed767b363774
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/gtest -DBUILD_GMOCK=OFF  -GNinja
ninja install

cd /
rm -rf googletest
