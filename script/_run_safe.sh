#!/bin/bash

if [ -z "$APPDIR" ] ; then
  path="$(dirname "$(readlink -f "${0}")")"
  while [[ "$path" != "" && ! -e "$path/$1" ]]; do
    path=${path%/*}
  done
  APPDIR="$path"
fi

echo ${APPDIR}

cd ${APPDIR}

LD_BIN=$(find . | grep "ld-linux-.*\.so\.2" | head -n 1)
echo ${LD_BIN}
export QTDIR=${APPDIR}/lib
export QT_QPA_PLATFORM_PLUGIN_PATH=${APPDIR}/lib/qtplugin
export LD_LIBRARY_PATH=${APPDIR}/lib:$LD_LIBRARY_PATH
${LD_BIN} ${APPDIR}/bin/main

# LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH ${APPDIR}/main
