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
int input_validation (int arg_count, char *arg_vector[]){

  if(arg_count != 2) {
    printf("\nOnly use one input parameter.\nExample: \"lcs-serial input.in\"\n\n");
    
    return -1;
  }
  return 1;
}

/* Slowdown function, used to make our code easier to time*/
short cost(int x){
  int i, n_iter = 20;
  double dcost = 0;
  
  #pragma omp parallel for reduction(+:dcost)
  for(i = 0; i < n_iter; i++)
    dcost += pow(sin((double) x),2) + pow(cos((double) x),2);
  return (short) (dcost / n_iter + 0.1);
}



/* main: process parameters */
int main(int argc, char *argv[]) {
  
  FILE *fp;
  unsigned short int errnum;
  
  unsigned short int size_of_vector[3];
  char *seq_1, *seq_2;
  
  input_validation(argc, argv);

  /* Program start: read file from argv */
  fp = fopen (argv[1], "r");

  /* Validate if file exists*/
  if (fp == NULL){

    /* If not: print error */
    errnum = errno;
    fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
  }

   else{
    /* Read important values from file - 2 ints - 2 strings */
    fscanf(fp, "%d %d\n", &size_of_vector[0], &size_of_vector[1]);
    
    
    /* Start arrays to store sequences */
    seq_1 = (char *) malloc ((size_of_vector[0]) * sizeof(char));
    seq_2 = (char *) malloc ((size_of_vector[1]) * sizeof(char));
    
    /* Read arrays */
    fscanf(fp,"%s\n%s", seq_1, seq_2);
    
    fclose (fp);
  }
  
  //~ printf("%s\n",seq_1);
  //~ printf("%s\n",seq_2);
  
  unsigned short int i=0, j=0;
  unsigned short int** Matrix = (unsigned short int **) malloc( (size_of_vector[0]+1)*sizeof(unsigned short int));
  
  /* Loop to populate the matrix and give us the longest common subsequence size */
  for(i = 0; i < (size_of_vector[0]+1); i++){
    
    Matrix[i] = (unsigned short int *) malloc( (size_of_vector[1]+1)*sizeof(unsigned short int));
    
    for(j = 0; j < (size_of_vector[1]+1); j++){
      if(i==0||j==0){ 
        Matrix[i][j]=0;
        }
      else if(seq_1[i-1]==seq_2[j-1]){
        Matrix[i][j]=Matrix[i-1][j-1] + cost(i);
        }
      else{
        Matrix[i][j]= fmax(Matrix[i-1][j],Matrix[i][j-1]);
        }
    }
  }
  
  /* Debugging code, use to print out full matrix. Comment when not using */
  //~ for( i=0; i<size_of_vector[0]+1; i++){
    //~ printf("\n");
    //~ for( j=0; j<size_of_vector[1]+1; j++){
    //~ printf("%4d ",Matrix[i][j]);
    //~ }
  //~ }
  
  printf("%d\n",Matrix[size_of_vector[0]][size_of_vector[1]]);
  
  /* Loop to discover the longest common subsequence */
  
  char *LongestSubsequence = (char *) malloc( ((Matrix[size_of_vector[0]][size_of_vector[1]]))*sizeof(char));
  
  i=size_of_vector[0]+1;
  j=size_of_vector[1]+1;
  unsigned short int CurrentNumber=Matrix[i-1][j-1];
  
  while( i>0 && j>0 ){
    if(seq_1[i-1]==seq_2[j-1]){
      LongestSubsequence[CurrentNumber]=seq_1[i-1];
      i--;
      j--;
      CurrentNumber--;
    }
    else if(Matrix[i-1][j]>Matrix[i][j-1]){
      i--;
    }
    else{
      j--;
    }
  }
    
  printf("%s\n",LongestSubsequence);

  return 0;
}
