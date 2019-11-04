#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[]){
	
	int myid, proc, numprocs = 0;
	unsigned long long int i, j, N, dim, partial_sum = 0;
	unsigned long long int* array;
	double remainder, start_comm, end_comm, start_comp, end_comp = 0.0;
	MPI_Status status;
	std::fstream fd;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	 
	start_comm = MPI_Wtime(); // communication time
	if(myid == 0){
			
		// read the dimension of the problem from a file
		start_comm = MPI_Wtime();
		fd.open("./input.txt", std::ios::in);
		fd >> dim;
		fd.close();
		end_comm = MPI_Wtime();
		std::cout << "T_read: " << end_comm - start_comm << std::endl;
		if ( dim <=1 ) {
			std::cout << "Dim must be at least equal to 2";
			exit(-1);
		} 
			
		for(i=1; i<numprocs; i++){ // root sends to everyone the size of the problem
			MPI_Ssend(&dim, 1, MPI_LONG_LONG, i, 10, MPI_COMM_WORLD);
			//printf("%d: sended to %d\n", myid, i);
		}

	} else {
		// processes receive from the root...
		MPI_Recv(&dim, 1, MPI_LONG_LONG, 0, 10, MPI_COMM_WORLD, &status);
		//printf("%d: recieved from 0: %ld\n", myid, dim);
	}
	start_comp = MPI_Wtime(); // computation time
	N=dim/numprocs; remainder=dim%numprocs; // determine the size of subproblem (and the rest)
	//printf("%d: N = %ld, R = %2.2f\n", myid, N, R);
	if(myid < remainder) {
		N++; // the process increases the size to include the rest
		array = new unsigned long long int[N]; // everyone allocate the array with its own size
		for(i=1; i <= N; i++){
			*(array-1+i) =i+N*myid;
		}
	} else {
		array = new unsigned long long int[N];
		for(i=1; i <= N; i++){
			*(array-1+i)=i+N*myid+remainder;
		}
	}
	for(i=0; i<N; i++){
		partial_sum += *(array+i);
	}
	   
	//printf("%d: partial_sum = %lld\n", myid, partial_sum);
	delete [] array;
	   
	if(myid==0) {
		array = new unsigned long long int[numprocs-1]; // buffer of the sums
		for(i=1; i<numprocs; i++){
			MPI_Recv((array+i-1), 1, MPI_LONG_LONG, i, 10, MPI_COMM_WORLD,&status) ;
			//printf("received from %ld: %ld\n", i, array[i-1]);
		}
		end_comm = MPI_Wtime(); // end communication
		for(i=0; i<numprocs-1; i++){
			partial_sum += *(array+i);
		}
		delete [] array;
		end_comp = MPI_Wtime(); // end computation
		std::cout << "final sum: " << partial_sum << std::endl;
		std::cout << "T_comm master: " << end_comm - start_comm << std::endl;
		std::cout << "T_comp master: " << end_comp - start_comp << std::endl;
	} else {
		end_comp = MPI_Wtime();
		MPI_Ssend(&partial_sum, 1 , MPI_LONG_LONG, 0, 10 , MPI_COMM_WORLD);
		//printf("sended to 0: %ld\n", partial_sum);
		end_comm=MPI_Wtime();

		std::cout << "T_comp proc" << myid << end_comp - start_comp << std::endl;
		std::cout << "T_comm proc" << myid << end_comm -start_comm << std::endl;
	}
	   
	MPI_Finalize();
	return 0;
}
