To run program locally:
❯ clang matrix_add_seq.c -o matrix_add_seq
❯ ./matrix_add_seq

To run program on WSU Grid:
❯ sbatch job-script-seq.sh






Please refer to set of instructions provided in mpi-instructions-F22.txt for further information:

STEP 1:
======

To login to WSU Grid use ssh or putty:

ssh <your_WSU_id>@grid.wayne.edu

with the same password you have for all WSU systems (e.g., academica, canvas)


Place the source file and Makefile into your home directory using sftp.

sftp <your_WSU_id>@grid.wayne.edu



STEP 2:
=======
The compilation of you program should not be done on the
head node "warrior". You should request an interactive session
to a grid node by typing:

srun -q debug -t 10:0 --pty bash

Once the interactive session starts you can compile your program by typing:

make

in your home directory.
This should generate the mpi executable.

You should type exit to go back to "warrior" node. Once there go to STEP 3.


STEP 3:
======
To execute your job interactively you should copy the "job-script.sh"
file in your home directory. 

Then, submit your job for execution by typing

sbatch job-script.sh

In the jobscript.sh file, you would need to specify the number of nodes and the number of
cores per node required by your MPI program. 


The output of your program will be placed in a file named: output_<job-id>.out
The <job-id> is the job id. 

======================================