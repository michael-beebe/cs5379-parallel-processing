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

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define INFTY 11111111
#define MIN(a,b) (((a) < (b)) ? (a) : (b)) //get the minimum for two values
////////////////////////////////////////////////////////////////////////////////////////////////////
void f(int SOURCE, int n, int **edge, int *dist, int rank, int P);
int choose(int *dist, int n, int *found, int P, int rank, int startIndex);
////////////////////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char* argv[]) {
    int rank, P, **edge = NULL, *dist = NULL, n= 1024, SOURCE=0, name_len,i,l; // P is number of processor
    char processor_name[MPI_MAX_PROCESSOR_NAME]; // holds the processor name
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //MPI Initialisation
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Get_processor_name(processor_name, &name_len); // get the name of the processor
    printf("\n/*************************************************************************************/\n");
    printf("/******* Hello world from processor %s, rank %d out of %d processors *****************/\n", processor_name, rank, P);
    printf("/*************************************************************************************/\n");
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if(rank == 0)   // only master rank can enter this section
    {
        printf("Master processor entering 2D inputs ... \n\n");
        srand(0);
        edge= malloc(n * sizeof(int *));
        dist = malloc(n * sizeof(int));
        for (i = 0; i < n; i++)
        {
            edge[i] = malloc(n * sizeof(int));
            for(l=0; l<n; l++)
            {
                edge[i][l]= rand()%50; // fill the 2D matrix with random numbers up to 50
                printf("%d  ", edge[i][l]); //print the matrix
            }
            printf("\n");

        }
        f(SOURCE, n, edge, dist, rank, P);  //call f function and attache SOURCE, n, edge, dist, rank, and P
        for(i = 0; i < n; i++)
        {
            printf("dist[%d]= %d \n",i, dist[i]);
        }
        printf("\n");
        free(edge); //free edge array
        free(dist); //free dist array
    }
        ////////////////////////////////////////////////////////////////////////////////////////////////////
    else    // every processor can enter this section except the master processor
    {
        printf("\n/*************************************************************************************/\n");
        printf("/******* Hello world from processor %s, rank %d out of %d processors *****************/\n", processor_name, rank, P);
        printf("/*************************************************************************************/\n");
        int *edge2= malloc((n/P) * sizeof(int *)); // each processor will initiate its own edge = n/p
        for (i = 0; i < n; i++)
            edge2[0]= (int) malloc((n ) * sizeof(int));
        int *dist2= malloc((n) * sizeof(int));  // each processor will initiate its own dist = n/p
        f(SOURCE, n, &edge2, dist2, rank, P);   //call f function and attache SOURCE, n, edge, dist, rank, and P
        free(edge2);    //free edge array
        free(dist2);    //free dist array
    }
    MPI_Finalize();
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void f(int SOURCE, int n, int **edge, int *dist, int rank, int P) {
    int  location,j, minimum = 0, minimum2 = 0, *minimumLocationBuffer, *found, *tempFound, i, d, c;
    MPI_Status status[n], status2[5*P]; // every processor will make 5 send/recv
    found = malloc(n * sizeof(int));
    tempFound = malloc((n / P) * sizeof(int));
    minimumLocationBuffer = malloc(P * sizeof(int)); // holds least values of each processor
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    for (i = 0; i < n; i++) {
        found[i] = 0;   //initialize found
        dist[i] = edge[SOURCE][i];  //initialize dist
    }
    found[SOURCE] = 1;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    for(d=1; d<n; d++)
    {
        if (rank == 0) { // only master can enter this section
            j = choose(dist, n, found, P, rank, 0);
            minimumLocationBuffer[0] = j;
            for (i = 1; i < P; i++) {
                location = (n / P) * i;
                MPI_Send(&dist[location], (n / P), MPI_INT, i, 1, MPI_COMM_WORLD); // send n/p dist to processors
                MPI_Send(&found[location], (n / P), MPI_INT, i, 2, MPI_COMM_WORLD); // send n/p found to processors
                MPI_Recv(&minimumLocationBuffer[i], 1, MPI_INT, i, 3, MPI_COMM_WORLD, &status[i]); //receive the minimum location
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            int flag=0; // flag used to avoid the first -1 value of the minimum location
            for (c = 1 ; c < P ; c++ ) {
                if (minimumLocationBuffer[c - 1] != -1 && flag == 0) {
                    minimum = dist[minimumLocationBuffer[c - 1]];
                    flag = 1;
                }
                if (minimumLocationBuffer[c] != -1 && flag == 1) {
                    if (dist[minimumLocationBuffer[c]] < minimum) {
                        minimum = dist[minimumLocationBuffer[c]];
                        j = c;
                    }
                }
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            found[j] = 1;
            for (i = 0; i < (n / P); i++)
            {
                if (!(found[i]))
                {
                    dist[i] = MIN(dist[i], (minimum + edge[j][i])); // find minimum value of dist
                }
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            for (i = 1; i < P; i++) {
                location = i * (n / P);
                MPI_Send(&found[location], (n / P), MPI_INT, i, 10, MPI_COMM_WORLD);    // send n/p dist to processors
                MPI_Send(&edge[j][location], (n / P), MPI_INT, i, 11, MPI_COMM_WORLD);  // send n/p edges to processors
                MPI_Send(&minimum, 1, MPI_INT, i, 12, MPI_COMM_WORLD);                  // send minimum to processors
                MPI_Recv(&dist[location], (n / P), MPI_INT, i, 20, MPI_COMM_WORLD, &status[i]); //receive the minimum dist value
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////
        }
        else {  // all processors enter this section except the master processor
            MPI_Recv(dist, (n / P), MPI_INT, 0, 1, MPI_COMM_WORLD, &status2[rank]); // receive n/p dist from master
            MPI_Recv(found, (n / P), MPI_INT, 0, 2, MPI_COMM_WORLD, &status2[rank+1]);  // receive n/p found from master
            j = choose(dist, n, found, P, rank,0); // get the minimum location
            MPI_Send(&j, 1, MPI_INT, 0, 3, MPI_COMM_WORLD); //send j
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            MPI_Recv(tempFound, (n / P), MPI_INT, 0, 10, MPI_COMM_WORLD, &status2[rank+2]); //found n/p dist from master
            MPI_Recv(edge[0], (n / P), MPI_INT, 0, 11, MPI_COMM_WORLD, &status2[rank+3]);   //found n/p edge from master
            MPI_Recv(&minimum2, 1, MPI_INT, 0, 12, MPI_COMM_WORLD, &status2[rank+4]);   //found n/p dist from master
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            for (i = 0; i < (n / P); i++)
            {
                if (!(tempFound[i])) {
                    dist[i] = MIN(dist[i], (minimum2 + edge[0][i])); //find minimum
                }

            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            MPI_Send(dist, (n / P), MPI_INT, 0, 20, MPI_COMM_WORLD); //send dist to master
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if (rank == 0)
    {
        free(minimumLocationBuffer);
        free(found);
        free(tempFound);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
int choose(int *dist, int n, int *found, int P, int rank, int startIndex)
{
    int i, tmp, least = INFTY, leastPosition = -1;
    for(i=startIndex;i<n/P;i++){
        tmp = dist[i];
        if( (!found[i]) && (tmp < least)){
            least = tmp;
            leastPosition = i;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if(leastPosition==-1)
        return -1;
    return leastPosition+(rank*(n/P)); // return the location of the least value on the chunk;
}
////////////////////////////////////////////////////////////////////////////////////////////////////



