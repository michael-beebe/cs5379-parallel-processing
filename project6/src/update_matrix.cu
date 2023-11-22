__global__ void update_matrix() {
  int k = blockIdx.x; // Current phase based on block index
  int i = threadIdx.y + blockDim.y * blockIdx.y; // Row index
  int j = threadIdx.x + blockDim.x * blockIdx.x; // Column index

  // Check bounds
  if (i < n && j < n) {
    // Copy the k-th row and column to shared memory
    __shared__ int hbuf[MAX_N];
    __shared__ int vbuf[MAX_N];

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

  // Launch kernel with 32 blocks and 256 threads per block
  int numBlocks = 32;
  dim3 blocks(numBlocks, numBlocks);
  dim3 threadsPerBlock(THREADS_PER_BLOCK / numBlocks, THREADS_PER_BLOCK / numBlocks);
  updateMatrix<<<blocks, threadsPerBlock>>>(dev_D, n);

  // Copy result back to host
  cudaMemcpy(D, dev_D, n * n * sizeof(int), cudaMemcpyDeviceToHost);
  cudaFree(dev_D);
}






// void run_update_matrix(int *D, int n) {
//   int *dev_D;
  
//   // Allocate memory on the device
//   cudaMalloc((void**)&dev_D, n * n * sizeof(int));
//   cudaMemcpy(dev_D, D, n * n * sizeof(int), cudaMemcpyHostToDevice);

//   // Launch kernel
//   dim3 blocks(n, n / THREADS_PER_BLOCK); 
//   dim3 threadsPerBlock(THREADS_PER_BLOCK, THREADS_PER_BLOCK);
//   update_matrix<<<blocks, threadsPerBlock>>>(dev_D, n);

//   // Copy result back to host
//   cudaMemcpy(D, dev_D, n * n * sizeof(int), cudaMemcpyDeviceToHost);
//   cudaFree(dev_D);
// }