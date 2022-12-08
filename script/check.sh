#!/bin/bash

source "$(cd "$(dirname "${0}")"; pwd)/env/common.sh" > /dev/null

${CODE_DIR}/cpplint.py --recursive ${CODE_DIR} > /dev/null
if [ $? != 0 ]; then
    echo "please check your code style"
    exit 1
fi
