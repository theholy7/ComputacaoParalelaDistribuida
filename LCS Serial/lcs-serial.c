/*************************************************************************
DESCRIPTION:
        Serial Implementation of LCS algorithm

STUDENTS:
        César Alves
        José Antunes
        Mauro Machado

Course:
        Computação Paralela e Distribuida - 2014 @ IST

**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
    printf("%d %d\n", size_of_vector[0], size_of_vector[1]);

    /* Start arrays to store sequences */
    
    char *seq_1 = (char *) malloc ((size_of_vector[0]+1) * sizeof(char));
    int i = 0;
    char c;
    do{
      c = getc(fp);
      seq_1[i] = c;
      i++;

    } while(c != '\n');

    for(i = 0; i < size_of_vector[0]+1; i++)
      printf("%c", seq_1[i]);

    char *seq_2 = (char *) malloc ((size_of_vector[1]+1) * sizeof(char));
    i=0;
    do{
      c = getc(fp);
      seq_2[i] = c;
      i++;

    } while(c != '\n');

    for(i = 0; i < size_of_vector[1]+1; i++)
      printf("%c", seq_2[i]);


    fclose (fp);
   }

  return 0;
}
