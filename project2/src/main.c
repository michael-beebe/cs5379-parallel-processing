#include <stdio.h>
#include <math.h>
#include "mpi.h"

void mybarrier(MPI_Comm comm);

int main(int argc,char** argv)
{
    int rank, process_count, process_name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    // MPI onitialisation
    MPI_Init(&argc, &argv);
    MPI_Comm comm = MPI_COMM_WORLD;

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    // Get Processor Machine Name
    MPI_Get_processor_name(processor_name, &process_name_len);

    printf("/*************************************************************************************/\n");
    printf("/******* Hello world from %s, rank %d out of %d processors *****************/\n", processor_name, rank, process_count);
    printf("/*************************************************************************************/\n");

    mybarrier(comm);
    printf("******* Processor #%d exited the barrier section ******* \n", rank);
    return 0;
}

void mybarrier(MPI_Comm comm)
{
    int rank, process_count;
    // Get the rank of the process
    MPI_Comm_rank(comm, &rank);
    // Get the number of processes
    MPI_Comm_size(comm, &process_count);

    printf("******* Processor #%d has entered the barrier section ******* \n", rank);
    int i, j, n, dest, buf, k, k1;
    int counter = 1;
    n = (int)  log(process_count);   //The Output will be 2*log2(process_count)

    //The reducing part  which is been log2(process_count)
    //loop for log(process_count) times
    for (i = n - 1; i >= 0; i--) {
        k = (int)pow(2, i);
        k1 = (int)pow(2, i + 1);
        for (j = k; j < k1; j++)
        {
            if (rank == j) {
                dest = j - (int)pow(2, i);
                //Message needs to be sent to process with dest
                //Notification message
                MPI_Send(&counter, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                printf("******* Processor #%d sent notification message to processor #%d *******\n", rank, dest);
            }
            else if (rank == (j - (int)pow(2, i)))
            {
                //packs up all of its necessary data into a buffer for process dest.
                //printf("\n j =%d, (int)pow(2, i)= %d, rank=%d \n", j, (int)pow(2, i), rank);
                MPI_Recv(&buf, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }

    /* The scattering part which is been log2(process_count)*/
    for (i = 0; i < n; i++) {
        k = ((int)pow(2, i)) - 1;
        for (j = 0; j <= k; j++)
        {
            if (rank == j) {
                dest = j + (int)pow(2, i);
                MPI_Send(&counter, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
                printf("******* Processor #%d sent notification message to processor #%d *******\n", rank, dest);
            }
            else if (rank == (j + (int)pow(2, i)))
            {
                MPI_Recv(&buf, 1, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }

    MPI_Finalize(); //MPI_Finalize is used to clean up the MPI environment. No more MPI calls can be made after this one.
}

int main(int argc,char** argv) {
  int rank, p_number, name_len;
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  // MPI initialization
  MPI_Init(&argc, &argv);
  // Get the rank of the process
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // Get the number of processes
  MPI_Comm_size(MPI_COMM_WORLD, &p_number);
  // Get Processor Machine Name
  MPI_Get_processor_name(processor_name, &name_len);
  printf("/*************************************************************************************/\n");
  printf("/******* Hello world from processor %s, rank %d out of %d processors \n",
    processor_name, rank, p_number);
  printf("/*************************************************************************************/\n");
  myBarrier(rank, p_number);
  printf("******* Processor #%d exited the barrier section ******* \n", rank);
  return 0;
}