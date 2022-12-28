#!/bin/bash

set -e

CODE_DIR=$(cd "$(dirname "${0}")/../"; pwd)

if [[ $# == "0" ]]
then
  ${CODE_DIR}/script/_build_docker.sh --os-version 20.04 --static
else
  echo "args: $@"
  ${CODE_DIR}/script/_build_docker.sh $@
fi
