#!/bin/bash

DATA_DIR=data
DATASET_NAME=codeforces
DUPLICATES_FILE=output-system.txt
DATA_PATH=${DATA_DIR}/${DATASET_NAME}
DUPLICATES_STORAGE=${DATA_PATH}/duplicates.txt
ROUNDS_PATH=anti-plagiarism-datasets-master/rounds

if [ ! -d "$DATA_DIR" ]
then
  mkdir "$DATA_DIR"
fi

if [ ! -f "$DATA_DIR/$DATASET_NAME.zip" ]
then
  aws s3 cp s3://datasets.ml.labs.aws.intellij.net/codeforces-code-clone/anti-plagiarism-datasets-master.zip "$DATA_DIR/$DATASET_NAME.zip"
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

echo "Searching for labeled duplicates in files"
rounds=$(find "$DATA_PATH" -mindepth 1 -type d)
for round in $rounds
do
  output_file=$(find "$round" -type f -name "$DUPLICATES_FILE")
  if [ -n "$output_file" ]
  then
    { basename "$round"; cat "$output_file"; printf "\n"; } >> "$DUPLICATES_STORAGE"
  fi
done

python scripts/codeforces/process_duplicates_file.py --dataset_path="$DATA_PATH"

echo "Deleting not C/C++ files"
find "$DATA_PATH" -type f ! -name "*.cpp" -and ! -name "*.c" -and ! -name "*duplicates.txt" -delete