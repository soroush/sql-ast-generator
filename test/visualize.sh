#!/bin/bash

# Check for graphviz
if ! [ -x "$(command -v dot)" ]; then
  echo 'Error: graphviz is not installed.' >&2
  exit 1
fi

find . -name "*.dot" | while read fname; do
    pngfile="${fname%.dot}.png"
    echo "$pngfile"
    dot -Tpng $fname > $pngfile
done