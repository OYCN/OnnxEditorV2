#!/bin/bash

if [ -z "$APPDIR" ]; then
  APPDIR="$(dirname "$(readlink -f "${0}")")"
fi

LD_BIN=$(find "${APPDIR}" | grep "ld-linux-.*\.so\.2" | head -n 1)
export QTDIR=${APPDIR}/lib
export QT_QPA_PLATFORM_PLUGIN_PATH=${APPDIR}/lib/qtplugin
export LD_LIBRARY_PATH=${APPDIR}/lib:$LD_LIBRARY_PATH
${LD_BIN} ${APPDIR}/bin/main $@

# LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ${APPDIR}/main
