#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define generate_data(i, j) (i) + (j) * (j)

int main(int argc, char **argv) {
  int i, j, pid, np, mtag, count;
  double t0, t1;
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

    // TODO: Asynchronously send the first half of the data array to process 1
    mtag = 1;
    MPI_Isend(data, 5000, MPI_INT, 1, mtag, MPI_COMM_WORLD, &req_s);

    // Generate the second half of the data array
    for (i = 50; i < 100; i++)
      for (j = 0; j < 100; j++)
        data[i][j] = generate_data(i, j);

    // Compute row sums for the second half of the data array
    for (i = 50; i < 100; i++) {
      row_sum[i] = 0;
      for (j = 0; j < 100; j++)
        row_sum[i] += data[i][j];
    }
    MPI_Wait(&req_s, &status);

    // Receive computed row sums for the first half from process 1
    mtag = 2;
    MPI_Recv(row_sum, 50, MPI_INT, 1, mtag, MPI_COMM_WORLD, &status);

    // Output the row sums
    for (i = 0; i < 100; i++) {
      printf(" %d ", row_sum[i]);
      if (i % 5 == 0)
        printf("\n");
    }
  }
  else { // Worker process (pid 1)
    // TODO: Receive the first half of the data array from process 0
    mtag = 1;
    MPI_Recv(data, 5000, MPI_INT, 0, mtag, MPI_COMM_WORLD, &status);

    // Compute row sums for the first half of the data array
    for (i = 0; i < 50; i++) {
      row_sum[i] = 0;
      for (j = 0; j < 100; j++)
        row_sum[i] += data[i][j];
    }

    // Send computed row sums for the first half to process 0
    mtag = 2;
    MPI_Send(row_sum, 50, MPI_INT, 0, mtag, MPI_COMM_WORLD);
  }

  // Finalize MPI environment
  MPI_Finalize();
  return 1;
} /****************** End of function main() ********************/