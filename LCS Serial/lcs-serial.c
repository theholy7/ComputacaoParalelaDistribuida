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

    fclose (fp);
   }

  return 0;
}
