
#include <stdio.h>
#include <math.h>
#include "mpi.h"

void myBarrier(int rank, int p_number) {
  printf("Processor #%d has entered the barrier section\n", rank);
  int i, j, n, dest, buf, k, k1;
  int counter = 1;
  n = (int)  log(p_number);   //The Output will be 2*log2(p_number)
  //The reducing part  which is been log2(p_number)
  //loop for log(p_number) times
  for (i = n - 1; i >= 0; i--) {
    k = (int)pow(2, i);
    k1 = (int)pow(2, i + 1);
    for (j = k; j<k1; j++) {
      if (rank == j) {
        dest = j - (int)pow(2, i);
        //Message needs to be sent to process with dest
        //Notification message
        MPI_Send(&counter, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        printf("Processor #%d sent notification message to processor #%d\n", rank, dest);
      }
      else if (rank == (j - (int)pow(2, i))) {
        //packs up all of its necessary data into a buffer for process dest.
        //printf("\n j =%d, (int)pow(2, i)= %d, rank=%d \n", j, (int)pow(2, i), rank);
        MPI_Recv(&buf, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }
  }
  /* The scattering part which is been log2(p_number)*/
  for (i = 0; i<n; i++) {
    k = ((int)pow(2, i)) - 1;
    for (j = 0; j <= k; j++) {
      if (rank == j) {
        dest = j + (int)pow(2, i);
        MPI_Send(&counter, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
          printf("Processor #%d sent notification message to processor #%d\n", rank, dest);
      }
      else if (rank == (j + (int)pow(2, i))) {
        MPI_Recv(&buf, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }
    }
  }
}

int main(int argc,char** argv) {
  int rank, p_number, name_len;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p_number);
  MPI_Get_processor_name(processor_name, &name_len);
  
  printf("\nHello world from processor %s, rank %d out of %d processors\n",
    processor_name, rank, p_number);
  
  myBarrier(rank, p_number);
  
  printf("Processor #%d exited the barrier section\n\n", rank);
  
  MPI_Finalize();
  return 0;
}