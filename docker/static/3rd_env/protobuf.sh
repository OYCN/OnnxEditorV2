#!/bin/bash

set -e

cd /

git clone https://github.com/protocolbuffers/protobuf.git protobuf_git
cd protobuf_git
git checkout cac9765af0ace57ce00b6ea07b8829339a622b1d
git submodule init
git submodule update --init --recursive
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/protobuf -Dprotobuf_BUILD_TESTS=OFF -GNinja
ninja install

cd /
rm -rf protobuf_git
