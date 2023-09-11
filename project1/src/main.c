#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define generate_data(i, j) (i) + (j) * (j)
#define CHUNK_SIZE 5  // Number of rows in each chunk

int main(int argc, char **argv) {
  int i, j, pid, np, mtag;
  int data[100][100], row_sum[100];

  // Initialize MPI environment and get process ID and total number of processes
  MPI_Status status;
  MPI_Request req_s, req_r;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &np);

  if (pid == 0) { // Master process (pid 0)
    // Generate the first half of the data array
    for (i = 0; i < 50; i++)
      for (j = 0; j < 100; j++)
        data[i][j] = generate_data(i, j);

    // Asynchronously send the first half of the data array to process 1 in chunks
    mtag = 1;
    bool compute_flag = true;
    for (i = 0; i < 50; i += CHUNK_SIZE) {
      MPI_Isend(&data[i][0], CHUNK_SIZE * 100, MPI_INT, 1, mtag, MPI_COMM_WORLD, &req_s);

      if (compute_flag) {
        int m, n;  // use new variables instead of i and j
        // Generate the second half of the data array
        for (m = 50; m < 100; m++)
          for (n = 0; n < 100; n++)
            data[m][n] = generate_data(m, n);

        // Compute row sums for the second half of the data array
        for (m = 50; m < 100; m++) {
          row_sum[m] = 0;
          for (n = 0; n < 100; n++)
            row_sum[m] += data[m][n];
        }
        compute_flag = false;
      }

      if (i > 0) {  // Wait for the previous send to complete after initiating the next one
        MPI_Wait(&req_s, &status);
      }
    }
    // Wait for the last send to complete
    MPI_Wait(&req_s, &status);

    // Receive computed row sums for the first half from process 1
    mtag = 2;
    MPI_Recv(row_sum, 50, MPI_INT, 1, mtag, MPI_COMM_WORLD, &status);

    // Output the row sums
    for (i = 0; i < 100; i++) {
      printf(" %d ", row_sum[i]);
      if ((i + 1) % 5 == 0)
        printf("\n");
    }
  }
  else { // Worker process (pid 1)
    for (i = 0; i < 50; i += CHUNK_SIZE) {
      mtag = 1;
      MPI_Irecv(&data[i][0], CHUNK_SIZE * 100, MPI_INT, 0, mtag, MPI_COMM_WORLD, &req_r);
      MPI_Wait(&req_r, &status);  // Wait immediately for the data to be received

      // Compute row sums for the received chunk
      for (int k = i; k < i + CHUNK_SIZE; k++) {
        row_sum[k] = 0;
        for (j = 0; j < 100; j++) {
          row_sum[k] += data[k][j];
        }
      }
    }

    // Send computed row sums for the first half to process 0
    mtag = 2;
    MPI_Send(row_sum, 50, MPI_INT, 0, mtag, MPI_COMM_WORLD);
  }

  // Finalize MPI environment
  MPI_Finalize();
  return 0;
}

