#!/bin/bash

set -e

export DEBIAN_FRONTEND=noninteractive

apt update

apt install -y wget gpg
apt install -y libgflags-dev libgoogle-glog-dev libgtest-dev ccache ninja-build gcc g++ libprotobuf-dev protobuf-compiler
apt install -y libgl-dev libglu-dev 'libxcb*-dev' libx11-xcb-dev libxkbcommon-x11-dev libpcre2-dev libz-dev libfreetype6-dev libpng-dev libjpeg-dev libsqlite3-dev libharfbuzz-dev libb2-dev libdouble-conversion-dev libmd4c-dev

wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
echo "deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main" | tee /etc/apt/sources.list.d/kitware.list >/dev/null
apt-get update
rm /usr/share/keyrings/kitware-archive-keyring.gpg
apt-get install -y kitware-archive-keyring
apt-get install -y cmake

rm -rf /var/lib/apt/lists/*
