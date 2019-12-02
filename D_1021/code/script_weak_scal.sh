n=(1000000000 2000000000 4000000000 8000000000 10000000000)
  

for j in 1 2 4 ; do
   i=0
   for procs in 2 4 8 16 20 ; do
      /usr/bin/time mpirun -np ${procs} mpi_pi.x $((${n[$i]} * $j))
      let "i = i + 1"
   done
done
