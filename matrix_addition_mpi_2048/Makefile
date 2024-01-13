CC	=	/opt/ohpc/pub/mpi/openmpi3-gnu7/3.1.0/bin/mpicc
CCLINK	=	/opt/ohpc/pub/mpi/openmpi3-gnu7/3.1.0/bin/mpicc
SHELL	=	/bin/sh

EXEC	= matrix_add_mpi

main: matrix_add_mpi.c
	$(CC) -o $(EXEC) $(EXEC).c

clean:
	/bin/rm -f $(EXEC) $(EXEC)*.o $(EXEC)*.s

EXECSEQ	= matrix_add_seq

main: matrix_add_mpi.c
	$(CC) -o $(EXECSEQ) $(EXECSEQ).c

clean:
	/bin/rm -f $(EXECSEQ) $(EXECSEQ)*.o $(EXECSEQ)*.s

