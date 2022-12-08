#!/bin/bash

set -e

source "$(cd "$(dirname "${0}")"; pwd)/utils/common.sh" > /dev/null

BUILD_TYPE=${BUILD_TYPE:-"Debug"}
BUILD_DIR=${BUILD_DIR:-"${CODE_DIR}/build"}

parse_args ()
{
  while [ "$#" -gt 0 ]; do
      CURRENT_OPT="$1"
      debug_print "${CURRENT_OPT}"
      case "$1" in
      -r|--release)
          BUILD_TYPE="Release"
          ;;
      -d|--dir)
          shift
          BUILD_DIR="${1}"
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

CMD_BASE="cd /ws && ./script/build_self.sh"

CMD=(docker run --rm -it \
  -v ${CODE_DIR}:/ws \
  -v /:/hostfs \
  --env SELF_TMP=${BUILD_DIR} \
  --env BUILD_TYPE=${BUILD_TYPE} \
  opluss/onnxeditor:"${OS_NAME}-${OS_VER}-$(get_arch)" \
  bash -c "\"${CMD_BASE}\""
  )

echo "${CMD[*]}"
bash -c "${CMD[*]}"
