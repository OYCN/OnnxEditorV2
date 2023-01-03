#!/bin/bash

set -e

cd /

git clone https://github.com/Neargye/magic_enum.git magic_enum_git
cd magic_enum_git
git checkout f4ebb4f185ce956bf50b93acbef1516030ecdb36
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/magic_enum -DMAGIC_ENUM_OPT_BUILD_EXAMPLES=OFF -DMAGIC_ENUM_OPT_BUILD_TESTS=OFF -GNinja
ninja install

cd /
rm -rf magic_enum_git
