
/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ contact: luca.tornatore@inaf.it                                            │
 │                                                                            │
 │     This is free software; you can redistribute it and/or modify           │
 │     it under the terms of the GNU General Public License as published by   │
 │     the Free Software Foundation; either version 3 of the License, or      │
 │     (at your option) any later version.                                    │
 │     This code is distributed in the hope that it will be useful,           │
 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 │     GNU General Public License for more details.                           │
 │                                                                            │
 │     You should have received a copy of the GNU General Public License      │
 │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 │                                                                            │
 * ────────────────────────────────────────────────────────────────────────── */


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#if !defined(_OPENMP)
#error "OpenMP support required for this code"
#endif
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>


#define N_default 1000


#define CPU_ID_ENTRY_IN_PROCSTAT 39
#define HOSTNAME_MAX_LENGTH      200


int main( int argc, char **argv )
{

  int nthreads           = 1;
  int nthreads_requested = 1;
  

  if ( argc > 1 )
    nthreads_requested = atoi( *(argv+1) );

  if ( nthreads_requested > 1 )
    omp_set_num_threads( nthreads_requested ); 

  char *places = getenv("OMP_PLACES");
  char *bind   = getenv("OMP_PROC_BIND");
  if ( places != NULL )
    printf("OMP_PLACES is set to %s\n", places);
  if ( bind != NULL )
    printf("OMP_PROC_BINDING is set to %s\n", bind);

  
#pragma omp parallel
  {
    
#pragma omp master
    {
      nthreads = omp_get_num_threads();
      printf("+ %d threads in execution - -\n", nthreads );
    }
    int me      = omp_get_thread_num();

    // get on what "place" this thread is running ong
    int place   = omp_get_place_num();

    // get how many places are available in the place list
    int nplaces = omp_get_num_places();    

    /* int proc_ids[nplaces]; */
    /* omp_get_place_proc_ids( place, proc_ids ); */
    
    // get the number of procs available at this place
    int nprocs  = omp_get_place_num_procs(place);
    
    // get how many places are available in the place list
    int npplaces = omp_get_partition_num_places();

    
#pragma omp critical
    printf("thread %2d: place %d, nplaces %d, nprocs %d, npplaces %d\n",
	   me, place, nplaces, nprocs, npplaces );

#ifdef SPY
    #define REPETITIONS 10000
    #define ALOT        10000000000
    long double S = 0;
    for( int j = 0; j < REPETITIONS; j++ )
      #pragma omp for
      for( unsigned long long i = 0; i < ALOT; i++ )
	S += (long double)i;
#endif
  }

  return 0;
}







