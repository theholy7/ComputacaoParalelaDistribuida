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

/* input validation: verify if only 2 parameters are used */
int input_validation (int arg_count, char *arg_vector[])
{

    if (arg_count != 2)
    {
        printf("\nOnly use one input parameter.\nExample: \"lcs-serial input.in\"\n\n");

        return -1;
    }
    return 1;
}

/* Slowdown function, used to make our code easier to time*/
short cost(int x)
{
    int iter, n_iter = 20;
    double dcost = 0;

    for (iter = 0; iter < n_iter; iter++)
        dcost += pow(sin((double) x), 2) + pow(cos((double) x), 2);

    return (short) (dcost / n_iter + 0.1);
}



/* main: process parameters */
int main(int argc, char *argv[])
{

    FILE *fp;
    short int errnum;

    int size_of_vector[3];
    char *seq_1, *seq_2;

    input_validation(argc, argv);

    /* Program start: read file from argv */
    fp = fopen (argv[1], "r");

    /* Validate if file exists*/
    if (fp == NULL)
    {

        /* If not: print error */
        errnum = errno;
        fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        return -1;
        exit(EXIT_SUCCESS);
    }

    else
    {
        /* Read important values from file - 2 ints - 2 strings */
        fscanf(fp, "%d %d\n", &size_of_vector[0], &size_of_vector[1]);

        // printf("%d %d", size_of_vector[0], size_of_vector[1]);


        /* Start arrays to store sequences */
        seq_1 = (char *) malloc ((size_of_vector[0]) * sizeof(char));
        seq_2 = (char *) malloc ((size_of_vector[1]) * sizeof(char));

        /* Read arrays */
        fscanf(fp, "%s\n%s", seq_1, seq_2);

        fclose (fp);
    }

    // printf("%s\n", seq_1);
    // printf("%s\n", seq_2);

    int i = 0, j = 0;
    short int **Matrix = (short int **) malloc((size_of_vector[0] + 1) * sizeof(short int *));


    /* Loop to populate the matrix and give us the longest common subsequence size */
    printf("Iniciar Matrix.\n");
    #pragma omp parallel for schedule(static) private(j)
    for (i = 0; i <= (size_of_vector[0]); i++)
    {

        Matrix[i] = (short int *) malloc( (size_of_vector[1] + 1) * sizeof(short int));

        for (j = 0; j <= (size_of_vector[1]); j++)
        {
            Matrix[i][j] = 0;
        }
    }

    printf("Matrix feita.\n");


    // Start important variables for anti-diagonal
    int num_diag = 0; //Anti-diagonal number
    int diff_2 = 0;   // Difference between last vector position of height and diagonal number
    int diff = 0;
    //Iterate through all anti-diagonals of a matrix
    printf("Calc Matrix.\n");
    for (num_diag = 0; num_diag <= size_of_vector[0] + size_of_vector[1]; num_diag++)
    {
        j = num_diag; //Column = anti-diag number
        i = 0;        //Start in row zero

        // if anti-diag number bigger than num of columns of matrix
        if (num_diag > size_of_vector[1])
        {
            j = size_of_vector[1]; //Use last column as start point
            diff = num_diag - size_of_vector[1];  //Difference between anti-diag number and matrix width
            i = diff;  //Start in row equal to difference
        }

        // if anti-diag number bigger than number of rows
        if (num_diag > size_of_vector[0])
        {
            diff_2 = num_diag - size_of_vector[0]; //Difference between anti-diag number and matrix height
        }

        // printf("%d - diagonal antes do parallel for i= %d diff_2= %d j=%d\n", num_diag, i, diff_2, j);
        int a = i; // create variable with start point (i know it is redundant)
        int interval = num_diag - diff_2; //create variable with for limit (another redundancy)

        #pragma omp parallel for firstprivate(j)
        for (i = a; i <= interval; i++)
        {
            //printf("ndiag=%d    i=%d\n", i);
            j = num_diag - i;


            // printf("Teste 1\n");
            if (i >= 1 && j >= 1)
            {
                if (seq_1[i - 1] == seq_2[j - 1])
                {
                    Matrix[i][j] = Matrix[i - 1][j - 1] + cost(i);
                }
                else
                {
                    Matrix[i][j] = fmax(Matrix[i - 1][j], Matrix[i][j - 1]);
                }
            }
        }
    }

    printf("\n FIM \n");
    // for (i = 1; i <= (size_of_vector[0]); i++)
    // {
    //     for (j = 1; j <= (size_of_vector[1]); j++)
    //     {
    // if (seq_1[i - 1] == seq_2[j - 1])
    // {
    //     Matrix[i][j] = Matrix[i - 1][j - 1] + cost(i);
    // }
    // else
    // {
    //     Matrix[i][j] = fmax(Matrix[i - 1][j], Matrix[i][j - 1]);
    // }
    //     }
    // }
    /* Debugging code, use to print out full matrix. Comment when not using */
    // for ( i = 0; i <= size_of_vector[0]; i++)
    // {
    //     printf("\n");
    //     for ( j = 0; j <= size_of_vector[1]; j++)
    //     {
    //         printf("%4d ", Matrix[i][j]);
    //     }
    // }
    // printf("\n");

    printf("%d\n", Matrix[size_of_vector[0]][size_of_vector[1]]);

    // /* Loop to discover the longest common subsequence */

    char *LongestSubsequence = (char *) malloc( ((Matrix[size_of_vector[0]][size_of_vector[1]])) * sizeof(char));

    i = size_of_vector[0] + 1;
    j = size_of_vector[1] + 1;
    short int CurrentNumber = Matrix[i - 1][j - 1];

    while ( i > 0 && j > 0 )
    {
        if (seq_1[i - 1] == seq_2[j - 1])
        {
            LongestSubsequence[CurrentNumber] = seq_1[i - 1];
            i--;
            j--;
            CurrentNumber--;
        }
        else if (Matrix[i - 1][j] > Matrix[i][j - 1])
        {
            i--;
        }
        else
        {
            j--;
        }
    }

    printf("%s\n", LongestSubsequence);

    return 0;
}
