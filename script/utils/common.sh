#!/bin/bash

set -e

_TMP=$(cat /etc/os-release | grep "^ID=")
OS_NAME=${_TMP##*=}
echo "[COMMON] OS_NAME: ${OS_NAME}"

UTILS_DIR=$(cd "$(dirname "${0}")/utils/"; pwd)
echo "[COMMON] UTILS_DIR: ${UTILS_DIR}"
SCRIPT_DIR=$(cd "$(dirname "${0}")/"; pwd)
echo "[COMMON] SCRIPT_DIR: ${SCRIPT_DIR}"
CODE_DIR=$(cd "$(dirname "${0}")/../"; pwd)
echo "[COMMON] CODE_DIR: ${CODE_DIR}"

if [ "${OS_NAME}" = "ubuntu" ]
then
  source "${UTILS_DIR}/ubuntu/interface.sh"
  OS_VER=${UBUNTU_VERSION}
else
  echo "[COMMON] unmatch linux os: ${OS_NAME}"
  exit 1
fi
