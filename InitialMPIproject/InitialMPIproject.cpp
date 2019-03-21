//// InitialMPIproject.cpp : Defines the entry point for the console application.
////
//
///* -*- Mode: C; c-basic-offset:4 ; -*- */
///*
//*  (C) 2001 by Argonne National Laboratory.
//*      See COPYRIGHT in top-level directory.
//*/
//
///* This is an interactive version of cpi */
//#include <mpi.h>
//#include <stdio.h>
//#include <math.h>
//
//
//#define HEAVY 100000
//
//// This function performs heavy computations, 
//// its run time depends on x and y values
//double heavy(int x, int y) {
//	int i, loop = 1;
//	double sum = 0;
//
//	// Super heavy tasks
//	if (x < 5 || y < 5)
//		loop = 10;
//	// Heavy calculations
//	for (i = 0; i < loop*HEAVY; i++)
//		sum += sin(exp(sin((double)i / HEAVY)));
//
//	return sum;
//}
//
//
//int main(int argc, char *argv[])
//{
//
//	int  namelen, numprocs, myid;
//	char processor_name[MPI_MAX_PROCESSOR_NAME];
//
//	MPI_Init(&argc, &argv);
//
//	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
//
//	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
//
//	MPI_Get_processor_name(processor_name, &namelen);
//
//	MPI_Status status;
//	
//	// my code
//
//	int x, y;
//	int N = 20;
//	double answer = 0;
//
//	//printf("num of process %d", numprocs);
//	if (myid == 0)
//	{
//		int ChunkSize = N / (numprocs -1 ); // 20 /  4 = 5
//
//		for (int i = 1; i < numprocs; i++)
//		{
//
//			MPI_Send(&ChunkSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD); // send to process i a SizeChunk
//
//		}
//
//		int reciveCounter = 0;
//		double temp;
//		while (reciveCounter != numprocs - 1)
//		{
//			MPI_Recv(&temp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
//			answer += temp;
//			reciveCounter++;
//		}
//		//printf("Take time :  %e", totalTime);
//		printf("answer = %e\n", answer);
//
//
//	}
//	else
//	{
//		int input;
//		MPI_Recv(&input, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
//
//		int startIndex = (myid - 1)*input;
//		int endIndex = myid*input;
//
//		for (int x = startIndex; x < endIndex; x++)
//		{
//			for (int y = 0; y < N; y++)
//			{
//				answer += heavy(x, y);
//			}
//		}
//		MPI_Send(&answer, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
//
//
//
//	}
//	
//
//	// mycode
//
//
//	MPI_Finalize();
//	return 0;
//}
//
