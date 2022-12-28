#!/bin/bash

set -e

source "$(cd "$(dirname "${0}")"; pwd)/utils/common.sh" > /dev/null

BUILD_TYPE=${BUILD_TYPE:-"Debug"}
BUILD_DIR=${BUILD_DIR:-"/hostfs/${CODE_DIR}/build"}

parse_args ()
{
  DOCKER_MODE="dynamic"
  PKG_DIR=""
  DOCKER_ARG="-it"
  CCACHE_DIR=""
  while [ "$#" -gt 0 ]; do
      CURRENT_OPT="$1"
      case "$1" in
      --os-name)
          shift
          OS_NAME="${1}"
          ;;
      --os-version)
          shift
          OS_VER="${1}"
          ;;
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
      --static)
          DOCKER_MODE="static"
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

CMD_BASE="cd /ws && ./script/build_self.sh"

if [[ ${DOCKER_MODE} == "static" ]]
then
    PKG_CMD="./script/collect.sh ${BUILD_DIR}/main ${PKG_DIR} && cp ./script/_run.sh ${PKG_DIR}/run.sh"
else
    PKG_CMD="cd ${BUILD_DIR} && DESTDIR=/hostfs/${CODE_DIR}/${PKG_DIR} ninja install"
fi

if [[ -n ${PKG_DIR} ]]
then
    CMD_BASE="${CMD_BASE} && ${PKG_CMD}"
fi

CMD=(docker run --rm ${DOCKER_ARG} \
  -v ${CODE_DIR}:/ws \
  -v /:/hostfs \
  --env SELF_TMP=${BUILD_DIR} \
  --env BUILD_TYPE=${BUILD_TYPE} \
  --env CCACHE_DIR=${CCACHE_DIR} \
  opluss/onnxeditor:"${DOCKER_MODE}-${OS_NAME}-${OS_VER}-$(get_arch)" \
  bash -c "\"${CMD_BASE}\""
  )

echo "${CMD[*]}"
bash -c "${CMD[*]}"
