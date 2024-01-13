//  CSC 6220 - Parallel Computing I: Programming
//  Homework 3 - Matrix Addition Program
//  Neema Sadry

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Declare variables for matrices
int matrix_1[256][256];
int matrix_2[256][256];
int matrix_final[256][256];

// Function for matrix addition
void* matrix_addition(void *a) {
    int n = (int) (size_t) a;
    
    for (int i = (n * 32); i <= ((n * 32) + 31); i++) {
        for (int j = 0; j < 256; j++) {
            matrix_final[i][j] = matrix_1[i][j] + matrix_2[i][j];
        }
    }
    
    printf("Thread %d: Done\n", n);
    pthread_exit(NULL);
}

// Function for writing matrix_final to TXT file: "result.txt"; pass in an integer matrix of size 256x256
int matrix_output(int output[256][256]) {
    // FILE *file_pointer = fopen("/Users/neema/Dropbox/Semesters/Fall 2022/CSC 6220 - Parallel Computing I Programming/Homeworks/result.txt", "w+");
    FILE *file_pointer = fopen("/wsu/home/eh/eh27/eh2731/result.txt", "w+"); // CHANGE TO DESIRED PATH FOR result.txt FILE LOCATION   

    // Output error to console and exit program if the file_pointer is null
    if (file_pointer == NULL) {
        printf("Error: File 'result.txt' could not be created/opened.\n");
        exit(-1);
    }
    
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            // Send formatted output to a stream
            fprintf(file_pointer, "%d ", output[i][j]);
        }
        fprintf(file_pointer, "\n");
    }
    
    return fclose(file_pointer);
}


int main(int argc, const char * argv[]) {
    printf("---------------------------------------------\n");
    printf("Starting matrix (256x256) addition program...\n\n");
    
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            matrix_1[i][j] = (j + 1);
            matrix_2[i][j] = (j + 1);
            matrix_final[i][j] = 0;
        }
    }
    
    pthread_t worker[8];
    
    for (int x = 7; x >= 0; x--)
        pthread_create(&worker[x], NULL, &matrix_addition, x);
    
    for (int x = 7; x >= 0; x--)
        pthread_join(worker[x], NULL);
    
    matrix_output(matrix_final);
    
    printf("\nEnd of program. Exiting...\n");
    printf("---------------------------------------------\n");
    
    return 0;
}

