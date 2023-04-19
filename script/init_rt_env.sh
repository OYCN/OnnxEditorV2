#!/bin/bash

SCRIPT_DIR=$(
  cd "$(dirname "${0}")/"
  pwd
)
CODE_DIR=$(
  cd "$(dirname "${0}")/../"
  pwd
)

if [ "${OS_NAME}" = "ubuntu" ]; then
  CMD="libgl-dev libglu-dev 'libxcb*-dev' libx11-xcb-dev libxkbcommon-x11-dev libpcre2-dev libz-dev libfreetype6-dev libpng-dev libjpeg-dev libsqlite3-dev libharfbuzz-dev libb2-dev libdouble-conversion-dev"
  if [ "${OS_VER}" = "22.04" ]; then
    CMD="${CMD} libmd4c-dev"
  elif [ "${OS_VER}" = "20.04" ]; then
    CMD="${CMD} libmd4c-dev"
  elif [ "${OS_VER}" = "18.04" ]; then
    CMD="${CMD}"
  else
    echo "Unsupport OS Version: ${OS_VER}"
    exit 1
  fi
  echo "Will Install: ${CMD}"
  bash -c "sudo apt install ${CMD}"
else
  echo "Unsupport OS: ${OS_NAME}"
  exit 1
fi
