#!/bin/bash
./cpplint.py --recursive . > /dev/null
if [ $? != 0 ]; then
    echo "please check your code style"
    exit 1
fi
