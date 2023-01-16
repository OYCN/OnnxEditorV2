#!/bin/bash

source "$(cd "$(dirname "${0}")"; pwd)/utils/common.sh" > /dev/null

cd ${CODE_DIR}

git fetch origin

change_list=$(git diff origin/master --name-only)

for file in $change_list;do
  if test -f "$file"; then  
    if [ "${file##*.}"x = "cpp"x ]||[ "${file##*.}"x = "cc"x ]||[ "${file##*.}"x = "cu"x ]||[ "${file##*.}"x = "h"x ]||[ "${file##*.}"x = "hpp"x ]||[ "${file##*.}"x = "hxx"x ];then
      echo "Formating $file"
      clang-format -i --style=file $file
    fi
  fi
done

if [ $(git status -s | grep "^ M " | wc -l) -ne 0 ]
then
  echo "ERROR: please check the modified files"
  exit 1
else
  echo "Pass!"
fi
