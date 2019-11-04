#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

int main(int argc , char *argv[ ]) {
    
    
    long long  int array_length = 0;
    double start = 0.0, end = 0.0;
    double start_proc = MPI_Wtime(), end_proc=0.0;
    
    long long  int local_sum = 0;
    int myid , numprocs , proc, i;

    MPI_Status status;
    MPI_Request request;
    
    int tag = 123;
    int master = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    
    if(myid == master) { 
        start = MPI_Wtime();
        FILE* file = fopen ("./input.txt", "r");
        fscanf (file, "%llu", &array_length);    
        while (!feof (file))
        {  
            fscanf (file, "%llu", &array_length);      
        }
        fclose (file);        
        end = MPI_Wtime();
        printf("T_read: %f\n", end-start);
    }


    long long  int interval = array_length/numprocs;
    int remainder = array_length % numprocs;

    if(myid == master) {
        long long int *sum = (long long int*)malloc(sizeof(sum));
        *sum = 0;
        long long  int *array = (long long  int*)malloc( (interval+remainder) * sizeof(long long  int));
        
        for(i=0; i< interval+remainder; i++) {
            array[i] = i;
            if(i == 0)
                start = MPI_Wtime();
            *sum = *sum + array[i];
            if(i == 0)
                end = MPI_Wtime();
        }
        
        printf("T_comp: %f\n", end-start);

        for(proc=1; proc<numprocs; proc++) {
            if(proc == 1)
                start = MPI_Wtime();
            MPI_Send(&interval , sizeof(interval) ,MPI_LONG_LONG, proc , proc ,MPI_COMM_WORLD);
            if(proc == 1)
                end = MPI_Wtime();
        }
        printf("T_comm master to slave: %f\n", end-start);

        for(proc=1; proc<numprocs; proc++) {
            MPI_Recv(&local_sum, sizeof(local_sum), MPI_LONG_LONG, proc, proc, MPI_COMM_WORLD, &status);
            *sum = *sum + local_sum;
        }
        end_proc = MPI_Wtime();
        printf("Master Wtime: %f\n", end_proc-start_proc);
        printf("The total sum is %llu\n", *sum);
    } else {
        MPI_Recv(&interval,sizeof(interval), MPI_LONG_LONG, master, myid, MPI_COMM_WORLD, &status);

        long long int value = interval;        
        long long int *array = (long long int*) malloc(interval*sizeof(long long int));
        for(i= 0; i < interval; i++) {
            array[i] = value*myid+i+remainder;
            
        }
        
        for(i = 0; i< interval; i++) {
            local_sum += array[i];
        }
        start = MPI_Wtime();
        MPI_Send(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, master, myid, MPI_COMM_WORLD);
        end = MPI_Wtime();
        end_proc = MPI_Wtime();
        printf("T_comm slave to master: %f\n", end-start);
        printf("WTime proc %i: %f\n", myid, end_proc-start_proc);
    }

    MPI_Finalize();

}