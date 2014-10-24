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


short cost(int x){
  int iter, n_iter = 20;
  double dcost = 0;
  
  for(iter = 0; iter < n_iter; iter++)
    dcost += pow(sin((double) x),2) + pow(cos((double) x),2);
  
  return (short) (dcost / n_iter + 0.1);
}

int main(int argc, char *argv[]) {
  unsigned short int i=0, j=0;
  
  /* Loop to populate the matrix and give us the longest common subsequence size */
  char teste[] = "012345";

  printf("%s - len %d\n", teste, strlen(teste));

  for(i = 0; i <= strlen(teste)+2; i++)
    printf("%c\n", teste[i]);
 

  return 0;
}