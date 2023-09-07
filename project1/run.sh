#!/bin/bash


# Default number of processes
NUM_PROCESSES=4

# Check the number of arguments and if an argument is provided
if [ "$#" -gt 1 ]; then
  echo "Usage: ./run.sh [number_of_processes]"
  exit 1
elif [ "$#" -eq 1 ]; then
  NUM_PROCESSES=$1
fi

# Path to the executable
EXECUTABLE="./bin/project1"

# MPI launcher
MPI_LAUNCHER=mpirun

# Check if the executable exists and run the program
[ ! -f "$EXECUTABLE" ] && echo "$EXECUTABLE not found. Compile first." && exit 1 \
|| $MPI_LAUNCHER -np $NUM_PROCESSES $EXECUTABLE