//  CSC 6220 - Parallel Computing I: Programming
//  Homework 5 - Matrix Addition Program using MPI
//  Neema Sadry

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include <time.h>

// Declare variables for matrices
int matrix_1[2048][2048];
int matrix_2[2048][2048];
int matrix_final[2048][2048];

int rec_matrix_buffer_1[256][256];
int rec_matrix_buffer_2[256][256];
int rec_matrix_buffer_final[256][256];

// Function for matrix addition
/*
void* matrix_addition(void *a) {
  int process_id = (int) (size_t) a;
  // 8 processes each of which responsible for handling 256 segments of matrix addition
  for (int i = (process_id * 256); i <= ((process_id * 256) + 255); i++) 
    for (int j = 0; j < 256; j++) 
      matrix_final[i][j] = matrix_1[i][j] + matrix_2[i][j];
  
  printf("Process %d: Done\n", process_id);
}
*/

// Function for writing matrix_final to TXT file: "result.txt"; pass in an integer matrix of size 256x256
int matrix_output(int output[2048][2048]) {
  FILE *file_pointer = fopen("/wsu/home/eh/eh27/eh2731/hw5_result.txt", "w+"); // CHANGE TO DESIRED PATH FOR result.txt FILE LOCATION
  // FILE *file_pointer = fopen("/Users/neema/Dropbox/Semesters/Fall 2022/CSC 6220 - Parallel Computing I Programming/Homeworks/Grid/Neema_Sadry_HW5/result.txt", "w+");

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


int main(int argc, char **argv) {
  printf("---------------------------------------------\n");
  printf("Starting MPI matrix (2048x2048) addition program...\n\n");

  int npes;
  int myrank;

  MPI_Init(&argc, &argv); // Initialize MPI

  MPI_Comm_size(MPI_COMM_WORLD, &npes);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  // MPI_Wtime results in errors!
  // double mpi_start_time = MPI_Wtime;
  // double mpi_end_time;
  clock_t begin_time = clock();


  // Process 0 initializes matrices and distributes blocks of 256 rows to remaining processes
  if (myrank == 0) {
    for (int i = 0; i < 2048; i++) {
      for (int j = 0; j < 2048; j++) {
        matrix_1[i][j] = (j + 1);
        matrix_2[i][j] = (j + 1);
        matrix_final[i][j] = 0;
      }
    }

    MPI_Scatter(&matrix_1, 256, MPI_INT, &rec_matrix_buffer_1, 256, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(&matrix_2, 256, MPI_INT, &rec_matrix_buffer_2, 256, MPI_INT, 0, MPI_COMM_WORLD);
    // for (int i = 0; i < 2048; i++) 
      // for (int j = 0; j < 2048; j++) 
        // MPI_Send(&matrix_1, 2048, MPI_INT, i, 0, MPI_COMM_WORLD);
      
  }

  if (myrank == 0) 
    MPI_Gather(&rec_matrix_buffer_final, 256, MPI_INT, &matrix_final, 256, MPI_INT, 0, MPI_COMM_WORLD);

  // MPI_Recv(matrix_1, 2048, MPI_INT, 0, 0, MPI_COMM_WORLD);
  // 8 processes each of which responsible for handling 256 segments of matrix addition
  for (int process_id = 7; process_id >= 0; process_id--) {
    for (int i = (process_id * 256); i <= ((process_id * 256) + 255); i++) {
      for (int j = 0; j < 256; j++) {
        matrix_final[i][j] = matrix_1[i][j] + matrix_2[i][j];
      }
    }
    printf("Process %d: Done\n", process_id);
  }
  
  MPI_Finalize();
  matrix_output(matrix_final);

  clock_t end_time = clock();
  double time_diff = ((double)(end_time - begin_time) / CLOCKS_PER_SEC);

  // MPI_Wtime results in errors!
  // mpi_end_time = MPI_Wtime;
  // double time_diff = mpi_end_time - mpi_start_time;

  printf("\nMPI Time difference: %f", time_diff);
  printf("\nEnd of program. Exiting...\n");
  printf("---------------------------------------------\n");
  
  return 0;
}

