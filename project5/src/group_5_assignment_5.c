
/*
|---------------------------------------|
|          Group 5 Assignment 5         |
|---------------------------------------|
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


#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "mpi.h"

#define sgn(x) ( ((x)<0.0) ? (-1.0) : (1.0) )
#define c1 1.23456
#define c2 6.54321
// #define n 123456

/* Input: n, x[n]. Note that x[i] \not= x[j] for different i, j.
Output: f[n].
*/
void calc_force(int n, double *x, double *f) {
    int i,j;
    double diff, tmp;
    for(i=0; i<n; i++) {
        f[i] = 0.0;
    }
    for(i=1; i<n; i++) {
        for(j=0; j<i; j++) {
            diff = x[i] - x[j] ;
            tmp = c1/(diff*diff*diff) - c2*sgn(diff)/(diff*diff) ;
            f[i] += tmp;
            f[j] -= tmp;
        }
    }
}

/***************************************************************************/
int main(int argc, char **argv ) {
    int i,j;

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Each process will compute forces for a subset of particles
    int local_n = n / size;
    double *local_x = (double *)malloc(local_n * sizeof(double));
    double *local_f = (double *)malloc(local_n * sizeof(double));

    // Distribute the particle positions
    MPI_Scatter(x, local_n, MPI_DOUBLE, local_x, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Initialize local force array
    for (int i = 0; i < local_n; i++) {
        local_f[i] = 0.0;
    }

    // Calculate forces for local particles
    calc_force(local_n, local_x, local_f);

    // Gather the results back to the root process
    MPI_Gather(local_f, local_n, MPI_DOUBLE, f, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // If this is the root process, combine the results
    if (rank == 0) {
        // Combine results from each process here
        // Note: This would involve additional logic to handle the forces
        // calculated on different processes.
    }

    // Free allocated memory
    free(local_x);
    free(local_f);

    // Finalize the MPI environment
    MPI_Finalize();
    return 0;
}