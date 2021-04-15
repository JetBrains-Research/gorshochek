#!/bin/bash

SRC=actual
DST=reduced

echo "Deliting non .cpp files"
find "$SRC" -type f -not -name "*.cpp" -exec rm {} \;
echo "Deliting non .cpp files"
find "$SRC"/* -type d -mindepth 1 -maxdepth 1 -exec cp -r {} "$DST" \;
echo "Mining duplicates"
fdupes -r "$DST" | > "dups.txt"
echo "Deliting duplicates"
python scripts/postprocessing/filter_duplicates.py
echo "Deliting empty dirs"
find "$DST" -type d -empty -delete