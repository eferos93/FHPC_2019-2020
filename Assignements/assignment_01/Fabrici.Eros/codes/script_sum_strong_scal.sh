for procs in 1 2 4 8 16 20 ; do
	/usr/bin/time mpirun -np ${procs} sumNumbers_mpi.x
done
