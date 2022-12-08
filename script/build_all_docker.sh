#!/bin/bash

source "$(cd "$(dirname "${0}")"; pwd)/utils/common.sh" > /dev/null

# PLATFORM=(
#   "linux/amd64"
#   "linux/arm64"
# )

CMD=("${SCRIPT_DIR}/build_docker.sh")

for PF in ${PLATFORM[*]}
do
  CMD+=("-p ${PF}")
done

OTHER_ARGS="${@}"

build_multi_platform_docker () {
  OS=${1}
  VER=${2}
  CMD_STR="${CMD[*]} --os-name ${OS} --os-version ${VER}"
  echo "${CMD_STR} ${OTHER_ARGS}"
  bash -c "${CMD_STR} ${OTHER_ARGS}"
}

build_multi_platform_docker "ubuntu" "22.04"
build_multi_platform_docker "ubuntu" "20.04"
build_multi_platform_docker "ubuntu" "18.04"
