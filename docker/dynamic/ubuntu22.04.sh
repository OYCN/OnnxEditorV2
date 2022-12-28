#!/bin/bash

set -e

export DEBIAN_FRONTEND=noninteractive

# Install CMake
apt update
apt install -y gcc g++ cmake ninja-build qt6-base-dev libgl-dev libglu-dev 'libxcb*-dev' libx11-xcb-dev libxkbcommon-x11-dev libpcre2-dev libz-dev libfreetype6-dev libpng-dev libjpeg-dev libsqlite3-dev libharfbuzz-dev libb2-dev libdouble-conversion-dev

rm -rf /var/lib/apt/lists/*
