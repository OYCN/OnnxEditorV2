#!/bin/bash

set -e

cd /

git clone https://github.com/nlohmann/json.git
cd json
git checkout 0e61ee8b074ecb40be6439c7470764dc6c3e0743
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/nlohmann-json -DJSON_BuildTests=OFF JSON_MultipleHeaders=OFF -GNinja
ninja install

cd /
rm -rf jsom
