#!/bin/bash

DATA_DIR=data

if [ ! -d "$DATA_DIR" ]
then
  mkdir "$DATA_DIR"
fi

SRC=codeforces_augmented
SRC_PATH="$DATA_DIR"/"$SRC"

DST=codeforces_reduced
DST_PATH="$DATA_DIR"/"$DST"

CLONES_FILE=clones.txt
CLONES_FILE_PATH="$SRC_PATH"/"$CLONES_FILE"

DUPLICATES_FILE=duplicates.txt
DUPLICATES_FILE_PATH="$DST_PATH"/"$DUPLICATES_FILE"

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
  if [ ! -d "$DATA_DIR"/"$SRC" ]
  then
    if [ ! -f "$DATA_DIR"/"$SRC".zip ]
    then
      wget "${ACTUAL_DATA}"
      mv actual.zip "$DATA_DIR"/"$SRC".zip
    fi
    unzip "$DATA_DIR"/"$SRC".zip -d "$DATA_DIR"
    mv "$DATA_DIR"/actual "$DATA_DIR"/"$SRC"
  fi

  echo "Deleting non .cpp files"
  find "$SRC_PATH" -type f ! -name "*.cpp" -and ! -name "*clones.txt" -delete

  echo "Copy dirs with transformed files"
  if [ ! -d "$DST_PATH" ]
  then
    mkdir "$DST_PATH"
  fi
  find "$SRC_PATH"/* -type d -mindepth 1 -maxdepth 1 -exec mv {} "$DST_PATH" \;

  if [ -f "$CLONES_FILE_PATH" ]
  then
    echo "Merging clones"
    python scripts/postprocessing/merge_clones.py \
           --dataset_path="$DST_PATH" \
           --clones_file_path="$CLONES_FILE_PATH"
  fi

  echo "Mining duplicates"
  fdupes -r "$DST_PATH" > "$DUPLICATES_FILE_PATH"

  echo "Deleting duplicates"
  python scripts/postprocessing/delete_duplicates.py \
         --duplicates_file_path="$DUPLICATES_FILE_PATH"

  echo "Deleting empty dirs"
  find "$DST_PATH" -type d -empty -delete
fi