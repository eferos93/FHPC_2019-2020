module load gnu
echo "SERIAL" &>> FHPC_2019-2020/Assignements/Assignment02/binary_search_output.txt
(FHPC_2019-2020/Assignements/Assignment02/./binary_search_serial.x 1000000000 100000000) &>> FHPC_2019-2020/Assignements/Assignment02/binary_search_output.txt
echo "PARALLEL" &>> FHPC_2019-2020/Assignements/Assignment02/binary_search_output.txt
for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do
	export OMP_NUM_THREADS=${threads}
	(FHPC_2019-2020/Assignements/Assignment02/./binary_search_parallel.x 1000000000 100000000) &>> FHPC_2019-2020/Assignements/Assignment02/binary_search_output.txt
done
