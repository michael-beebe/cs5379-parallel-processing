
/*
|---------------------------|-----------|
|          Group 5 Assignment 4         |
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
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "mpi.h"

#define INTINFINITY 12345678
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

int W_rank;				//MPI world rank
int no_P_worldCome;		//# of P in world Comm
MPI_Comm row_group, column_group; //row/ column communication
int row_rank;		    //row group ranking
int col_rank;			// Processor Rank in a Column
int n = 16;				// # of rows and columns
int **D0, **D;          //input/output matrices (given be the professor)
int chunk;

/***************************************************************************/
void shortestPath();
void print_subMatrix(int chunk, int **D00);
void generateRandomElements(int chunk, bool diagonal);

/***************************************************************************/
int main(int argc, char **argv )
{
    int i,j;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &W_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &no_P_worldCome);

    //The following loop finds the color of the row grouping based on W_Rank
    int tempRank=W_rank, color;
    for(i = 0;i < (int)sqrt(no_P_worldCome); i++)
    {
        if(tempRank<(int)sqrt(no_P_worldCome))
        {
            color=i;
            i=(int)sqrt(no_P_worldCome);
        }
        else
            tempRank=tempRank-(int)sqrt(no_P_worldCome);
    }

    //split MPI_COMM_WORLD into two groups, one for row and the other for column
    MPI_Comm_split(MPI_COMM_WORLD, color, W_rank, &row_group);
    MPI_Comm_split(MPI_COMM_WORLD, W_rank%(int)sqrt(no_P_worldCome), W_rank, &column_group);

    //assign the groups ranks variable
    MPI_Comm_rank(row_group, &row_rank);
    MPI_Comm_rank(column_group, &col_rank);

    //calculate chunck
    chunk = n/(int)sqrt(no_P_worldCome);

    //fill D0 with random numbers
    generateRandomElements(chunk, row_rank == col_rank);

    //call the shortest path function
    shortestPath();

    MPI_Finalize();
}
/***************************************************************************/
void shortestPath()
{
    int *row_no_K, *col_no_K,k,i,j,row_handeler, col_handeler;

    //the following buffer holds the row/column number K to be broadcast
    row_no_K = malloc(chunk*sizeof(int));
    col_no_K = malloc(chunk*sizeof(int));

    k=0;
    while (k<n)
    {
        //get the rank which handle the Kth row and column
        row_handeler = k/chunk;
        col_handeler = k/chunk;

        //update Kth row if this row assigned to you
        if (row_rank == row_handeler)
        {
            memcpy ( &row_no_K, &D0[k%chunk], sizeof(chunk) );
        }

        //update Kth column if this row assigned to you
        if (col_rank == col_handeler)
        {
            for (i=0; i<chunk; i++)
            {
                col_no_K[i] = D0[i][k%chunk];
            }
        }

        //Broadcast Kth row and column to designated group
        MPI_Bcast(row_no_K, chunk, MPI_INT, row_handeler, row_group);
        MPI_Bcast(col_no_K, chunk, MPI_INT, col_handeler, column_group);

        for (i=0; i<chunk; i++)
        {
            for (j=0; j<chunk; j++)
            {
                D[i][j]= MIN(D0[i][j],(row_no_K[i] + col_no_K[j]));
                D0[i][j]=D[i][j];
            }
        }

        k++;
    }


    //print the sub matrix
    print_subMatrix(chunk, D);
    free(D);
    free(D0);
}

/***************************************************************************/
//this function generates random elements and 0 for the diagonal
void generateRandomElements(int chunk, bool diagonal){
    int i,j;
    D0 = malloc(chunk * sizeof(int *));
    D = malloc(chunk * sizeof(int *));
    for (i = 0; i < chunk; i++){
        D0[i] =  malloc(chunk * sizeof(int));
        D[i] =  malloc(chunk * sizeof(int));
    }
    srand((unsigned int) (time(NULL) + W_rank));
    for(i = 0; i < chunk; i++)
    {
        for(j = i; j < chunk; j++)
        {
            if(diagonal && i == j){
                D0[i][j] = 0;
            }
            else
            {
                // generate numbers between 1 to 10, if it is 10 set to be INFINITY
                int r = (rand() % (10))+1;
                int value = (r ==10 )? INTINFINITY: r;
                D0[i][j] = value;
                D0[j][i] = value;
            }
        }
    }
    print_subMatrix(chunk, D0);
}

/***************************************************************************/
//this function print the sub matrix
void print_subMatrix(int chunk, int **D)
{
    int i, j;
    printf("\n");
    for(i = 0; i < chunk; i++) {
        for (j = 0; j < chunk; j++) {
            printf("%d \t", D[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
/***************************************************************************/
