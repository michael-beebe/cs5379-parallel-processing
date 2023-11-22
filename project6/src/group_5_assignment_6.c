#include <stdio.h>
#include <stdlib.h>
#include "update_matrix.h"

void initialize_matrix(int *D, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) {
        D[i * n + j] = 0;
      } else {
        D[i * n + j] = rand() % 10 + 1; // Random weights between 1 and 10
      }
    }
  }
}

void print_matrix(int *D, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", D[i * n + j]);
    }
    printf("\n");
  }
}

int main() {
    int n = MAX_N; // Use the maximum matrix size
    int *D = (int*)malloc(n * n * sizeof(int));

    // Initialize D with random data
    initialize_matrix(D, n);

    printf("Original Matrix:\n");
    print_matrix(D, n);

    run_update_matrix(D, n);

    printf("Matrix after Floyd-Warshall:\n");
    print_matrix(D, n);

    free(D);
    return 0;
}