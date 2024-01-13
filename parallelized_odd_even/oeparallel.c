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
    // file_pointer = fopen("/wsu/home/eh/eh27/eh2731/termproject/results/result.txt", "w+");
    file_pointer = fopen("/Users/neema/Dropbox/Semesters/Fall 2022/CSC 6220 - Parallel Computing I Programming/Homeworks/Grid/Neema_Sadry_TermProject/result.txt", "w+"); // default 
  }
  else if (output_type == 2)
    // file_pointer = fopen("/wsu/home/eh/eh27/eh2731/termproject/results/squicksort_result.txt", "w+"); 
    file_pointer = fopen("/Users/neema/Dropbox/Semesters/Fall 2022/CSC 6220 - Parallel Computing I Programming/Homeworks/Grid/Neema_Sadry_TermProject/squicksort_result.txt", "w+");
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

void sequential_sort(int *array_to_sort, int size) {
  int done = 0;
  
  while (done == 0) {
    done = 1;
    int i;

    for (i = 1; i < (size - 1); i += 2) {
      if (array_to_sort[i] > array_to_sort[i+1]) {
        int temp = array_to_sort[i+1];
        array_to_sort[i+1] = array_to_sort[i];
        array_to_sort[i] = temp;
        done = 0;
      }
    }

    for (i = 0; i < (size - 1); i += 2) {
      if (array_to_sort[i] > array_to_sort[i+1]) {
        int temp = array_to_sort[i+1];
        array_to_sort[i+1] = array_to_sort[i];
        array_to_sort[i] = temp;
        done = 0;
      }
    }   
  }
}


void low_array_half(int *first_array, int *second_array, int size) {
  int *new = malloc(size * sizeof(int));
  int a = 0;
  int b = 0;
  int count;

  for (count = 0; count < size; count++) {
    if (first_array[a] <= second_array[b]) {
      new[count] = first_array[a];
      a++;
    } else {
      new[count] = second_array[b];
      b++;
    }
  } 
  
  for (count = 0; count < size; count++) 
    first_array[count] = new[count];
  
  free(new);
}


void high_array_half(int *first_array, int *second_array, int size) {
  int *new = malloc(size * sizeof(int));
  int a = (size - 1);
  int b = (size - 1);
  int count;

  for (count = (size - 1); count >= 0; count--) {
    if (first_array[a] >= second_array[b]) {
      new[count] = first_array[a];
      a--;
    } else {
      new[count] = second_array[b];
      b--;
    }
  } 
  
  for (count = 0; count < size; count++) 
    first_array[count] = new[count];
  
  free(new);
}

void exchange_next(int *sub_array, int size, int rank) {
  MPI_Send(sub_array, size, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);
  int *next_array = malloc(size * sizeof(int));
  MPI_Status stat;
  MPI_Recv(next_array, size, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD, &stat);
  low_array_half(sub_array, next_array, size);
  free(next_array);
}

void exchange_previous(int *sub_array, int size, int rank) {
  MPI_Send(sub_array, size, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD);
  int *previous_array = malloc(size * sizeof(int));
  MPI_Status stat;
  MPI_Recv(previous_array, size, MPI_INT, (rank - 1), 0, MPI_COMM_WORLD, &stat);
  high_array_half(sub_array, previous_array, size);
  free(previous_array);
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
      
  int host_count;
  MPI_Comm_size(MPI_COMM_WORLD, &host_count);
  
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int *array_to_sort = malloc(N * sizeof(int));
  if (rank == 0) {
    srand(time(NULL));

    int i;
    for (i = 0; i < N; i++) 
      array_to_sort[i] = rand() % 129; // random integer - range [0, 128]
  }
    
  double start_clock;
  if (rank == 0) 
    start_clock = clock();
  
  
  int *sub_array = malloc(((N / host_count)) * sizeof(int));
  if (rank == 0)
    MPI_Scatter(array_to_sort, (N / host_count), MPI_INT, sub_array, (N / host_count), MPI_INT, 0, MPI_COMM_WORLD);
  
  int *subarray_displacement = malloc(host_count * sizeof(int));
  int i;
  for (i = 0; i < host_count; i++) 
    subarray_displacement[i] = i*((N / host_count));
  
  int *send_element_count = malloc(host_count * sizeof(int));
  for (i = 0; i < host_count; i++) 
    send_element_count[i] = (N / host_count);
    
  MPI_Scatterv(array_to_sort, send_element_count, subarray_displacement, MPI_INT, sub_array, (N / host_count), MPI_INT, 0, MPI_COMM_WORLD);
  free(subarray_displacement);
  free(send_element_count);
    
  sequential_sort(sub_array, (N / host_count));

  i = 0;
  for (i = 0; i < host_count; i++) {
    // EVEN PHASE 
    if (i % 2 == 0) {
      if (rank % 2 == 0) { // Check if host number is EVEN
        if (rank < (host_count - 1)) 
          exchange_next(sub_array, (N / host_count), rank);
      } else { 
        if ((rank - 1) >= 0) 
          exchange_previous(sub_array, (N / host_count), rank);
      }
    }
    // ODD PHASE
    else { 
      if (rank % 2 != 0) { // Check if host number is ODD
        if (rank < (host_count - 1)) 
          exchange_next(sub_array, (N / host_count), rank);
      }
      else {
        if ((rank - 1) >= 0) 
          exchange_previous(sub_array, (N / host_count), rank);
      }
    }
  }

  MPI_Gather(sub_array, (N / host_count), MPI_INT, array_to_sort, (N / host_count), MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    printf("\nArray sorted using Modified Odd-Even Parallel algorithm!\n");
    sorted_array_output(array_to_sort, 1);
    
    double end_clock = clock();
    printf("Sorted with MPI in %f seconds\n\n", (end_clock - start_clock) / CLOCKS_PER_SEC);
  }
  

  MPI_Finalize();

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