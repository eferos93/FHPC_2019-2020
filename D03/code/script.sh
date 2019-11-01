for procs in 1 2 4 8 16 32 64 ; do
 time mpirun -np ${procs} mpi_pi.x 1000000
 done