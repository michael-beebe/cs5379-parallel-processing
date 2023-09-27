#!/bin/bash


# Default number of processes
NUM_PROCESSES=4

if [ $1 ]; then
  NUM_PROCESSES=$1
fi

# Path to the executable
EXECUTABLE="./bin/project2"

# MPI launcher
MPI_LAUNCHER=mpirun

# Check if the executable exists and run the program
[ ! -f "$EXECUTABLE" ] && echo "$EXECUTABLE not found. Compile first." && exit 1 \
|| $MPI_LAUNCHER -oversubscribe -np $NUM_PROCESSES $EXECUTABLE
