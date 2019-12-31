module load gnu
for threads in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 ; do
	export OMP_NUM_THREADS=${threads}
	FHPC_2019-2020/Assignements/Assignment02/./04_touch_by_all.x 10000000000
done
