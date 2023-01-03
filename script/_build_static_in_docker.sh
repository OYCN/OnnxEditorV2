#!/bin/bash

set -e

CODE_DIR=$(cd "$(dirname "${0}")/../"; pwd)

BUILD_DIR=${BUILD_DIR:-"${CODE_DIR}/build"}
BUILD_TYPE=${BUILD_TYPE:-"Debug"}
CCACHE_DIR=${CCACHE_DIR:-""}
CCACHE_MAX_SIZE=${CCACHE_MAX_SIZE:-"2G"}

cd ${CODE_DIR}

if [[ -n ${CCACHE_DIR} ]]
then
  CCACHE_DIR=$(readlink -f ${CCACHE_DIR})
  echo "ccache dir: ${CCACHE_DIR}"
  ccache --set-config cache_dir="${CCACHE_DIR}"
  ccache --set-config max_size=${CCACHE_MAX_SIZE}
  export CCACHE_DIR
fi

echo "BUILD_DIR=${BUILD_DIR}"

rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${CODE_DIR}

export PKG_CONFIG_PATH=/graphviz/lib/pkgconfig/

cmake -GNinja -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -S . -B ${BUILD_DIR} \
  -DIN_DOCKER=ON \
  -DCMAKE_FIND_DEBUG_MODE=OFF \
  -DCMAKE_FIND_PACKAGE_TARGETS_GLOBAL=ON \
  -DCMAKE_MODULE_PATH=${CODE_DIR}/cmake \
  -DGRAPHVIZ_ROOT=/graphviz \
  -DCMAKE_PREFIX_PATH="/qt;/gtest;/glog;/gflag;/ogdf;/onnx;/magic_enum;/nlohmann-json;graphviz"

cmake --build ${BUILD_DIR}

if [[ -n ${CCACHE_DIR} ]]
then
  ccache -s
  ccache -x 2>/dev/null || true
  ccache -z
fi
