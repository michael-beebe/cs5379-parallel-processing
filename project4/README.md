# Project 2 - Group 5
## Team Members
| Group Member                      | R# |
|---------------------------|-----------|
| Michael Beebe             | R11772231 |
| Diego Salas Noain         | R11794236 |
| Bandar Alkhalil           | R11836831 |
| Yongjian Zhao             | R11915830 |
| Denish Otieno             | R11743138 |
| Shiva Kumar Neekishetty   | R11842757 |

## Required Software
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
```
./run.sh <desired number of processes>
```
The default number of processes is 4. You can change this by passing a command line argument when executing `run.sh`.

If you get an error saying "permission denied", run
```
chmod +x run.sh
```
then rerun `./run.sh`

### Clean Build
```
make clean
```

## Code Breakdown
The idea is that we divide processors into different row_group/col_group according to the rank. Then for each iteration k, we broadcast the corresponding row/col within its row_group/col_group, then calculate the min value.

### generateRandomElements()
- Generate random elements for each submatrix
- The submatrix on diagonal (row_rank == col_rank) will be 0
- Other values will be random number between 1-9, or a large number INTINFINITY

### print_subMatrix()
- Print value of each sub matrix

### shortestPath()
- For each k, determine row_handeler and col_handler (k/chunk) which represents the row/col needs to be broadcast to rowgroups/colgroups
- Broadcast the corresponding row/col within rowgroup/colgroup

### main()
-	Initialize MPI. 
-	Initialize the W0 buffer. 
-	Initialize the W buffer. 
-	Split the main communication into two groups (for rows and columns grouping).

![image](https://github.com/michael-beebe/cs5379-parallel-processing/assets/113784916/ed3297a3-f5c9-401d-b19f-397741877cc8)

