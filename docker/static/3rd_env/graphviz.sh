#!/bin/bash

set -e

cd /
git clone https://gitlab.com/graphviz/graphviz.git graphviz_git
cd graphviz_git
git checkout 7.0.5
mkdir build
cd build
cmake .. \
  -DCMAKE_INSTALL_PREFIX=/graphviz \
  -Denable_ltdl=OFF \
  -Dwith_expat=OFF \
  -Dwith_digcola=OFF \
  -Dwith_gvedit=OFF \
  -Dwith_ortho=OFF \
  -Dwith_sfdp=OFF \
  -GNinja
ninja install

cd /
rm -rf graphviz_git
