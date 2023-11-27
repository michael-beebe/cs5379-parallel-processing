#include "update_matrix.h"

__global__ void update_matrix(int *D, int n) {
  int k = blockIdx.x; // Current phase based on block index
  int i = threadIdx.y + blockDim.y * blockIdx.y; // Row index
  int j = threadIdx.x + blockDim.x * blockIdx.z; // Column index

  if (i < n && j < n) {
    __shared__ int kRow[MATRIX_SIZE];
    __shared__ int kCol[MATRIX_SIZE];

    // Load the k-th row and column into shared memory
    if (threadIdx.x == 0 && i < n) kCol[i] = D[i * n + k];
    if (threadIdx.y == 0 && j < n) kRow[j] = D[k * n + j];

    __syncthreads(); // Ensure loading is complete

    // Update the matrix
    if (i != j) {
      atomicMin(&D[i * n + j], kCol[i] + kRow[j]);
    }
  }
}

void run_update_matrix(int *D, int n) {
  int *dev_D;

  // Allocate memory on the device
  cudaMalloc((void**)&dev_D, n * n * sizeof(int));
  cudaMemcpy(dev_D, D, n * n * sizeof(int), cudaMemcpyHostToDevice);

  // Define grid and block sizes
  dim3 blocks(n, 1, 1); // One block per phase
  dim3 threadsPerBlock(THREADS_PER_BLOCK / n, n);

  // Launch the kernel
  for (int k = 0; k < n; ++k) {
    update_matrix<<<blocks, threadsPerBlock>>>(dev_D, n);
  }

  // Synchronize and copy back results
  cudaDeviceSynchronize();
  cudaMemcpy(D, dev_D, n * n * sizeof(int), cudaMemcpyDeviceToHost);

  // Free device memory
  cudaFree(dev_D);
}
