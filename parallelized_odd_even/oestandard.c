#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include <mpi.h>

#define N 65536 
// #define N 1048576
// #define N 16777216

// Function for writing result of program to text file named: result.txt (default)
// Listed below are the integer keys to output text file containing result of program executed
// 1 - Modified Odd-Even Sort Parallel algorithm
// 2 - Standard Odd-Even Sort algorithm
// 3 - Serial Quicksort
int sorted_array_output(int sorted_array[N]) {
  // FILE *file_pointer = fopen("/wsu/home/eh/eh27/eh2731/termproject/oestandard_result.txt", "w+"); // CHANGE TO DESIRED PATH FOR result.txt FILE LOCATION
  
  FILE *file_pointer;
  file_pointer = fopen("/wsu/home/eh/eh27/eh2731/termproject/results/oestandard_result.txt", "w+");
  // file_pointer = fopen("/Users/neema/Dropbox/Semesters/Fall 2022/CSC 6220 - Parallel Computing I Programming/Homeworks/Grid/Neema_Sadry_TermProject/oestandard_result.txt", "w+");

  // Output error to console and exit program if the file_pointer is null
  if (file_pointer == NULL) {
    printf("Error: File 'oestandard_result.txt' could not be created/opened.\n");
    exit(-1);
  }
    
  int i;
  for (i = 0; i < N; i++) 
    fprintf(file_pointer, "%d, ", sorted_array[i]); 
  
  return fclose(file_pointer);
}


/* This is the CompareSplit function */ 
void CompareSplit(int nlocal, int *elmnts, int *relmnts, int *wspace, int keepsmall) { 
  int i, j, k; 

  for (i= 0; i < nlocal; i++) 
    wspace[i] = elmnts[i]; /* Copy the elmnts array into the wspace array */ 

  if (keepsmall) { /* Keep the nlocal smaller elements */ 
    for (i=j=k=0; k < nlocal; k++) { 
      if (j == nlocal || (i < nlocal && wspace[i] < relmnts[j])) 
        elmnts[k] = wspace[i++]; 
      else 
        elmnts[k] = relmnts[j++]; 
    } 
  } 
  else { /* Keep the nlocal larger elements */ 
    for (i=k=nlocal-1, j=nlocal-1; k >= 0; k--) { 
      if (j == 0 || (i >= 0 && wspace[i] >= relmnts[j])) 
        elmnts[k] = wspace[i--]; 
      else 
        elmnts[k] = relmnts[j--]; 
    } 
  } 
} 
 
/* The IncOrder function that is called by qsort is defined as follows */ 
int IncOrder(const void *e1, const void *e2) { 
  return (*((int *)e1) - *((int *)e2)); 
} 

int main(int argc, char *argv[]) { 
  int n;         /* The total number of elements to be sorted */ 
  int npes;      /* The total number of processes */ 
  int myrank;    /* The rank of the calling process */ 
  int nlocal;    /* The local number of elements, and the array that stores them */ 
  int *elmnts;   /* The array that stores the local elements */ 
  int *relmnts;  /* The array that stores the received elements */ 
  int oddrank;   /* The rank of the process during odd-phase communication */ 
  int evenrank;  /* The rank of the process during even-phase communication */ 
  int *wspace;   /* Working space during the compare-split operation */ 
  int i; 
  MPI_Status status; 

  /* Initialize MPI and get system information */ 
  MPI_Init(&argc, &argv); 
  MPI_Comm_size(MPI_COMM_WORLD, &npes); 
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank); 

  // n = atoi(argv[1]); 
  n = N;
  nlocal = n/npes; /* Compute the number of elements to be stored locally. */ 

  /* Allocate memory for the various arrays */ 
  elmnts  = (int *)malloc(nlocal*sizeof(int)); 
  relmnts = (int *)malloc(nlocal*sizeof(int)); 
  wspace  = (int *)malloc(nlocal*sizeof(int)); 

  /* Fill-in the elmnts array with random elements */ 
  srand(myrank); 
  for (i = 0; i < nlocal; i++) 
    elmnts[i] = rand() % 129; // random integer - range [0, 128]

  double start_clock;
  if (myrank == 0) 
    start_clock = clock();

  /* Sort the local elements using the built-in quicksort routine */ 
  qsort(elmnts, nlocal, sizeof(int), IncOrder); 

  /* Determine the rank of the processors that myrank needs to communicate during ics/ccc.gifthe */ 
  /* odd and even phases of the algorithm */ 
  if ((myrank % 2) == 0) { 
    oddrank  = myrank - 1; 
    evenrank = myrank + 1; 
  } 
  else { 
    oddrank  = myrank + 1; 
    evenrank = myrank - 1; 
  } 

  /* Set the ranks of the processors at the end of the linear */ 
  if (oddrank == -1 || oddrank == npes) 
    oddrank = MPI_PROC_NULL; 
  if (evenrank == -1 || evenrank == npes) 
    evenrank = MPI_PROC_NULL; 

  /* Get into the main loop of the odd-even sorting algorithm */ 
  for (i = 0; i < (npes-1); i++) { 
    if (i%2 == 1) /* Odd phase */ 
      MPI_Sendrecv(elmnts, nlocal, MPI_INT, oddrank, 1, relmnts, nlocal, MPI_INT, oddrank, 1, MPI_COMM_WORLD, &status); 
    else /* Even phase */ 
      MPI_Sendrecv(elmnts, nlocal, MPI_INT, evenrank, 1, relmnts, nlocal, MPI_INT, evenrank, 1, MPI_COMM_WORLD, &status); 

    CompareSplit(nlocal, elmnts, relmnts, wspace, myrank < status.MPI_SOURCE); 
  } 

  if (myrank == 0) {
    printf("\nArray sorted using Standard Odd-Even Sort!\n");
    // sorted_array_output(relmnts);
    
    double end_clock = clock();
    printf("\n\tStandard Odd-Even Sort with MPI in %f seconds\n", (end_clock - start_clock) / CLOCKS_PER_SEC);
  }

  free(elmnts);
  free(relmnts);
  free(wspace);

  MPI_Finalize();
  return 0;
} 