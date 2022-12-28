#!/bin/bash

set -e

CODE_DIR=$(cd "$(dirname "${0}")/../"; pwd)

if [[ $# == "0" ]]
then
  ${CODE_DIR}/script/_build_in_docker.sh --os-version 20.04 --static
else
  ${CODE_DIR}/script/_build_in_docker.sh $@
fi
