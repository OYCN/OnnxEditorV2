#! /bin/bash

set -e

if [[ -z $1 ]]
then
  echo "bin file with args[1] miss"
  exit 1
fi

if [[ -z $2 ]]
then
  echo "target dir with args[2] miss"
  exit 1
fi

export LD_LIBRARY_PATH=/qt/lib:/graphviz/lib:/graphviz/lib/graphviz:/glog/lib:${LD_LIBRARY_PATH}

deps=$( ldd $1 | awk '{if (match($3,"/")){ print $3}}' )

echo "bin file: $1"
echo "target dir: $2"
echo "libs:"

BAN_LIST=(
  "libc.so.6"
  "libdl.so.2"
  "libgcc_s.so.1"
  "libm.so.6"
  "libpcre.so.3"
  "libpthread.so.0"
  "libstdc++.so.6"
  "libz.so.1"
)

mkdir -p $2
for dep in ${deps[*]}
do
  if echo "${BAN_LIST[@]}" | grep -w "${dep##*/}" &>/dev/null; then
    echo "  ${dep}  -> skip"
  else
    echo "  ${dep}"
    cp ${dep} $2
  fi
done
