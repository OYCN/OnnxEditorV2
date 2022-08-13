#!/bin/bash

set -e

basepath=$(cd `dirname $0`; pwd)
cd $basepath
mkdir -p build_qt
cd build_qt

$basepath/3rd_party/qtbase/configure \
    -release \
    -static \
    -opensource \
    -confirm-license \
    -make libs \
    -nomake tests \
    -nomake examples \
    -prefix $basepath/3rd_party/qtbase-release

ninja install
