#!/bin/bash

set -e

cd /

git clone https://github.com/ogdf/ogdf.git ogdf_git
cd ogdf_git
git checkout 214105da97863e1d0a066157e5cc573b65b433a9
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/ogdf -GNinja
ninja install

cd /
rm -rf ogdf_git
