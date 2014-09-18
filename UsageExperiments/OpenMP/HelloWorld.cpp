/*
 Taken from http://en.wikipedia.org/wiki/OpenMP#Hello_World
 
 A first test to see if everything is working ok
 
 Compile with:
  Linux Debian Jessie: "g++ -fopenmp -o HelloWorld HelloWorld.cpp"
 
 Currently Working for: César
 
*/
#include <iostream>

using namespace std;
 
#include <omp.h>
 
int main(int argc, char *argv[])
{
  int th_id, nthreads;
  #pragma omp parallel private(th_id) shared(nthreads)
  {
    th_id = omp_get_thread_num();
    #pragma omp critical
    {
      cout << "Hello World from thread " << th_id << '\n';
    }
    #pragma omp barrier
 
    #pragma omp master
    {
      nthreads = omp_get_num_threads();
      cout << "There are " << nthreads << " threads" << '\n';
    }
  }
 
  return 0;
}
