for procs in 2 4 8 16 20 ; do
 /usr/bin/time mpirun -np ${procs} mpi_pi.x $1 #$1 = input size passed as arg
 done
