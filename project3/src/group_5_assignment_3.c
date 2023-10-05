/*
|---------------------------|-----------|
|          Group 5 Assignment 3         |
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
#include <stdlib.h>
#include <limits.h> 
#include "mpi.h"
#define min(i, j) (((i) < (j)) ? (i) : (j))

void HW3(int SOURCE, int n, int **edge, int *distance, int rank, int process_count) {
  int i, j, count, tmp, *found;
  int local_min_val, local_min_pos, *local_min_val_group, *local_min_pos_group, global_min_val, global_min_pos;

  found = (int *)calloc(n, sizeof(int));
  for (i = 0; i < n; i++) {
    found[i] = 0;
    distance[i] = edge[SOURCE][i];
  }
  found[SOURCE] = 1;
  count = 1;
  
  local_min_val_group = (int *)calloc(n, sizeof(int));
  local_min_pos_group = (int *)calloc(n, sizeof(int));
  //Determine the range each process calculate in
  int chunk, istart, iend;
  chunk = n/process_count;
  istart = rank * chunk;
  iend  = istart + chunk ;

  while (count < n) {
    //Reset the local/global min val to INT_MAX at each iteration
    local_min_val = INT_MAX, global_min_val = INT_MAX;
    for (i = istart; i < iend; i++) { //parallelize this loop (Achieved by range istart and iend)
      tmp = distance[i];
      if ((!found[i]) && (tmp < local_min_val)) {
        local_min_val = tmp;
        local_min_pos = i;
      }
    }
    //MPI_Allgather will gather the local info into local groups and broadcast to every process
    MPI_Allgather(&local_min_val, 1, MPI_INT, local_min_val_group, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(&local_min_pos, 1, MPI_INT, local_min_pos_group, 1, MPI_INT, MPI_COMM_WORLD);
    //Get the global min val and position
    for (i = 0; i < process_count; i++) {
      if (local_min_val_group[i] < global_min_val) {
        global_min_val = local_min_val_group[i];
        global_min_pos = local_min_pos_group[i];
      }
    }
    //Debug info, may be deleted in the final submission
    if (rank == 0 && count % 10 == 0) {
      for (i = 0; i < process_count; i++) {
        printf("%d ", local_min_val_group[i]);
      }
      printf("This is the %dth iteration, global min is %d at node %d\n", count, global_min_val, global_min_pos);
    }

    found[global_min_pos] = 1;
    count++;
    for (i = istart; i < iend; i++) { //parallelize this loop (Achieved by range istart and iend)
      if (!found[i]) {
        distance[i] = min(distance[i], global_min_val + edge[global_min_pos][i]);
      }
    }
    //We do not need to gather the info for distance array since each process handles an independent part, will gather the results after the while loop
    //MPI_Barrier(MPI_COMM_WORLD); //Not sure MPI_Barrier is needed, but in my test case, it is not needed
  } /*** End of while ***/

  // The send and receive buffer to be the same works on my end, or if you have objections we may follow the answer 
  // listed in https://stackoverflow.com/a/34453564/22692032
  
  // Gather the distance result on root process, i.e. process 0
  MPI_Gather(distance + istart, chunk, MPI_INT, distance, chunk, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("Final distance array value: ");
    for (i = 0; i < n; i++) {
      printf("%d ", distance[i]);
    }
    printf("\n");
  }
  
  free(found);
  free(local_min_val_group);
  free(local_min_pos_group);
}

int main(int argc, char **argv)
{
  int rank, process_count, n, *distance, **edge;

  // MPI initialization
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &process_count);

  n = 100;
  distance = (int *)calloc(n, sizeof(int));
  edge = (int **)malloc(n * sizeof(int *));
  for (int i = 0; i < n; i++) {
    edge[i] = (int *)calloc(n, sizeof(int));
  }

  //TODO: Generate edge[n][n] values
  //      Simply using random function does not work 
  //      since it may violate triangle inequality
  //Simple choice : Suppose all nodes are distributed in a straight line
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      edge[i][j] = edge[j][i] = j - i;
    }
  }
  //SOURCE = 0
  HW3(0, n, edge, distance, rank, process_count);
  free(distance);
  free(edge);
  MPI_Finalize();
  return 0;
}