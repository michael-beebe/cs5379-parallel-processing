# Project 1 - 
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
./run.sh
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
### Summary
- The program uses asynchronous communication (MPI_Isend and MPI_Irecv) to overlap communication and computation. This aims to achieve better performance by not making the process wait idly while sending or receiving data.
- The matrix data is divided between the two processes. The master process (pid 0) generates the entire matrix but only computes sums for the second half, while the worker process (pid 1) computes sums for the first half.
- This program MUST use exactly two processes

### Master Process (pid 0)
- Generates the first half of the data matrix.
- Asynchronously sends chunks (5 rows at a time, as per the CHUNK_SIZE) of the first half of the data to the worker process (pid 1).
- After sending a chunk, while waiting for the send to complete, it generates the second half of the data matrix and computes its row sums. This is only done once thanks to the compute_flag.
- Once all chunks are sent, the master process waits to receive the computed row sums of the first half of the data from the worker process.
- After receiving all the sums, it prints all the row sums (both halves).

### Worker Process (pid 1)
- Waits to receive a chunk of data from the master process.
- After receiving a chunk, computes the row sums for the previous chunk that was received. - This way, the process can be receiving new data and processing the previously received data concurrently.
- Once all chunks are received, computes the row sums for the last chunk.
- Sends the computed row sums of the first half of the data back to the master process.