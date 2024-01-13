CSC 6220 - Homework #3
Matrix Addition Program by Neema Sadry

===========================================

This program was compiled and executed on both MacBook Pro (M1 Pro) and WSU's HPC Grid. Therefore, the commands given here are for UNIX-based systems. Please use the equivalent commands if using a Windows system. For more information on compiling C/C++ programs on the WSU HPC Grid, please read the following article: https://tech.wayne.edu/kb/high-performance-computing/hpc-tutorials/500065

1) Make sure that the PATH for outputting the result.txt file (Line 32) is set to your desired path.

2) To compile the program locally, run the following command in your Terminal or Command Prompt:

	gcc pthread_matrix_addition.c -pthread -o pthread_matrix_addition

	NOTE: This program utilizes the pthread library. Therefore, make sure the "-pthread" flag is included. Otherwise, the compilation process will fail.

3) Run the newly generated executable using the following command:

	./pthread_matrix_addition