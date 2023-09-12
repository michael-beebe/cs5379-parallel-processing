#!/bin/bash


# Default number of processes
NUM_PROCESSES=2

# Path to the executable
EXECUTABLE="./bin/project1"

# MPI launcher
MPI_LAUNCHER=mpirun

# Check if the executable exists and run the program
[ ! -f "$EXECUTABLE" ] && echo "$EXECUTABLE not found. Compile first." && exit 1 \
|| $MPI_LAUNCHER -np $NUM_PROCESSES $EXECUTABLE