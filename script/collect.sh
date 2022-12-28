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

deps=$( ldd $1 | awk '{if (match($3,"/")){ print $3}}' )

echo "bin file: $1"
echo "target dir: $2"
echo "libs:"
for dep in $deps
do
  echo "  ${dep}"
done

mkdir -p $2

cp $deps $2
cp $(find /usr 2>&1 | grep "ld-linux-.*\.so\.2" | head -n 1) $2
cp $1 $2
