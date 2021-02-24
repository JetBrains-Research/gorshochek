#!/usr/bin/env bash
# Run script from gorshochek dir using command:
#    sh scripts/run.sh
# options:
# $1              specify a path to dir contains input data or direct path to the file that
#                 is going to be transformed
# $2              specify a path to dir where transformed files are going to be stored
# $3              [optional] specify a path to config file

BUFFER_SIZE=100

INPUT_PATH=$1
OUTPUT_PATH=$2
if [ ! -d $OUTPUT_PATH ]
then
  mkdir "$OUTPUT_PATH"
fi

IN_BUFFER=in_buffer_$(basename "$INPUT_PATH")
if [ ! -d "$IN_BUFFER" ]
then
  mkdir -p "$IN_BUFFER"
fi

OUT_BUFFER=out_buffer_$(basename "$INPUT_PATH")
if [ ! -d "$OUT_BUFFER" ]
then
  mkdir -p "$OUT_BUFFER"
fi

LOG_BUFFER=log_buffer_$(basename "$INPUT_PATH")
if [ ! -d "$LOG_BUFFER" ]
then
  mkdir -p "$LOG_BUFFER"
fi


if [[ -n $3 ]]
then
  CONFIG_PATH=$3
else
  CONFIG_PATH=config.yaml
fi

index=0;
files=$(find "${INPUT_PATH}"/* -type f)
for file in $files
do
  echo "$file"
  cp "$file" "$IN_BUFFER"
  if ((index % BUFFER_SIZE == BUFFER_SIZE - 1))
  then
    echo "$OUT_BUFFER"
    docker run --ipc=host --uts=host \
          -v $PWD/"${IN_BUFFER}":/gorshochek/data/input \
          -v $PWD/"${OUT_BUFFER}":/gorshochek/data/output \
          -i -t gorshochek "${CONFIG_PATH}"
    logs=$(find "${OUT_BUFFER}"/* -type f -maxdepth 0 -name "*.txt")
    for log in $logs
    do
      cat "$log" >> "$LOG_BUFFER"/"$(basename "$log")"
    done
    mv "$OUT_BUFFER"/* "$OUTPUT_PATH"
    find "$IN_BUFFER" -type f -delete
  fi
  index=$((index+1))
done

docker run --ipc=host --uts=host \
      -v $PWD/"${IN_BUFFER}":/gorshochek/data/input \
      -v $PWD/"${OUT_BUFFER}":/gorshochek/data/output \
      -i -t gorshochek "${CONFIG_PATH}"
logs=$(find "${OUT_BUFFER}"/* -type f -maxdepth 0 -name "*.txt")
for log in $logs
do
  cat "$log" >> "$LOG_BUFFER"/"$(basename "$log")"
  mv "$LOG_BUFFER"/"$(basename "$log")" "$OUTPUT_PATH"
  rm "$log"
done
mv "$OUT_BUFFER"/* "$OUTPUT_PATH"

rm -rf "$IN_BUFFER"
rm -rf "$OUT_BUFFER"
rm -rf "$LOG_BUFFER"

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