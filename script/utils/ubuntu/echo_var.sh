#!/bin/bash

set -e

echo "[UBUNTU][ECHO_VAR] UBUNTU_SCRIPTS_DIR: ${UBUNTU_SCRIPTS_DIR}"
echo "[UBUNTU][ECHO_VAR] UBUNTU_NAME: ${UBUNTU_NAME}"
echo "[UBUNTU][ECHO_VAR] UBUNTU_VERSION: ${UBUNTU_VERSION}" 
echo "[UBUNTU][ECHO_VAR] arch: $(get_arch)"
echo "[UBUNTU][ECHO_VAR] os type: $(get_os_type)"
