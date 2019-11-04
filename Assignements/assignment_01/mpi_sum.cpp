#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char ** argv)
{
	long long unsigned int res,dst = 0,tag = 0,i;
    long long unsigned int dim = 0;
    int myrank, p;
    int remainder;
    std::fstream fd;

    fd.open("./input.txt", std::ios::in);
	fd >> dim;
	fd.close();
    long long unsigned int* array = new long long unsigned int[dim];
    long long unsigned int interval = dim/p;
    /*long long unsigned int* iter, stop;
    iter = array;
    stop = array + dim;
	for(;iter < stop;iter++)
		*(iter)= i;
    */
    for(int i = 0; i < dim; i++) {
        array[i] = i;
    }
	
	MPI_Status status;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	if(myrank != 0)
	{
		long long unsigned int x, y;
		x = interval*(myrank);
		y = interval*(myrank+1)-1;

		res = 0;
        /*iter = array + x;
        stop = array + y;
		for(;iter<stop;iter++)
			res+=*(iter);*/
        for(i = x; i<=y; y++) {
            res+=array[i];
        }
		std::cout<<"sum "<< res << std::endl;
		MPI_Send(&res, sizeof(res), MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
        long long unsigned int sum = 0;
        /*iter = array;
        stop = array + interval;
        for (; iter<stop; iter++) {
            sum += *(iter);
        }*/
        for (int i = 0; i < interval; i++) {
            sum+= array[i];
        }
		for(int src= 1; src< p; src++)
		{
			MPI_Recv(&res, sizeof(res), MPI_INT, src, 0, MPI_COMM_WORLD, &status);
			std::cout << "sum recieved from the process: "<< src << " is " << res << std::endl;
			sum += res;
		}
	 	std::cout << "the total sum is " << sum << std::endl;
	 }
	 
	MPI_Finalize();
}	