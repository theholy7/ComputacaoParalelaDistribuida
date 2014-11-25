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
//#include <time.h>

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
            int iter, n_iter = 21;
            double dcost = 0;

            for (iter = 0; iter < n_iter; iter++)
                dcost += pow(sin((double) x), 2) + pow(cos((double) x), 2);

            return (short) (dcost / n_iter + 0.1);
        }






/* main: process parameters */
        int main(int argc, char *argv[])
        {

    //double start = omp_get_wtime();

            FILE *fp;
            short int errnum;

            int size_of_vector[3], block_size=21;
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



        /* Start arrays to store sequences */
                seq_1 = (char *) malloc ((size_of_vector[0]) * sizeof(char));
                seq_2 = (char *) malloc ((size_of_vector[1]) * sizeof(char));

        /* Read arrays */
                fscanf(fp, "%s\n%s", seq_1, seq_2);

                fclose (fp);
            }

    // printf("%s\n", seq_1);
    // printf("%s\n", seq_2);

            int i = 0, j = 0, aux_test=0;
            short int **Matrix_test = (short int **) malloc((size_of_vector[0] + 1) * sizeof(short int *));




    /* Loop to populate the matrix and give us the longest common subsequence size */
    //printf("Iniciar Matrix.\n");
    #pragma omp parallel for schedule(static) private(j)
            for (i = 0; i <= (size_of_vector[0]); i++)
            {

                Matrix_test[i] = (short int *) malloc( (size_of_vector[1] + 1) * sizeof(short int));

                for (j = 0; j <= (size_of_vector[1]); j++)
                {
                    Matrix_test[i][j] = 0;
                }
            }




   // Start important variables for anti-diagonal
    int diag = 0, dummy=0; //Anti-diagonal number
    int diff_22 = 0;   // Difference between last vector position of height and diagonal number
    int diff_1 = 0;
    //Iterate through all anti-diagonals of a matrix
   // printf("\nCalc Matrix_test.\n");
    //Nr total de anti-diagonais num bloco será block_size + block_size
    //No inicio a anti-diagonal é 0 mas para o seguinte a 1ºa anti-diagonal será
   // for(diag_new=0; diag_new <= size_of_vector[0] + size_of_vector[1]; diag_new=diag_new+block_size){
    
  //  printf("\n\ndiag_new: %d", diag_new);
    for (diag=0; diag <= block_size + block_size; diag++)


    {
        j = diag; //Column = anti-diag number
        i = 0;        //Start in row zero

        // if anti-diag number bigger than num of columns of matrix
        if (diag > block_size)
        {
            j = block_size; //Use last column as start point
            diff_1 = diag - block_size;  //Difference between anti-diag number and matrix width

            i = diff_1;  //Start in row equal to difference
        }

        // if anti-diag number bigger than number of rows
        if (diag > block_size)
        {
            diff_22 = diag - block_size; //Difference between anti-diag number and matrix height
        }

        // printf("%d - diagonal antes do parallel for i= %d diff_22= %d j=%d\n", diag, i, diff_22, j);
        int a = i; // create variable with start point (i know it is redundant)
        int interval = diag - diff_22; //create variable with for limit que corresponde em qual anti diagonal estamos (another redundancy)
       // printf("\nintervalo: %d\n", interval);


        //Para fazer a anti-diagonal
       // #pragma omp parallel for firstprivate(j)
        for (i = a; i <= interval; i++)
        {
            //printf("ndiag=%d    i=%d\n", i);
            j = diag - i;


            // printf("Teste 1\n");
            if (i >= 1 && j >= 1)
            {
                if (seq_1[i - 1] == seq_2[j - 1])
                {
                    Matrix_test[i][j] = Matrix_test[i - 1][j - 1] + cost(i);
                }
                else
                {
                    Matrix_test[i][j] = fmax(Matrix_test[i - 1][j], Matrix_test[i][j - 1]);
                }
            }
        }
     //   printf("\nanti-diagonal: %d\n", diag);
    }









//Corresponde em que fase nos encontramos no inicio temos os 2 blocos em paralelo (iteracao=0) depois temos 3 (iteracao=1)...
    short int iteracao=0, j_iniciobloco=0, i_iniciobloco=0, nr_diagbloco_block_size=0, diff_blocos=0, diff_2blocos=0, aux_zerodiag=0, aux_umdiag=0;
    float aux_diag=0, aux_zero=0, aux_um=0, nr_blocos_aux=0;
