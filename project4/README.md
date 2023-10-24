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
mpicc -o main main.c -lm

### Run
```
mpirun -np 2 main
```
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
The main function is responsible of doing the following:
1.	Initialize MPI. 
2.	Initialize the W0 buffer. 
3.	Initialize the W buffer. 
4.	Split the main communication into two groups (for rows and columns grouping).

![image](https://github.com/michael-beebe/cs5379-parallel-processing/assets/113784916/ed3297a3-f5c9-401d-b19f-397741877cc8)

