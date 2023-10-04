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
#include "mpi.h"
#define min(i, j) (((i) < (j)) ? (i) : (j))

void HW3(int SOURCE, int n, int **edge, int *distance, int rank, int process_count) {
  int i, j, count, tmp, least, leastPos, *found;

  found = (int *)calloc(n, sizeof(int));
  for (i = 0; i < n; i++) {
    found[i] = 0;
    distance[i] = edge[SOURCE][i];
  }
  found[SOURCE] = 1;
  count = 1;

  // int chunk, istart, iend;
  // chunk = n/process_count;
  // istart = rank * chunk;
  // iend  = istart + chunk ;

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

  HW3(0, n, edge, distance, rank, process_count);
  MPI_Finalize();
  return 0;
}