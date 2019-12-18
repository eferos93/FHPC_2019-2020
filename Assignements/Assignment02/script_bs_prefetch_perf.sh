module load gnu
export OMP_NUM_THREADS=20
perf record --call-graph -e cache-misses FHPC_2019-2020/Assignements/Assignment02/./binary_search_parallel_prefetch.x 1000000000 1000000000

