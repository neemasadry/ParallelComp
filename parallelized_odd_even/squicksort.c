#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

// SELECT SIZE OF ARRAY PRIOR TO COMPILING AND EXECUTING PROGRAM
#define N 65536 // 2^16
// #define N 1048576 // 2^20
// #define N 16777216 // 2^24

// Function for writing result of program to text file named: result.txt (default)
// Listed below are the integer keys to output text file containing result of program executed
// 1 - Modified Odd-Even Sort Parallel algorithm
// 2 - Standard Odd-Even Sort algorithm
// 3 - Serial Quicksort
int sorted_array_output(int sorted_array[N], int output_type) {
  // FILE *file_pointer = fopen("/wsu/home/eh/eh27/eh2731/termproject/result.txt", "w+"); // CHANGE TO DESIRED PATH FOR result.txt FILE LOCATION
  FILE *file_pointer;
  if (output_type == 1) {
    file_pointer = fopen("/wsu/home/eh/eh27/eh2731/termproject/results/result.txt", "w+");
    // file_pointer = fopen("/Users/neema/Dropbox/Semesters/Fall 2022/CSC 6220 - Parallel Computing I Programming/Homeworks/Grid/Neema_Sadry_TermProject/result.txt", "w+"); // default 
  }
  else if (output_type == 2)
    file_pointer = fopen("/wsu/home/eh/eh27/eh2731/termproject/results/squicksort_result.txt", "w+"); 
    // file_pointer = fopen("/Users/neema/Dropbox/Semesters/Fall 2022/CSC 6220 - Parallel Computing I Programming/Homeworks/Grid/Neema_Sadry_TermProject/serial_quicksort_result.txt", "w+");
  else
    printf("Please select either 1 (Modified Odd-Even Sort Parallel algorithm) or 2 (Serial Quicksort).");
    
  // Output error to console and exit program if the file_pointer is null
  if (file_pointer == NULL) {
    printf("Error: File 'result.txt' could not be created/opened.\n");
    exit(-1);
  }
    
  int i;
  for (i = 0; i < N; i++) 
    fprintf(file_pointer, "%d, ", sorted_array[i]); 
  
  return fclose(file_pointer);
}

int increasing_order(const void *e1, const void *e2) { 
  return (*((int *)e1) - *((int *)e2)); 
}


int main(int argc, char **argv) { 
  double start_clock;
  start_clock = clock();

  // Begin Serial Quicksort
  int *array_to_quicksort = malloc(N * sizeof(int));
  
  srand(time(NULL));
  int z;
  for (z = 0; z < N; z++) 
    array_to_quicksort[z] = rand() % 129; // random integer - range [0, 128]
    
  double start_quicksort_clock;
  start_quicksort_clock = clock();
  
  qsort(array_to_quicksort, N, sizeof(int), increasing_order);

  printf("\nArray Quick Sorted!\n");
  sorted_array_output(array_to_quicksort, 2);

  double end_quicksort_clock = clock();
  printf("Sorted using Serial Quicksort in %f seconds\n\n", (end_quicksort_clock - start_quicksort_clock) / CLOCKS_PER_SEC);
  
  return 0;
}