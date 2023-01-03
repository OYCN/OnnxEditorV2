#!/bin/bash

set -e

cd /

git clone https://github.com/qt/qtbase.git
cd qtbase
git checkout v6.3.1
mkdir build
cd build

../configure \
    -release \
    -opensource \
    -confirm-license \
    -nomake tests \
    -nomake examples \
    -prefix /qt

ninja install

cd /
rm -rf qtbase
