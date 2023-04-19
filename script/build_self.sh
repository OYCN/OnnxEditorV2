#!/bin/bash

set -e

CODE_DIR=$(
  cd "$(dirname "${0}")/../"
  pwd
)

SELF_DIR=${SELF_DIR:-"${CODE_DIR}"}
SELF_TMP=${SELF_TMP:-"${CODE_DIR}/build"}
QT_LIB=${QT_LIB:-"${CODE_DIR}/3rd_party/qtbase-release"}
BUILD_TYPE=${BUILD_TYPE:-"Debug"}
CCACHE_DIR=${CCACHE_DIR:-""}
CCACHE_MAX_SIZE=${CCACHE_MAX_SIZE:-"2G"}

if [[ -z ${STATIC} ]]; then
  SUFFIX="-DCMAKE_INSTALL_PREFIX=/usr"
else
  SUFFIX="-DQT_PREFIX_PATH=${QT_LIB}"
fi

if [[ -n ${CCACHE_DIR} ]]; then
  cd ${SELF_DIR}
  CCACHE_DIR=$(readlink -f ${CCACHE_DIR})
  echo "ccache dir: ${CCACHE_DIR}"
  ccache --set-config cache_dir="${CCACHE_DIR}"
  ccache --set-config max_size=${CCACHE_MAX_SIZE}
  export CCACHE_DIR
fi

echo "SELF_DIR=${SELF_DIR}"
echo "SELF_TMP=${SELF_TMP}"
echo "QT_LIB=${QT_LIB}"
echo "SUFFIX=${SUFFIX}"

# rm -rf ${SELF_TMP}
mkdir -p ${SELF_TMP}
cd ${SELF_DIR}

cmake -GNinja -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -S . -B ${SELF_TMP} ${SUFFIX}
cmake --build ${SELF_TMP}

if [[ -n ${CCACHE_DIR} ]]; then
  ccache -s
  ccache -x 2>/dev/null || true
  ccache -z
fi
