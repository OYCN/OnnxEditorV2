#!/bin/bash

CODE_DIR=$(cd "$(dirname "${0}")/../"; pwd)

${CODE_DIR}/cpplint.py --recursive ${CODE_DIR} > /dev/null
if [ $? != 0 ]; then
    echo "please check your code style"
    exit 1
else
    echo "cpplint pass"
fi
