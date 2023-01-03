#!/bin/bash

set -e

cd /

git clone https://github.com/google/glog.git glog_git
cd glog_git
git checkout a1b6164ef15cf4be3b1914f2df7125fc7ffc7ca8
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/glog  -GNinja
ninja install

cd /
rm -rf glog_git
