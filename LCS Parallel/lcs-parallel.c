/*************************************************************************
DESCRIPTION:
        Parallel Implementation of LCS algorithm

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

/* Slowdown function, used to make our code easier to time*/
short cost(int x){
  int i, n_iter = 20;
  double dcost = 0;
  
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
    fscanf(fp, "%hu %hu\n", &size_of_vector[0], &size_of_vector[1]);
    
    
    /* Start arrays to store sequences */
    seq_1 = (char *) malloc ((size_of_vector[0]) * sizeof(char));
    seq_2 = (char *) malloc ((size_of_vector[1]) * sizeof(char));
    
    /* Read arrays */
    fscanf(fp,"%s\n%s", seq_1, seq_2);
    
    fclose (fp);
  }
  
  //~ printf("%s\n",seq_1);
  //~ printf("%s\n",seq_2);
  
  unsigned short int i=0, j=0, jMax;
  unsigned int** Matrix = (unsigned int **) malloc( (size_of_vector[0]+size_of_vector[1]+1)*sizeof(unsigned int));
  
  /* Loop to populate the matrix and give us the longest common subsequence size */
  printf("\n");
  for(i=0;i<size_of_vector[0]+size_of_vector[1]+1;i++){
    
    jMax=fmin( i+1 , fmin( size_of_vector[0]+size_of_vector[1]+1-i , fmin( size_of_vector[0]+1 , size_of_vector[1]+1 ) ) );
    
    Matrix[i] = (unsigned int *) malloc( ( jMax )*sizeof(unsigned int));
    
    
    //~ printf("\n");
    
    for(j=0;j<jMax;j++){
      //~ printf("%d«",jMax);
      //~ printf("%hu:%hu=%hu  ",i,j,Matrix[i][j]);
      
      //~ Matrix[i][j]=i+1;
      
      if( (j==0&&i<size_of_vector[0]+1)||(j==jMax-1&&i<size_of_vector[1]+1) ){
        Matrix[i][j]=0;
      }
      else if(seq_1[i-(int) fmax(j,j+i-size_of_vector[0])-1]==seq_2[(int) fmax(j,j+i-size_of_vector[0])-1]){
        Matrix[i][j]=Matrix[i-2][j-1+(int) fmax(0,i-size_of_vector[0]+1)]+1;//cost(i);
      }
      else{
        Matrix[i][j]=0;// fmax(Matrix[i-1][(int) fmax(j,j+i-size_of_vector[0])],Matrix[i-2][(int) fmax(j,j+i-size_of_vector[0])-1]);
        
      }
      
    }
    
  }
  printf("\n\n");
  
  /* Debugging code, use to print out full matrix. Comment when not using */
  
  
  for(i=0;i<size_of_vector[0]+size_of_vector[1]+1;i++){
    
    jMax=fmin( i+1 , fmin( size_of_vector[0]+size_of_vector[1]+1-i , fmin( size_of_vector[0]+1 , size_of_vector[1]+1 ) ) );
    
    
    printf("\n");
    
    for(j=0;j<jMax;j++){
      printf("%6hu;",Matrix[i][j]);
      
    }
    
  }
  
  printf("\n\n");
  
  /* Printf in matrix form */
  unsigned short int a,b;
  for(a=0;a<size_of_vector[0]+1;a++){
        
    printf("\n");
    
    for(b=0;b<size_of_vector[1]+1;b++){
      i=a+b;
      j=b-fmax(0,a+b-1-size_of_vector[0]+1);
      printf("%6hu=%6hu:%6hu=%6hu:%6hu;",Matrix[i][j],i,j,i-2,j-2+(int) fmax(0,i-size_of_vector[0]+1));
      
    }
    
  }
  printf("\n\n");
  
  
  return 0;
}
