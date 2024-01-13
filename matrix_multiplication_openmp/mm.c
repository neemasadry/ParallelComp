#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <omp.h>


// Declare constants and variables for matrices
#define DIMENSION_SIZE 1024

int A[1024][1024];
int B[1024][1024];
int C[1024][1024];

int nthreads;

void initialize_matrices(int dim) {
  for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            A[i][j] = 1;
            B[i][j] = 2;
            C[i][j] = 0;
        }
    }
}


void mm_serial(int dim)
{
  double start = omp_get_wtime();
  for (int i = 0; i < dim; i++) 
    {
      for (int j = 0; j < dim; j++) 
      {
        for (int k = 0; k < dim; k++)
          C[i][j] += A[i][k] * B[k][j];
      }
    }
  double end = start - omp_get_wtime();
  printf("END TIME: %f!\n", end);
} 

void mm_parallel_static(int nthreads, int dim)
{
  int thread_id;
  double start = omp_get_wtime();
  
  // omp_set_num_threads(nthreads);
  #pragma omp parallel shared(A, B, C, dim) nthreads(4)
  {
    thread_id = omp_get_thread_num();
    // Split the outermost iteration across threads statically
    #pragma omp for schedule(static)
    for (int i = 0; i < dim; i++)
    {
      for (int j = 0; j < dim; j++) 
      {
        for(int k = 0; k < dim; k++) // iterating over columns of A/rows of B
          C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  double end = start - omp_get_wtime();
  printf("END TIME: %f!\n", end);
}

void mm_parallel_static_eight(int nthreads, int dim)
{
  int thread_id;
  double start = omp_get_wtime();
  
  // omp_set_num_threads(nthreads);
  #pragma omp parallel shared(A, B, C, dim) nthreads(4)
  {
    thread_id = omp_get_thread_num();
    // Split the outermost iteration across threads statically
    #pragma omp for schedule(static, 8)
    for (int i = 0; i < dim; i++)
    {
      for (int j = 0; j < dim; j++) 
      {
        for(int k = 0; k < dim; k++) // iterating over columns of A/rows of B
          C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  double end = start - omp_get_wtime();
  printf("END TIME: %f!\n", end);
}

void mm_parallel_dynamic(int nthreads, int dim)
{
  int thread_id;
  double start = omp_get_wtime();
  
  // omp_set_num_threads(nthreads);
  #pragma omp parallel shared(A, B, C, dim) nthreads(4)
  {
    thread_id = omp_get_thread_num();
    // Split the outermost iteration across threads statically
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < dim; i++)
    {
      for (int j = 0; j < dim; j++) 
      {
        for(int k = 0; k < dim; k++) // iterating over columns of A/rows of B
          C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  double end = start - omp_get_wtime();
  printf("END TIME: %f!\n", end);
}

void mm_parallel_guided(int nthreads, int dim)
{
  int thread_id;
  double start = omp_get_wtime();
  
  // omp_set_num_threads(nthreads);
  #pragma omp parallel shared(A, B, C, dim) nthreads(4)
  {
    thread_id = omp_get_thread_num();
    // Split the outermost iteration across threads statically
    #pragma omp for schedule(guided)
    for (int i = 0; i < dim; i++)
    {
      for (int j = 0; j < dim; j++) 
      {
        for(int k = 0; k < dim; k++) // iterating over columns of A/rows of B
          C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  double end = start - omp_get_wtime();
  printf("END TIME: %f!\n", end);
}

int main(int argc, const char * argv[]) {
    printf("---------------------------------------------\n");
    printf("Starting square matrix (1024x1024) multiplication program...\n\n");
    
    
    initialize_matrices(DIMENSION_SIZE);
    printf("Initialized matrices!\n");

    mm_serial(DIMENSION_SIZE);
    printf("finished serialized matrix multiplication!\n");

    mm_parallel_static(2, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 2)!\n");

    mm_parallel_static(4, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 4)!\n");

    mm_parallel_static(8, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 8)!\n");

    mm_parallel_static(16, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 16)!\n");

    mm_parallel_static(32, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 32)!\n");



    mm_parallel_static_eight(2, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 2)!\n");

    mm_parallel_static_eight(4, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 4)!\n");

    mm_parallel_static_eight(8, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 8)!\n");

    mm_parallel_static_eight(16, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 16)!\n");

    mm_parallel_static_eight(32, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 32)!\n");



    mm_parallel_dynamic(2, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 2)!\n");

    mm_parallel_dynamic(4, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 4)!\n");

    mm_parallel_dynamic(8, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 8)!\n");

    mm_parallel_dynamic(16, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 16)!\n");

    mm_parallel_dynamic(32, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 32)!\n");



    mm_parallel_guided(2, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 2)!\n");

    mm_parallel_guided(4, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 4)!\n");

    mm_parallel_guided(8, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 8)!\n");

    mm_parallel_guided(16, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 16)!\n");

    mm_parallel_guided(32, DIMENSION_SIZE);
    printf("finished parallelized matrix multiplication (# of Threads: 32)!\n");

    
    printf("\nEnd of program. Exiting...\n");
    printf("---------------------------------------------\n");
    
    return 0;
}