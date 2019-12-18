module load gnu
export OMP_NUM_THREADS=20
perf stat FHPC_2019-2020/Assignements/Assignment02/./01_array_sum_parallel.x 10000000000
perf stat FHPC_2019-2020/Assignements/Assignment02/./04_touch_by_all.x 10000000000
