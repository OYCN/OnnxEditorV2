#!/bin/bash

set -e

cd /

git clone https://github.com/gflags/gflags.git gflags_git
cd gflags_git
git checkout a738fdf9338412f83ab3f26f31ac11ed3f3ec4bd
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/gflag  -GNinja
ninja install

cd /
rm -rf gflags_git
