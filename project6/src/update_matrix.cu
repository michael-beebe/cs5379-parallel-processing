#include "update_matrix.h"

__global__ void update_matrix(int *D, int n) {
  int k = blockIdx.x; // Current phase based on block index
  int i = threadIdx.y + blockDim.y * blockIdx.y; // Row index
  int j = threadIdx.x + blockDim.x * blockIdx.z; // Column index

  // Check bounds
  if (i < n && j < n && k < n) {
    // Copy the k-th row and column to shared memory
    __shared__ int hbuf[MATRIX_SIZE];
    __shared__ int vbuf[MATRIX_SIZE];

    if (threadIdx.x == 0) vbuf[i] = D[i * n + k];
    if (threadIdx.y == 0) hbuf[j] = D[k * n + j];

    __syncthreads(); // Ensure all threads have written to shared memory

    // Update the matrix D
    if (i != j) {
      D[i * n + j] = min(D[i * n + j], vbuf[i] + hbuf[j]);
    }
  }
}

void run_update_matrix(int *D, int n) {
  int *dev_D;

  // Allocate memory on the device
  cudaMalloc((void**)&dev_D, n * n * sizeof(int));
  cudaMemcpy(dev_D, D, n * n * sizeof(int), cudaMemcpyHostToDevice);

  // Calculate dimensions for blocks and threads
  dim3 blocks(BLOCKS_PER_GRID, BLOCKS_PER_GRID, n); // Using 3D grid for phases and matrix rows/columns
  dim3 threadsPerBlock((THREADS_PER_BLOCK / BLOCKS_PER_GRID), (THREADS_PER_BLOCK / BLOCKS_PER_GRID));

  // Launch kernel
  update_matrix<<<blocks, threadsPerBlock>>>(dev_D, n);

  // Synchronize device
  cudaDeviceSynchronize();

  // Copy result back to host
  cudaMemcpy(D, dev_D, n * n * sizeof(int), cudaMemcpyDeviceToHost);

  // Free device memory
  cudaFree(dev_D);
}
