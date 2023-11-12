
/*
|---------------------------|-----------|
|          Group 5 Assignment 5         |
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
#include<stdlib.h>
#include "mpi.h"
/**************************************************************************************************/
#define N 64
void force_calc(int n, double *x, double *f, int P, int myRank);
/**************************************************************************************************/
int main(int argc, char * argv[])
{
    MPI_Init(&argc, &argv);
    int P, myRank, i;
    double *particles, *F;
    ///////////////////////////////////////////////////////////////////////////////
    F = (double *)malloc(N *sizeof(double));
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    ///////////////////////////////////////////////////////////////////////////////
    particles = (double *)malloc(N*sizeof(double)); //initialize and fill particles with random numbers
    if (myRank == 0)
    {
        for (i = 0; i < N; i++)
        {
            particles[i] = (i+1);// to make sure every X is unique
            printf("%lf \t", particles[i]);
        }
    }
    MPI_Bcast(particles, N, MPI_DOUBLE, 0, MPI_COMM_WORLD); //rank 0 distribute the generated particles
    MPI_Barrier(MPI_COMM_WORLD); //to make sure all processor receives a copy of X
    ///////////////////////////////////////////////////////////////////////////////
    printf("\nHello from rank %d\n", myRank);
    force_calc(N, particles, F, P, myRank);//call force calculation
    MPI_Finalize();
    return 0;
}
/**************************************************************************************************/
void force_calc(int n, double *x, double *f, int P, int myRank)
{
    int i, j, chunk = n / P, iteration=chunk/2,  row_location,l=0, k=0;
    double temp = 0.0,   diff;
    double *buffer = (double *)malloc(n* sizeof(double));
    ///////////////////////////////////////////////////////////////////////////////
    for (i = 0; i<n; i++)
    {
        f[i] = 0.0;
    }
    ///////////////////////////////////////////////////////////////////////////////
    // the iteration indicates how many split do we have
    // l indicates which iteration we are in
    while(l<iteration) {
        k=0; //k indicates which part we are in
        while (k < 2) {
            if (k == 0) {
                row_location = (l*(P*2))+myRank; //this will be repeated every 8 rows for the first part which has 4 rows
                printf("part 1 myrank=%d  startPos=%d endPos=%d\n", myRank, row_location, row_location+1);
            } else {
                row_location = ((l+1)*(P*2)-1) - myRank;//this will be repeated every 8 rows for the second part which has 4 rows
                printf("part 2 myrank=%d  startPos=%d endPos=%d\n", myRank, row_location, row_location+1);
            }
            for (i = row_location; i < row_location+1; i++) {
                for (j = 0; j < i; j++) {
                    if (j == i) {
                        temp = 0.0;
                    } else {
                        diff = (x[i] - x[j]);
                        temp += 1.0 / diff * diff * diff;
                    }
                    f[i] += temp;
                    f[j] -= temp;
                }
            }
            k++;
        }
        l++;
    }
    ///////////////////////////////////////////////////////////////////////////////
    //all the f arrays will gather in the master process
    MPI_Reduce(f, buffer, n, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    ///////////////////////////////////////////////////////////////////////////////
    if (myRank == 0)
    {
        for (i = 0; i < n; i++)
        {
            printf("%lf \t", buffer[i]);
        }
    }
}
