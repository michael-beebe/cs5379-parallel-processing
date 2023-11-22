#ifndef UPDATE_MATRIX_H_
#define UPDATE_MATRIX_H_

#define MAX_N 1024             // maximum matrix size
#define THREADS_PER_BLOCK 256  // adjust as needed

void run_update_matrix(int *D, int n);

#endif

