#!/usr/bin/env bash
# Run script from gorshochek dir using command:
#    sh scripts/run.sh
# options:
# $1              specify a path to dir contains input data or direct path to the file that
#                 is going to be transformed
# $2              specify a path to dir where transformed files are going to be stored
# $3              [optional] specify a path to config file

INPUT_PATH=$1
OUTPUT_PATH=$2
if [[ -n $3 ]]
then
  CONFIG_PATH=$3
else
  CONFIG_PATH=config.yaml
fi

sudo docker run --ipc=host --uts=host \
                -v $PWD/"${INPUT_PATH}":/gorshochek/data/input \
                -v $PWD/"${OUTPUT_PATH}":/gorshochek/data/output \
                -i -t gorshochek "${CONFIG_PATH}"

files=$(find "$OUTPUT_PATH"/* -type d)
for file in $files;
  do
    fdupes -dN -r "$file"
    transformed=$(find "$file" -type f -name "*.cpp")
    if [[ -z $transformed ]]
    then
      echo "remove all"
      rm -rf "$file"
    fi
  done