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
#include "mpi.h"

void HW3(int SOURCE, int n, int **edge, int *distance) {
  int i, j, count, tmp, least, leastPos, *found;

  found = (int *)calloc(n, sizeof(int));
  for (i = 0; i < n; i++) {
    found[i] = 0;
    distance[i] = edge[SOURCE][i];
  }
  found[SOURCE] = 1;
  count = 1;

  while (count < n) {
    least = 987654321;
    for (i = 0; i < n; i++) { //parallelize this loop
      tmp = distance[i];
      if ((!found[i]) && (tmp < least)) {
        least = tmp;
        leastPos = i;
      }
    }

    found[leastPos] = 1;
    count++;
    for (i = 0; i < n; i++) { //parallelize this loop
      if (!found[i]) {
        distance[i] = min(distance[i], least + edge[leastPos][i]);
      }
    }
  } /*** End of while ***/

  free(found);
}

int main(int argc, char **argv)
{
  int rank, process_count, *distance, **edge;

  // MPI initialization
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &process_count);

  distance = (int *)calloc(process_count, sizeof(int));
  edge = (int **)malloc(process_count * sizeof(int *));
  for (int i = 0; i < process_count; i++) {
    edge[i] = (int *)calloc(process_count, sizeof(int));
  }

  //TODO: Generate edge[n][n] values
  //      Simply using random function does not work 
  //      since it may violate triangle inequality
  for (int i = 0; i < process_count; i++) {
    for (int j = i + 1; j < process_count; j++) {

    }
  }

  HW3(0, process_count, edge, distance);
  MPI_Finalize();
  return 0;
}