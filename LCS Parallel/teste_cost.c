/*************************************************************************
DESCRIPTION:
        Serial Implementation of LCS algorithm

STUDENTS:
        César Alves
        José Antunes
        Mauro Machado

Course:
        Computação Paralela e Distribuida - 2014 @ IST

Compile with:

        gcc -lm -o lcs-serial lcs-serial.c

**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <omp.h>


int main(int argc, char *argv[])
{
    short int i = 0, j = 0;
    int tid;
    int size_of_vector[3];

    size_of_vector[0] = 10;
    size_of_vector[1] = 15;

    short int **Matrix = (short int **) malloc((size_of_vector[0]+1) * sizeof(short int *));

    #pragma omp parallel for private(j)
    for (i = 0; i <= (size_of_vector[0]); i++)
    {

        Matrix[i] = (short int *) malloc( (size_of_vector[1]+1) * sizeof(short int));

        for (j = 0; j <= (size_of_vector[1]); j++)
        {
            Matrix[i][j] = i + j;
        }
    }

    printf("Matrix feita.\n");

    // for ( i = 0; i <= size_of_vector[0]; i++)
    // {
    //     printf("\n");
    //     for ( j = 0; j <= size_of_vector[1]; j++)
    //     {
    //         printf("%4d ", Matrix[i][j]);
    //     }
    // }
    printf("\n");
    printf("Matrix impressa.\n");

    /* Loop to populate the matrix and give us the longest common subsequence size */
    int a = 0, b = 0;

    for (a= 0; a <= 5; a++)
    {
        #pragma omp parallel for private(tid)
        for (b = 0; b <= a; b++)
        {
            tid = omp_get_thread_num();
            printf("j=%2d    i=%2d    thread=%d \n", a, b, tid);
        }
    }

    return 0;
}