//Quantidade de blocos que numa linha vamos ter retirando o inicial



    aux_zerodiag=size_of_vector[0]/block_size;
    aux_umdiag=size_of_vector[1]/block_size;



    nr_diagbloco_block_size = aux_zerodiag + aux_umdiag;



//-2 porque uma anti diag já está computada e são nr_diagbloco_block_size-1 anti diag
    nr_diagbloco_block_size=nr_diagbloco_block_size-1;

    for(iteracao=1;iteracao<=nr_diagbloco_block_size;iteracao++){
//j_inicio será o inicio de cada bloco.
//No inicio é block_size + iteracao (em que iteracao estivermos) 
//Até j_iniciobloco=0 para andar na anti diag dos blocos   
//for(j_iniciobloco=block_size+iteracao;j_iniciobloco<=0;j_iniciobloco=j_iniciobloco-block_size){

//---------------------------


       j_iniciobloco=iteracao*block_size;
       i_iniciobloco=0;


       short int iniciar, intervalo_bloco;
 //a iteração vai ser a anti diagonal de blocos  

       if(iteracao*block_size>=size_of_vector[1]){
            //Ultima coluna de blocos como ponto inicial
        j_iniciobloco=size_of_vector[1]-block_size;
        diff_blocos=((iteracao*block_size) - (size_of_vector[1])+block_size);
        i_iniciobloco=diff_blocos;
    }

// if anti-diag number bigger than number of rows
    if (iteracao*block_size >= size_of_vector[0])
    {
        diff_2blocos = (iteracao*block_size) - size_of_vector[0]+block_size -1; //Difference between anti-diag number and matrix height
    }


    iniciar=i_iniciobloco;

    intervalo_bloco=iteracao*block_size - diff_2blocos;
    //printf("i_iniciobloco:%d\n",i_iniciobloco);
    //    printf("intervalo_bloco:%d\n", intervalo_bloco);



 #pragma omp parallel for firstprivate(j_iniciobloco)
    for(i_iniciobloco=iniciar;  i_iniciobloco<=(intervalo_bloco); i_iniciobloco=i_iniciobloco+block_size){
        //Corresponde à coordenada inicial do bloco em causa

        j_iniciobloco=iteracao*block_size - i_iniciobloco;


        short int i_aux= 0, diff_j=0, diff_i=0, ii=0, jj=0; 

        

        i_aux=i_iniciobloco; //Apenas no inicio do bloco        



        short int intervalo_j=0, intervalo_i=0;


        intervalo_j= j_iniciobloco+block_size;
        intervalo_i= i_aux+block_size;


        if (intervalo_i>size_of_vector[0])
        {
            intervalo_i=size_of_vector[0];

        }
        if (intervalo_j>size_of_vector[1])
        {
            intervalo_j=size_of_vector[1];

        }



//printf("intervalo: %d\n", intervalo);
//+i_iniciobloco para estar sempre relacionado com o inicio do bloco
        for (ii = i_aux; ii <= intervalo_i; ii++)
        {
            for (jj = j_iniciobloco; jj <= intervalo_j; jj++)
            {

                if (ii >= 1 && jj >= 1)
                {
                    if (seq_1[ii - 1] == seq_2[jj - 1])
                    {
                        Matrix_test[ii][jj] = Matrix_test[ii - 1][jj - 1] + cost(ii);
                    }
                    else
                    {
                        Matrix_test[ii][jj] = fmax(Matrix_test[ii - 1][jj], Matrix_test[ii][jj - 1]);
                    }
                }

            }



        }

        //Terminou a diagonal do bloco e agora tem de passar para a seguinte


    }


}


// printf("%d\n", Matrix_test[size_of_vector[0]][size_of_vector[1]]);

char *LongestSubsequence = (char *) malloc( ((Matrix_test[size_of_vector[0]][size_of_vector[1]])) * sizeof(char));
i = size_of_vector[0] + 1;
j = size_of_vector[1] + 1;
short int CurrentNumber = Matrix_test[i - 1][j - 1];
while ( i > 0 && j > 0 )
{
    if (seq_1[i - 1] == seq_2[j - 1])
    {
        LongestSubsequence[CurrentNumber] = seq_1[i - 1];
        i--;
        j--;
        CurrentNumber--;
    }
    else if (Matrix_test[i - 1][j] > Matrix_test[i][j - 1])
    {
        i--;
    }
    else
    {
        j--;
    }
}
//printf("%s\n", LongestSubsequence);


  //double end = omp_get_wtime();
    
  //  printf("tempo total:%f\n", end-start);





return 0;
}
