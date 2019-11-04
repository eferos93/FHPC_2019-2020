#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#define USE MPI
#define SEED 35791246

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
    
    long long  int N;
    long long  int local_sum = 0, sum = 0;
    int myid , numprocs , proc;

    MPI_Status status;
    MPI_Request request;
    
    long long  int index;
    int master = 0;
    int tag = 123;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    long long  int interval = array_length/numprocs;
     int remainder = array_length % numprocs;

    printf("Interval: %llu ", interval);

    

    if(myid == 0) {
        printf("master interval %llu\n", interval);
        
        
        long long  int *array = (long long  int*)malloc( interval * sizeof(long long  int));
        for(int i=0; i< interval; i++) {
            array[i] = i;
            sum += array[i];
        }
        printf("master's sum %llu\n", sum);

        for(int proc=1; proc<numprocs; proc++) {
            MPI_Send(&interval , sizeof(interval) ,MPI_UNSIGNED_LONG_LONG, proc , tag ,MPI_COMM_WORLD) ;
            
        }

        for(int proc=1; proc<numprocs; proc++) {
            MPI_Recv(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, proc, tag, MPI_COMM_WORLD, &status);
            printf("local sum received %llu\n", local_sum);
            sum += local_sum;
        }
        printf("The total sum is %llu\n", sum);
    } else {
        printf("I am proc %i\n", myid);
        MPI_Recv(&interval,sizeof(interval), MPI_UNSIGNED_LONG_LONG, master, tag, MPI_COMM_WORLD, &status);
        printf("i have received: %llu\n", interval);
        if(myid < remainder) {
            interval++;
        }

        long long int *array = (long long int*) malloc(interval*sizeof(long long int));
        for(int i= 0; i< interval; i++) {
            array[i] = i+interval*myid;
        }
        
        for(int i = 0; i< interval; i++) {
            local_sum += array[i];
            printf("arr[%d] = %llu\n", i, array[i]);
        }

        MPI_Send(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, master, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();

}