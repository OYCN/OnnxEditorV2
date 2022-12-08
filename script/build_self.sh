#!/bin/bash

set -e

CODE_DIR=$(cd "$(dirname "${0}")/../"; pwd)

SELF_DIR=${SELF_DIR:-"${CODE_DIR}"}
SELF_TMP=${SELF_TMP:-"${CODE_DIR}/build"}
QT_LIB=${QT_LIB:-"${CODE_DIR}/3rd_party/qtbase-release"}
BUILD_TYPE=${BUILD_TYPE:-"Debug"}

echo "SELF_DIR=${SELF_DIR}"
echo "SELF_TMP=${SELF_TMP}"
echo "QT_LIB=${QT_LIB}"

# rm -rf ${SELF_TMP}
mkdir -p ${SELF_TMP}
cd ${SELF_DIR}

cmake -GNinja -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -S . -B ${SELF_TMP} -DQT_PREFIX_PATH=${QT_LIB}
cmake --build ${SELF_TMP}
