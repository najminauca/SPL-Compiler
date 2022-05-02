#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters."
    echo "Usage: ./compile.sh <path_to_spl_file>"
    exit 1
fi

spl_file_path=$1
without_extension=${spl_file_path%.*}

SCRIPTDIR=$(dirname "$0")
BIN="$SCRIPTDIR"/bin
LIB="$SCRIPTDIR"/lib

function fail() {
    echo "$1"
    exit 1
}

$BIN/refspl --ershov --doWhile $spl_file_path $without_extension.s || fail "Compilation failed"
$BIN/as -o $without_extension.o $without_extension.s || fail "Assembly failed"
$BIN/ld -s $LIB/stdalone.lnk -L$LIB -o $without_extension.x $LIB/start.o $without_extension.o -lsplrts  || fail "Linking failed"
$BIN/load $without_extension.x $without_extension.bin  || fail "Loading failed"
