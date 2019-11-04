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
    int myid , numprocs , proc;

    MPI_Status status;
    MPI_Request request;
    
    long long unsigned int index;
    int master = 0;
    int tag = 123;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    long long unsigned int array_length = 10;
    long long unsigned int *arr;
    long long unsigned int interval = array_length/numprocs;
    long long unsigned int index_middle;
    printf("Interval: %llu ", interval);

    for(int i = 0; i<array_length; i++) {
        arr[i] = i;
    }
    

    if(myid == 0) {
        printf("master interval %llu\n", interval);
        unsigned int remainder = array_length % numprocs;
        index = interval + remainder;
        index_middle = index*(myid+1) + (array_length - interval)/2;
        //for(int i = 0; i< array_length; i++) {
          //  printf("a[%d] = %llu\n", i, arr[i]);
        //}
        for(int i=0; i< index; i++) {
            sum += arr[i];
        }
        printf("master's sum %llu\n", sum);
        /*MPI_Send(&index , sizeof(index), MPI_UNSIGNED_LONG_LONG, 1 , tag ,MPI_COMM_WORLD);
        MPI_Send(&index_middle , sizeof(index_middle), MPI_UNSIGNED_LONG_LONG, 2 , tag ,MPI_COMM_WORLD);

        MPI_Recv(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, 1, tag, MPI_COMM_WORLD, &status);
        sum += local_sum;
        MPI_Recv(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, 2, tag, MPI_COMM_WORLD, &status);
        sum += local_sum;*/

        for(int proc=1; proc<numprocs; proc++) {
            MPI_Send(&index , sizeof(index) ,MPI_UNSIGNED_LONG_LONG, proc , tag ,MPI_COMM_WORLD) ;
            index += interval;
        }

        for(int proc=1; proc<numprocs; proc++) {
            MPI_Recv(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, proc, tag, MPI_COMM_WORLD, &status);
            printf("local sum recv from %llu\n", local_sum);
            sum += local_sum;
        }
        printf("The total sum is %llu\n", sum);
    } else {
        printf("I am proc %i\n", myid);
        MPI_Recv(&index,sizeof(index), MPI_UNSIGNED_LONG_LONG, master, tag, MPI_COMM_WORLD, &status);
        printf("i have received: %llu\n", index);
        for(int i = 0; i< array_length; i++) {
            printf("arr[%d] = %llu", i, arr[i]);
        }
        /*if(proc*2+1 < numprocs) {
            for(int i = index; i < interval * myid; i++) {
                local_sum += arr[i];
            }
            index += interval;
            index_middle = index + 

        } else {

        }*/
        //long long unsigned int* ptr = &arr;
        //printf("%p", ptr);
        //printf("a[%llu] = %llu", index, arr[index]);
        /*for(int i = index; i<interval*(myid+1); i++) {
            printf("a[%d] = %llu\n", i, arr[i]);
            local_sum += arr[i];
        }*/
        MPI_Send(&local_sum, sizeof(local_sum), MPI_UNSIGNED_LONG_LONG, master, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();

}