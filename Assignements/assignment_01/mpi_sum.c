#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#define USE MPI
#define SEED 35791246

int main(int argc , char *argv[ ]) {
    /*
    FILE *fp;
    char str[MAXCHAR];
    char* filename = "c:\\temp\\test.txt";
 
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    while (fgets(str, MAXCHAR, fp) != NULL)
        printf("%s", str);
    fclose(fp);
    */
    long long unsigned int N;
    long long unsigned int local_sum = 0, sum = 0;
    long long unsigned int array_length = 10;
    long long unsigned int arr[array_length];
    int myid , numprocs , proc;
    MPI_Status status;
    MPI_Request request;
    
    long long unsigned int index;
    int master = 0;
    int tag = 123;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    long long unsigned int interval = array_length/numprocs;
    
    for(int i = 0; i<array_length; i++) {
        arr[i] = i;
    }
    

    if(myid == 0) {
        printf("master interval %llu\n", interval);
        unsigned int remainder = array_length % numprocs;
        index = interval + remainder;
        for(int i = 0; i< array_length; i++) {
            printf("a[%d] = %llu", i, arr[i]);
        }
        for(int i=0; i< index; i++) {
            sum += arr[i];
        }
        printf("master's sum %llu\n", sum);
        for(int proc=1; proc<numprocs; proc++) {
            index += interval;
            if(proc == 1)
                index += remainder;
            
            MPI_Send(&index , sizeof(index) ,MPI_UNSIGNED_LONG_LONG, proc , tag ,MPI_COMM_WORLD) ;
        }

        for(int proc=1; proc<numprocs; proc++) {
            MPI_Recv(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, proc, tag, MPI_COMM_WORLD, &status);
            printf("local sum recv from %llu\n", local_sum);
            sum += local_sum;
        }
        printf("The total sum is %llu\n", sum);
    } else {
        printf("I am proc %i\n", myid);
        MPI_Recv(&index,sizeof(interval), MPI_UNSIGNED_LONG_LONG, master, tag, MPI_COMM_WORLD, &status);
        for(index; index<interval*myid; index++) {
            local_sum += arr[index];
        }
        MPI_Send(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, master, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();

}