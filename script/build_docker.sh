#!/bin/bash

set -e

source "$(cd "$(dirname "${0}")"; pwd)/utils/common.sh" > /dev/null

DOCKER_PREFIX="opluss/onnxeditor"

DEBUG="no"

debug_print () {
  if [ "${DEBUG}" = "yes" ]
  then
    echo "$@"
  fi
}

parse_args ()
{
  # from common
  #   OS_NAME
  #   OS_VER
  TAG=""
  PLATFORM=()
  UPLOAD="no"
  while [ "$#" -gt 0 ]; do
      CURRENT_OPT="$1"
      debug_print "${CURRENT_OPT}"
      case "$1" in
      --os-name)
          shift
          OS_NAME="${1}"
          debug_print "reset OS_NAME to ${OS_NAME}"
          ;;
      --os-version)
          shift
          OS_VER="${1}"
          debug_print "reset OS_NAME to ${OS_VER}"
          ;;
      -t|--tag)
          shift
          TAG="${1}"
          debug_print "set TAG to ${TAG}"
          ;;
      -p|--platform)
          shift
          PLATFORM+=("${1}")
          debug_print "set PLATFORM to ${PLATFORM[*]}"
          ;;
      -u|--upload)
          UPLOAD="yes"
          debug_print "set UPLOAD to ${UPLOAD}"
          ;;
      -d|--debug)
          DEBUG="yes"
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

CMD=(docker)

auto_fill_args () {
  # fill platform
  if [ -z "${PLATFORM[*]}" ]
  then
    PLATFORM=("$(get_os_type)/$(get_arch)")
  fi
  # not cross platform
  debug_print "PLATFORM_TAG len: ${#PLATFORM[@]}"
  debug_print "PLATFORM_TAG[0]: ${PLATFORM[0]}"
  if [ "${#PLATFORM[@]}" = "1" ] && [ "${PLATFORM[0]}" = "$(get_os_type)/$(get_arch)" ]
  then
    CMD+=(build)
    PLATFORM_TAG="$(get_arch)"
  # cross platform
  elif [ "${#PLATFORM[@]}" -gt "0" ]
  then
    CMD+=(buildx build)
    if [ "${#PLATFORM[@]}" = "1" ]
    then
      PLATFORM_TAG=${PLATFORM[0]}
      PLATFORM_TAG=${PLATFORM_TAG##*/}
    else
      PLATFORM_TAG=""
    fi
    PLATFORM_STR="${PLATFORM[*]}"
    CMD+=("--platform=${PLATFORM_STR// /,}")
  fi
  # fill TAG
  if [ -z "${TAG}" ]
  then
    TAG="${OS_NAME}-${OS_VER}-${PLATFORM_TAG}"
    CMD+=("-t ${DOCKER_PREFIX}:${TAG}")
  fi
  CMD+=("--build-arg DOCKER_BASE_IMAGE=${OS_NAME}:${OS_VER}")
  CMD+=("--build-arg DOCKER_ENV_SCRIPT=./docker/${OS_NAME}${OS_VER}.sh")
  CMD+=("-f ${CODE_DIR}/docker/linux.DockerFile ${CODE_DIR}")
}

display_args () {
  echo "OS_NAME: ${OS_NAME}"
  echo "OS_VER: ${OS_VER}"
  echo "TAG: ${TAG}"
  echo "PLATFORM: ${PLATFORM[*]}"
}

check_args () {
  if [ "${#PLATFORM[@]}" = "0" ]
  then
    echo "ERR: PLATFORM is empty"
    exit 1
  fi
  if [ -z "${TAG}" ]
  then
    echo "ERR: TAG is empty"
    exit 1
  fi
  if [ ! -f "${CODE_DIR}/docker/${OS_NAME}${OS_VER}.sh" ]
  then
    echo "ERR: ${CODE_DIR}/docker/${OS_NAME}${OS_VER}.sh not Found"
    exit 1
  fi
}

parse_args "$@"
auto_fill_args
display_args
check_args

if [ "${UPLOAD}" = "yes" ]
then
  CMD+=( "&&" docker push ${DOCKER_PREFIX}:${TAG} )
fi

echo "${CMD[*]}"

if [ "${DEBUG}" = "no" ]
then
  bash -c "${CMD[*]}"
fi
