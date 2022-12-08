#!/bin/bash

set -e

UBUNTU_SCRIPTS_DIR="${UTILS_DIR}/ubuntu"

_TMP=$(cat /etc/os-release | grep "VERSION_CODENAME=")
UBUNTU_NAME=${_TMP##*=}

if [ "${UBUNTU_NAME}" = "jammy" ]
then
  UBUNTU_VERSION="22.04"
elif [ "${UBUNTU_NAME}" = "focal" ]
then
  UBUNTU_VERSION="20.04"
elif [ "${UBUNTU_NAME}" = "bionic" ]
then
  UBUNTU_VERSION="18.04"
else
  echo "[UBUNTU][COMMON] unmatch ubuntu version: ${UBUNTU_NAME}"
  exit 1
fi

get_os_type () {
  echo "linux"
}

get_arch () {
  dpkg --print-architecture
}
