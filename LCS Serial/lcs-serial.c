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

/* input validation: verify if only 2 parameters are used */
int input_validation (int arg_count, char *arg_vector[]){

  if(arg_count != 2) {
    printf("\nOnly use one input parameter.\nExample: \"lcs-serial input.in\"\n\n");
    
    return -1;
  }
  return 1;
}


/* main: process parameters */
int main(int argc, char *argv[]) {
  
  FILE *fp;
  int errnum;
  
  int size_of_vector[3];
  
  /* Start arrays to store sequences */
  char *seq_1 = (char *) malloc ((size_of_vector[0]+1) * sizeof(char));
  char *seq_2 = (char *) malloc ((size_of_vector[1]+1) * sizeof(char));
  
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
    
    
    
    /* Read arrays */
    fscanf(fp,"%s\n%s", seq_1, seq_2);
    
    fclose (fp);
   }
   
  int i, j;
  int **Matrix = (int **) malloc( (size_of_vector[0]+1)*sizeof(int));
  
  for(i = 0; i <= size_of_vector[0]; i++){
    
    Matrix[i] = (int *) malloc( (size_of_vector[1]+1)*sizeof(int));
    
    for(j = 0; j <= size_of_vector[1]; j++){
      if(i==0||j==0){ 
        Matrix[i][j]=0;
        }
      else if(seq_1[i+1]==seq_2[j+1]){
        Matrix[i][j]=Matrix[i-1][j-1]+1;
        }
      else{
        Matrix[i][j]= fmax(Matrix[i-1][j],Matrix[i][j-1]);
        }
    }
   }

  return 0;
}
