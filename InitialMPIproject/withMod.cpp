// InitialMPIproject.cpp : Defines the entry point for the console application.
//

/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
*  (C) 2001 by Argonne National Laboratory.
*      See COPYRIGHT in top-level directory.
*/

/* This is an interactive version of cpi */
#include <mpi.h>
#include <stdio.h>
#include <math.h>


#define HEAVY 100000
#define N 20
#define MasterP 0

// This function performs heavy computations, 
// its run time depends on x and y values
double heavy(int x, int y) {
	int i, loop = 1;
	double sum = 0;

	// Super heavy tasks
	if (x < 5 || y < 5)
		loop = 10;
	// Heavy calculations
	for (i = 0; i < loop*HEAVY; i++)
		sum += sin(exp(sin((double)i / HEAVY)));

	return sum;
}


int main(int argc, char *argv[])
{

	int  namelen, numprocs, myid;
	int x, y;
	double answer = 0;
	int ChunkSize, reciveCounter = 0, startIndex, endIndex, mod;
	double temp, StartTime, EndTime;

	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Get_processor_name(processor_name, &namelen);

	MPI_Status status;

	// my code

	StartTime = MPI_Wtime();
	//MasterP configuration
	if (myid == MasterP)
	{
		mod = N % (numprocs - 1); // remainder 

			ChunkSize = N / (numprocs - 1);

			for (int i = 1; i < numprocs - 1; i++)
			{
				startIndex = (i - 1)*ChunkSize;

				endIndex = i*ChunkSize;
        //send to Slaves
				MPI_Send(&startIndex, 1, MPI_INT, i, 0, MPI_COMM_WORLD); // send to process i a SizeChunk
				MPI_Send(&endIndex, 1, MPI_INT, i, 0, MPI_COMM_WORLD); // send to process i a SizeChunk

			}

			// send to last chunk + mod
			int startIndex = (numprocs - 2)*ChunkSize;
			int endIndex = startIndex + ChunkSize + mod;
      //send to Slave with id numprocs-1
			MPI_Send(&startIndex, 1, MPI_INT, numprocs - 1, 0, MPI_COMM_WORLD); // send to last process  a SizeChunk
			MPI_Send(&endIndex, 1, MPI_INT, numprocs - 1, 0, MPI_COMM_WORLD); // send to last process  a SizeChunk


		while (reciveCounter != numprocs - 1)
		{
      //recive from Slaves
			MPI_Recv(&temp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			answer += temp;
			reciveCounter++;
		}


		EndTime = MPI_Wtime();

		printf("Take time :  %e \n", (EndTime - StartTime));
		printf("answer = %e\n", answer);
		printf("Chunk = %d\n", ChunkSize);
		printf("Mod = %d\n", mod);

	}
	//Slave configuration
	else
	{
		int start, end;
    //recive from MasterP
		MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		//int startIndex = (myid - 1)*input;
		//int endIndex = myid*input;
		for (int x = start; x < end; x++)
		{
			for (int y = 0; y < N; y++)
			{
				answer += heavy(x, y);
			}
		}
    //send to MasterP
		MPI_Send(&answer, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);



	}


	// mycode


	MPI_Finalize();
	return 0;
}

