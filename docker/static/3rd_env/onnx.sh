#!/bin/bash

set -e

cd /

git clone https://github.com/onnx/onnx.git onnx_git
cd onnx_git
git checkout f0d824c0ead2825f7c24790c05f985ad1fb909f2
mkdir build
cd build
cmake .. -GNinja -DCMAKE_INSTALL_PREFIX=/onnx -DONNX_GEN_PB_TYPE_STUBS=OFF
ninja install 

cd /
rm -rf onnx_git
