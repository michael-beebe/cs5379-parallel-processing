# Project 1
## Team Members
- Michael Beebe (R11772231)
- TODO: everyone else add your names

### Required Software
- MPI implementation (we are using Open MPI)
- C Compiler (such as gcc or clang)
- Make
- Bash


## Instructions
### Compile
To change the MPI wrapper to something other than `mpicc` (such as `mpich`), edit line 1 of the Makefile.
```
make
```

### Run
Specify the number of desired MPI ranks in the sript argument. If no number is specified, 4 will be used by default.
```
./run.sh <desired number of MPI ranks>
```

### Clean Build
```
make clean
```