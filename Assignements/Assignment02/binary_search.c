
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



#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <omp.h>

#ifdef _OPENMP
  #define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
	                  (double)ts.tv_nsec * 1e-9)
#else
  #define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
	                  (double)ts.tv_nsec * 1e-9)
#endif
	                
                    
#define _GNU_SOURCE

int mybsearch(int *data, int N, int Key)
 {
   int register low = 0;
   int register high = N;
   int register mid;

   while(low <= high) {     
     mid = (low + high) / 2;
     __builtin_prefetch (&data[(mid + 1 + high)/2], 0, 3);
     __builtin_prefetch (&data[(low + mid - 1)/2], 0, 3);     
     if(data[mid] < Key)
       low = mid + 1; 
     else if(data[mid] > Key)
       high = mid - 1;
     else 
       return mid;
   }

   /* if ( Key == data[low] ) */
   /*   return 0; */
   /* else */
     return -1;
 }

#define N_DEFAULT  (1024*1024*128)
#define N_search_DEFAULT (N_DEFAULT / 10)

int main(int argc, char **argv)
{
  int N, Nsearch, i, n_threads = 1;
  int *data, *search;

  #ifndef _OPENMP
    printf("serial binary search\n");
  #else
  #pragma omp parallel
  {
    #pragma omp master
    {
      n_threads = omp_get_num_threads();
      printf("omp binary search with %d threads\n", n_threads );
    }
  }
  #endif

  if(argc > 1)
    N = atoi( *(argv+1) );
  else
    N = N_DEFAULT;

  if(argc > 2)
    Nsearch = atoi ( *(argv + 2) );
  else
    Nsearch = N_search_DEFAULT;

  printf("performing %d lookups on %d data..\n", Nsearch, N);

  printf("set-up data.."); fflush(stdout);
  data = (int*)malloc(N * sizeof(int));

  #if defined(_OPENMP)
    #pragma omp parallel for
      for (i = 0; i < N; i++)
        data[i] = i;
  #else
    for(i = 0; i < N; i++)
      data[i] = i;
  #endif

  printf(" set-up lookups.. "); fflush(stdout);  
  search = (int*)malloc(Nsearch * sizeof(int));
  srand(time(NULL));

  #if defined(_OPENMP)
   #pragma omp parallel for
      for (i = 0; i < Nsearch; i++)
        search[i] = rand() % N;
  #else
    for (i = 0; i < N; i++)
      search[i] = rand() % N;
  #endif
    

  int found = 0;
  double tstart, tstop;
  struct timespec ts;

  printf("\nstart cycle.. "); fflush(stdout);
  tstart = CPU_TIME;

  #if defined(_OPENMP)
    #pragma omp parallel for
    for (i = 0; i < Nsearch; i++)
      if( mybsearch(data, N, search[i]) >= 0)
        found++;
  #else
    for ( i = 0; i < Nsearch; i++)
      if(mybsearch(data, N, search[i]) >= 0)
        found++;
  #endif
    
  tstop = CPU_TIME;

  printf("time elapsed: %g\n", tstop - tstart);

  //free(data);
  //free(search);

  return 0;
 }
