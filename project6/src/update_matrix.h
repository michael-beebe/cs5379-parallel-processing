#ifndef UPDATE_MATRIX_H_
#define UPDATE_MATRIX_H_

#define MATRIX_SIZE 4                    // maximum matrix size

#define BLOCKS_PER_GRID MATRIX_SIZE      // adjust as needed
#define THREADS_PER_BLOCK 256            // adjust as needed

#ifdef __cplusplus
extern "C" {
#endif

void run_update_matrix(int *D, int n);

#ifdef __cplusplus
}
#endif

#endif
