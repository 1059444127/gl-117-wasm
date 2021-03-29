#!/bin/sh

path="$(dirname "$1")"
TEXINPUTS="$path:" exec pdflatex -output-directory "$2" "$1"
