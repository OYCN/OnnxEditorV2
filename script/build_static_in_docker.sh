#!/bin/bash

set -e

source "$(cd "$(dirname "${0}")"; pwd)/utils/common.sh" > /dev/null

BUILD_TYPE=${BUILD_TYPE:-"Debug"}
BUILD_DIR=${BUILD_DIR:-"/hostfs/${CODE_DIR}/build"}

parse_args ()
{
  PKG_DIR=""
  DOCKER_ARG="-it"
  CCACHE_DIR="./ccache"
  while [ "$#" -gt 0 ]; do
      CURRENT_OPT="$1"
      case "$1" in
      -r|--release)
          BUILD_TYPE="Release"
          ;;
      --debug)
          ;;
      --pkg)
          shift
          PKG_DIR="${1}"
          ;;
      -d|--dir)
          shift
          BUILD_DIR="${1}"
          ;;
      --ccache-dir)
          shift
          CCACHE_DIR="${1}"
          ;;
      --ci)
          DOCKER_ARG=""
          ;;
      *)
        UNKNOWN_OPT="yes"
        ;;
      esac
      shift
      if [ "$UNKNOWN_OPT" = "yes" ]; then
          echo "${CURRENT_OPT}: invalid command-line switch"
          exit 1
      fi
  done
}

parse_args "$@"

CMD_BASE="cd /ws && ./script/_build_static_in_docker.sh"

# PKG_CMD="
#     rm -rf ${PKG_DIR}
#     mkdir -p ${PKG_DIR}/lib
#     mkdir -p ${PKG_DIR}/bin
#     mkdir -p ${PKG_DIR}/lib/qtplugin
#     ./script/collect_safe.sh ${BUILD_DIR}/main ${PKG_DIR}/lib
#     cp ${BUILD_DIR}/main ${PKG_DIR}/bin/
#     cp ./script/_run_safe.sh ${PKG_DIR}/run.sh
#     cp /qt/lib/*.so* ${PKG_DIR}/lib
#     cp /qt/plugins/platforms/*.so ${PKG_DIR}/lib/qtplugin
# "

PKG_CMD="
    mkdir -p ${PKG_DIR}/lib
    mkdir -p ${PKG_DIR}/bin
    mkdir -p ${PKG_DIR}/lib/qtplugin
    cp ./script/_run_safe.sh ${PKG_DIR}/run.sh
    cp ${BUILD_DIR}/main ${PKG_DIR}/bin/
    cp /qt/lib/*.so* ${PKG_DIR}/lib
    cp /qt/plugins/platforms/*.so* ${PKG_DIR}/lib/qtplugin
    cp /graphviz/lib/*.so* ${PKG_DIR}/lib
    cp /graphviz/lib/graphviz/*.so* ${PKG_DIR}/lib
    cp /glog/lib/*.so* ${PKG_DIR}/lib
"

if [[ -n ${PKG_DIR} ]]
then
    CMD_BASE="${CMD_BASE} && ${PKG_CMD}"
fi

CMD=(docker run --rm ${DOCKER_ARG} \
  -v ${CODE_DIR}:/ws \
  -v /:/hostfs \
  --env BUILD_DIR=${BUILD_DIR} \
  --env BUILD_TYPE=${BUILD_TYPE} \
  --env CCACHE_DIR=${CCACHE_DIR} \
  opluss/onnxeditor-3rd:cmake_main \
  bash -c "\"${CMD_BASE}\""
  )

echo "${CMD[*]}"
bash -c "${CMD[*]}"
