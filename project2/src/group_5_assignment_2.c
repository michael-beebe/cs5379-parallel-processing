/*
|---------------------------|-----------|
|          Group 5 Assignment 2         |
|---------------------------|-----------|
|   Group Member            |    R#     |
|---------------------------|-----------|
| Michael Beebe             | R11772231 |
| Diego Salas Noain         | R11794236 |
| Bandar Alkhalil           | R11836831 |
| Yongjian Zhao             | R11915830 |
| Denish Otieno             | R11743138 |
| Shiva Kumar Neekishetty   | R11842757 |
|---------------------------------------|
*/

#include <stdio.h>
#include <math.h>
#include "mpi.h"

void mybarrier(MPI_Comm comm)
{
  int rank, process_count;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &process_count);

  printf("Processor #%d has entered the barrier section\n\n", rank);
  int i, j, n, dest, buf, k, k1;
  int counter = 1;
  n = (int)log2(process_count); // Iteration steps

  // The reducing phase, loop for n times
  for (i = n - 1; i >= 0; i--)
  {
    k = (int)pow(2, i);
    k1 = (int)pow(2, i + 1);
    for (j = k; j < k1; j++)
    {
      if (rank == j)
      {
        dest = j - (int)pow(2, i);
        // Message needs to be sent to process with dest
        MPI_Send(&counter, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        printf("Processor #%d sent notification message to processor #%d in the reducing phase\n", rank, dest);
      }
      else if (rank == (j - (int)pow(2, i)))
      {
        // packs up all of its necessary data into a buffer for process dest.
        MPI_Recv(&buf, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processor #%d received notification message from processor #%d in the reducing phase\n", rank, j);
      }
    }
  }

  /* The scattering part which takes log2(process_count) loops*/
  for (i = 0; i < n; i++)
  {
    k = ((int)pow(2, i)) - 1;
    for (j = 0; j <= k; j++)
    {
      if (rank == j)
      {
        dest = j + (int)pow(2, i);
        MPI_Send(&counter, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        printf("Processor #%d sent notification message to processor #%d in the scattering phase\n", rank, dest);
      }
      else if (rank == (j + (int)pow(2, i)))
      {
        MPI_Recv(&buf, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Processor #%d received notification message from processor #%d in the scattering phase\n", rank, j);
      }
    }
  }
  MPI_Finalize();
}

int main(int argc, char **argv)
{
  int rank, process_count, process_name_len;
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  // MPI initialization
  MPI_Init(&argc, &argv);
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &process_count);
  MPI_Get_processor_name(processor_name, &process_name_len);

  printf(
    "Hello world from processor %s, rank %d out of %d processors\n",
    processor_name, rank, process_count
  );
  mybarrier(comm);
  printf("Processor #%d exited the barrier section.\n\n", rank);

  // MPI_Finalize();
  return 0;
}