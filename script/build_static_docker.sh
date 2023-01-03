#!/bin/bash

set -e

CODE_DIR=$(cd "$(dirname "${0}")/../"; pwd)

cd ${CODE_DIR}

DOCKER_PREFIX="opluss/onnxeditor-3rd"

build_docker_as () {
  if [[ -z ${1} ]]
  then
    echo "script needed with args[1] miss"
    exit 1
  fi
  if [[ -z ${2} ]]
  then
    BASE_IMG="${DOCKER_PREFIX}:base"
  else
    BASE_IMG="${2}"
  fi
  if [[ -z ${3} ]]
  then
    DOCKER_FILE="${CODE_DIR}/docker/static/3rd.DockerFile"
  else
    DOCKER_FILE="${3}"
  fi
  echo "=========="
  echo "for:         ${DOCKER_PREFIX}:${1}"
  echo "CODE_DIR:    ${CODE_DIR}"
  echo "base:        ${BASE_IMG}"
  echo "docker file: ${DOCKER_FILE}"
  echo "sc:          ${CODE_DIR}/docker/static/3rd_env/${1}.sh"
  echo "=========="
  docker pull ${DOCKER_PREFIX}:${1} || true
  docker build ${CODE_DIR} -f ${DOCKER_FILE} \
    --build-arg DOCKER_3RD_PREFIX=${DOCKER_PREFIX} \
    --build-arg DOCKER_BASE_IMAGE=${BASE_IMG} \
    --build-arg DOCKER_ENV_SCRIPT=./docker/static/3rd_env/${1}.sh \
    -t ${DOCKER_PREFIX}:${1}
  docker push ${DOCKER_PREFIX}:${1}
}

build_docker_as "base" "ubuntu:20.04"
build_docker_as "nlohmann-json"
build_docker_as "magic_enum"
build_docker_as "gflag"
build_docker_as "gtest"
build_docker_as "glog"
build_docker_as "graphviz"
build_docker_as "ogdf"
build_docker_as "onnx"
build_docker_as "qt"
build_docker_as "cmake_main" "ubuntu:20.04" "${CODE_DIR}/docker/static/main.DockerFile"
