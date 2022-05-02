#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters."
    echo "Usage: ./run.sh <path_to_executable_eco32_file>"
    exit 1
fi

SCRIPTDIR=$(dirname "$0")
BIN="$SCRIPTDIR"/bin

display_exists=false

# Check if a display exists
if xhost >& /dev/null ; then display_exists=true
else display_exists=false ; fi

if [ "$display_exists" = true ] ; then
    $BIN/sim -x -l $1 -s 1 -t 0 -g 
else
    if ! command -v xvfb-run > "/dev/null"; then
        echo "ERROR: There is no display available and xvfb-run could not be found."
        echo "xvfb-run can be installed via apt with 'sudo apt install xvfb'"
        exit 1
    fi
    xvfb-run $BIN/sim -x -l $1 -s 1 -t 0
fi
