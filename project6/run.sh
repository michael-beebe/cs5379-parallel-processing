#!/bin/bash

# Path to the executable
EXECUTABLE="./bin/group_5_assignment_6"


# Check if the executable exists and run the program
[ ! -f "$EXECUTABLE" ] && echo "$EXECUTABLE not found. Compile first." && exit 1 \
|| $EXECUTABLE
