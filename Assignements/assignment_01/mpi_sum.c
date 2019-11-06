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
    long long int begin, last;
    long long  int local_sum = 0;
    int myid , numprocs , proc, i;

    MPI_Status status;
    MPI_Request request;
    
    int tag = 1;
    int master = 0;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    
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


    long long  int interval = array_length/numprocs;
    int remainder = array_length % numprocs;
    
    if(myid == master) {
        //somehow if the variable sum is allocated in the stack, 
        //it was resetting its value at each iterations when adding
        long long int *sum = (long long int*)malloc(sizeof(sum));
        *sum = 0;
        long long int *array = (long long  int*)malloc( (interval) * sizeof(long long  int));

        for(i=0; i< interval; i++) {
            array[i] = i;
            if(i == 0)
                start = MPI_Wtime();
            *sum = *sum + array[i];
            if(i == 0)
                end = MPI_Wtime();
        }
        printf("T_comp: %f\n", end-start);

        begin = array[interval-1]+1;
        for(proc=1; proc<numprocs; proc++) {
            

            last = begin + interval - 1;
            //distrubute equally the array elements not captured by array_length/nprocs
            if(remainder>0) {
                last++;
                remainder--;
            }
            if(proc == 1) {
                start = MPI_Wtime();
            }
            MPI_Send(&begin , sizeof(begin) ,MPI_LONG_LONG, proc , tag ,MPI_COMM_WORLD);
            MPI_Send(&last , sizeof(last) ,MPI_LONG_LONG, proc , tag ,MPI_COMM_WORLD);
            if(proc == 1){
                end = MPI_Wtime();
            }

            begin = last+1;
            
        }
        printf("T_comm master to slave: %f\n", end-start);

        for(proc=1; proc<numprocs; proc++) {
            MPI_Recv(&local_sum, sizeof(local_sum), MPI_LONG_LONG, proc, tag, MPI_COMM_WORLD, &status);
            printf("Master received: %lli; from: %i\n", local_sum, proc);
            *sum = *sum + local_sum;
            printf("Sum after recv from proc %i: %lli", proc, *sum);
        }
        end_proc = MPI_Wtime();
        printf("Master Wtime: %f\n", end_proc-start_proc);
        printf("The total sum is %lli\n", *sum);
    } else {
        MPI_Recv(&begin,sizeof(begin), MPI_LONG_LONG, master, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&last,sizeof(last), MPI_LONG_LONG, master, tag, MPI_COMM_WORLD, &status);
        interval = last-begin+1;
        printf("Proc: %i; Begin: %lli; Last: %lli\n", myid, begin, last);
        long long int *array = (long long int*) malloc(interval*sizeof(long long int));

        for(i= 0; i < interval; i++) {

            array[i] = begin++;
        }
        
        for(i = 0; i< interval; i++) {
            local_sum += array[i];
        }
        start = MPI_Wtime();
        MPI_Ssend(&local_sum, sizeof(local_sum), MPI_LONG_LONG, master, tag, MPI_COMM_WORLD);
        end = MPI_Wtime();
        end_proc = MPI_Wtime();
        printf("T_comm slave to master: %f\n", end-start);
        printf("WTime proc %i: %f\n", myid, end_proc-start_proc);
    }

    MPI_Finalize();

}