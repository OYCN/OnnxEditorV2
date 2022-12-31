#!/bin/bash

set -e

export DEBIAN_FRONTEND=noninteractive

# Install CMake
apt update
apt install -y gpg wget

wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main" | tee /etc/apt/sources.list.d/kitware.list >/dev/null
apt-get update
rm /usr/share/keyrings/kitware-archive-keyring.gpg
apt-get install -y kitware-archive-keyring
apt-get install -y cmake

apt install -y gcc g++ ninja-build ccache
apt install -y libgl-dev libglu-dev 'libxcb*-dev' libx11-xcb-dev libxkbcommon-x11-dev libpcre2-dev libz-dev libfreetype6-dev libpng-dev libjpeg-dev libsqlite3-dev libharfbuzz-dev libb2-dev libdouble-conversion-dev
apt install -y libmd4c-dev
# graphviz
apt install -y bison flex git

rm -rf /var/lib/apt/lists/*
