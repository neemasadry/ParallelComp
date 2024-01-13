//  CSC 6220 - Parallel Computing I: Programming
//  Homework 5 - Matrix Addition Program using MPI
//  Neema Sadry

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Declare variables for matrices
int matrix_1[2048][2048];
int matrix_2[2048][2048];
int matrix_final[2048][2048];

// Function for writing matrix_final to TXT file: "result.txt"; pass in an integer matrix of size 256x256
int matrix_output(int output[2048][2048]) {
  FILE *file_pointer = fopen("/wsu/home/eh/eh27/eh2731/hw5_seq_result.txt", "w+"); // CHANGE TO DESIRED PATH FOR result.txt FILE LOCATION
  // FILE *file_pointer = fopen("/Users/neema/Dropbox/Semesters/Fall 2022/CSC 6220 - Parallel Computing I Programming/Homeworks/Grid/Neema_Sadry_HW5/sequential_result.txt", "w+");

  // Output error to console and exit program if the file_pointer is null
  if (file_pointer == NULL) {
    printf("Error: File 'result.txt' could not be created/opened.\n");
    exit(-1);
  }
    
  for (int i = 0; i < 2048; i++) {
    for (int j = 0; j < 2048; j++) {
        // Send formatted output to a stream
        fprintf(file_pointer, "%d ", output[i][j]);
    }
    fprintf(file_pointer, "\n");
  }
  
  return fclose(file_pointer);
}



int main(int argc, const char * argv[]) {
  printf("---------------------------------------------\n");
  printf("Starting sequential MPI matrix (2048x2048) addition program...\n\n");

  for (int x = 0; x < 2048; x++) {
    for (int y = 0; y < 2048; y++) {
      matrix_1[x][y] = (y + 1);
      matrix_2[x][y] = (y + 1);
      matrix_final[x][y] = 0;
    }
  }

  clock_t begin_time = clock();

  for (int x = 0; x < 2048; x++) 
    for (int y = 0; y < 2048; y++) 
      matrix_final[x][y] = matrix_1[x][y] + matrix_2[x][y];

  matrix_output(matrix_final);

  clock_t end_time = clock();
  double time_diff = ((double)(end_time - begin_time) / CLOCKS_PER_SEC);

  printf("Sequential matrix addition finished! Time: %f\n", time_diff);
  printf("\nEnd of program. Exiting...\n");
  printf("---------------------------------------------\n");
  
  return 0;
}

