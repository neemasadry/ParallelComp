CC	=	/opt/ohpc/pub/mpi/openmpi3-gnu7/3.1.0/bin/mpicc
CCLINK	=	/opt/ohpc/pub/mpi/openmpi3-gnu7/3.1.0/bin/mpicc
SHELL	=	/bin/sh

main:
	$(CC) squicksort.c -o squicksort
	$(CC) oeparallel.c -o oeparallel
	$(CC) oestandard.c -o oestandard

clean: squicksort oeparallel oestandard
	/bin/rm -f $@ $@*.o $@*.s
