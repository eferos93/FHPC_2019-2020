#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

int main(int argc , char *argv[ ]) {
    
    FILE* file = fopen ("./input.txt", "r");
    long long  int array_length = 0;

    fscanf (file, "%llu", &array_length);    
    while (!feof (file))
    {  
      printf ("%llu ", array_length);
      fscanf (file, "%llu", &array_length);      
    }
    fclose (file);        
    printf("p = %llu", array_length);
    
    long long  int local_sum = 0, offset;
    int myid , numprocs , proc, i;

    MPI_Status status;
    MPI_Request request;
    
    int tag = 123;
    int master = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    long long  int interval = array_length/numprocs;
    int remainder = array_length % numprocs;

    printf("Interval: %llu ", interval);

    

    if(myid == master) {
        printf("master interval %llu\n", interval);
        long long int *sum = (long long int*)malloc(sizeof(sum));
        *sum = 0;
        long long  int *array = (long long  int*)malloc( (interval+remainder) * sizeof(long long  int));
        for(i=0; i< interval+remainder; i++) {
            array[i] = i;
            *sum = *sum + array[i];
        }
        printf("master's sum %llu\n", *sum);

        for(proc=1; proc<numprocs; proc++) {
            
            MPI_Send(&interval , sizeof(interval) ,MPI_LONG_LONG, proc , proc ,MPI_COMM_WORLD);
            
            
        }

        for(proc=1; proc<numprocs; proc++) {
            MPI_Recv(&local_sum, sizeof(local_sum), MPI_LONG_LONG, proc, proc, MPI_COMM_WORLD, &status);
            printf("local sum received %llu\n", local_sum);
            printf("sum before %llu\n", *sum);
            *sum = *sum + local_sum;
            printf("sum: %llu\n", *sum);

        }
        printf("The total sum is %llu\n", *sum);
    } else {
        MPI_Recv(&interval,sizeof(interval), MPI_LONG_LONG, master, myid, MPI_COMM_WORLD, &status);

        long long int value = interval;
        printf("proc: %i; i have received: %llu; my remainder: %i\n", myid, interval, remainder);
        
        long long int *array = (long long int*) malloc(interval*sizeof(long long int));
        for(i= 0; i < interval; i++) {
            array[i] = value*myid+i+remainder;
            
        }
        
        for(i = 0; i< interval; i++) {
            local_sum += array[i];
            printf("arr[%d] = %llu\n", i, array[i]);
        }

        MPI_Send(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, master, myid, MPI_COMM_WORLD);
    }

    MPI_Finalize();

}