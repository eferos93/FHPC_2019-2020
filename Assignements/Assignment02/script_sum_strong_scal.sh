for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do
	export OMP_NUM_THREADS = ${threads}
	./01_array_sum_parallel.x 1000000000
done
