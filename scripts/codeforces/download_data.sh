#!/bin/bash

DATA_DIR=data
DATASET_NAME=codeforces
DATA_PATH=${DATA_DIR}/${DATASET_NAME}
CLONES_FILE=output-system.txt
CLONES_STORAGE=${DATA_PATH}/clones.txt
ROUNDS_PATH=anti-plagiarism-datasets-master/rounds

LOAD_PROCESSED=false

while (( "$#" )); do
  case "$1" in
    -h|--help)
      echo "options:"
      echo "-h, --help                     show brief help"
      echo "--load_processed               pass it if you want to load already processed dataset, default false"
      exit 0
      ;;
    --load_processed*)
      LOAD_PROCESSED=true
      shift
      ;;
    *)
      echo "something went wrong"
      exit 1
  esac
done

if $LOAD_PROCESSED
then
  echo "Processed dataset is currently unavailable"
else
  if [ ! -d "$DATA_DIR" ]
  then
    mkdir "$DATA_DIR"
  fi

  if [ ! -f "$DATA_DIR/$DATASET_NAME.zip" ]
  then
    wget https://s3-eu-west-1.amazonaws.com/datasets.ml.labs.aws.intellij.net/codeforces-code-clone/anti-plagiarism-datasets-master.zip
    mv anti-plagiarism-datasets-master.zip "$DATA_DIR/$DATASET_NAME.zip"
  fi

  echo "Unzip dataset"

  unzip "$DATA_DIR/$DATASET_NAME.zip" -d $DATA_DIR

  mkdir $DATA_PATH

  zipped_files=$(find "$DATA_DIR/$ROUNDS_PATH" -name "*.zip" -type f)
  for round in $zipped_files
  do
    unzip "$round" -d "$DATA_DIR/$ROUNDS_PATH"
    round_dir="${round%.zip}"
    mv "$round_dir"/* "$DATA_PATH"/
    rm -rf "$round_dir"
    rm "$round"
  done
  rm -rf $DATA_DIR/anti-plagiarism-datasets-master

  if [ -f "$DUPLICATES_STORAGE" ]
  then
    rm "$DUPLICATES_STORAGE"
  fi

  echo "Searching for labeled clones in files"
  rounds=$(find "$DATA_PATH" -mindepth 1 -type d)
  for round in $rounds
  do
    output_file=$(find "$round" -type f -name "$CLONES_FILE")
    if [ -n "$output_file" ]
    then
      { basename "$round"; cat "$output_file"; printf "\n"; } >> "$CLONES_STORAGE"
    fi
  done

  python scripts/codeforces/process_clones_file.py --dataset_path="$DATA_PATH"

  echo "Deleting non C/C++ files"
  find "$DATA_PATH" -type f ! -name "*.cpp" -and ! -name "*.c" -and ! -name "*clones.txt" -delete

  echo "Deleting empty dirs"
  find "$DATA_PATH" -type d -empty -delete
fi