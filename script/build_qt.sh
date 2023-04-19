#!/bin/bash

set -e

CODE_DIR=$(
    cd "$(dirname "${0}")/../"
    pwd
)

gcc --version | head -1
cmake --version | head -1
echo "Ninja $(ninja --version)"
{ ninja --help || true; } 2>&1 | grep "run N jobs in parallel"

QT_DIR=${QT_DIR:-"${CODE_DIR}/3rd_party/qtbase"}
QT_TMP=${QT_TMP:-"${CODE_DIR}/build_qt"}
QT_DST=${QT_DST:-"${CODE_DIR}/3rd_party/qtbase-release"}

echo "QT_DIR=${QT_DIR}"
echo "QT_TMP=${QT_TMP}"
echo "QT_DST=${QT_DST}"

rm -rf ${QT_TMP}
mkdir -p ${QT_TMP}
cd ${QT_TMP}

${QT_DIR}/configure \
    -release \
    -static \
    -opensource \
    -confirm-license \
    -make libs \
    -nomake tests \
    -nomake examples \
    -prefix ${QT_DST} \
    ${@}
# -xcb \
# -system-sqlite \
# -system-pcre \
# -system-zlib \
# -system-freetype \
# -system-libpng \
# -system-libjpeg \
# -system-harfbuzz \
# -system-libb2 \
# -system-doubleconversion
# -system-libmd4c # C markdown parser

ninja install
