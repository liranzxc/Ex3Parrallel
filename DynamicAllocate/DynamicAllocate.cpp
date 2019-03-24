#include "DynamicAllocate.h"
#include <mpi.h>
#include <stdio.h>
#include <math.h>

#define HEAVY 100000
#define N 20
#define MasterP 0
#define DATA_TAG 1
#define EXIT_TAG 0

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

double DoWork(int input) {

	double temp = 0;

	for (int y = 0; y < N; y++)
	{
		temp += heavy(input, y);
	}
	return temp;

}
bool moreJob(int * numofTasks)
{
	if (*numofTasks >= 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main(int argc, char *argv[])
{

	int  namelen, numprocs, myid;
	double answer = 0, temp;
	int num_of_process_that_exit = 0;

	int jobTasks = N;

	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Get_processor_name(processor_name, &namelen);

	MPI_Status status;

	// my code

	// master
	if (myid == MasterP)
	{
		// sending to worker init work

		for (int i = 1; i < numprocs; i++)
		{
			// send to process i a init task - one number with DATA TAG
			MPI_Send(&jobTasks, 1, MPI_INT, i, DATA_TAG, MPI_COMM_WORLD);
			jobTasks--;
		}

		while (num_of_process_that_exit != numprocs - 1)
		{
			// receive from any slave answer
			MPI_Recv(&temp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, DATA_TAG, MPI_COMM_WORLD, &status);

			answer += temp;

			if (moreJob(&jobTasks))
			{
				MPI_Send(&jobTasks, 1, MPI_INT, status.MPI_SOURCE, DATA_TAG, MPI_COMM_WORLD);
				jobTasks--;
			}
			else
			{

				// send null of something else
				MPI_Send(&jobTasks, 1, MPI_INT, status.MPI_SOURCE, EXIT_TAG, MPI_COMM_WORLD);
				num_of_process_that_exit++;
			}
		}

		printf("Answer is %e \n", answer);


	}
	else // slave code
	{
		int Exited = 0;
		int input;

		while (!Exited)
		{
			MPI_Recv(&input, 1, MPI_INT, MasterP, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

			switch (status.MPI_TAG)
			{
			case EXIT_TAG:
				// exiting
				//printf("Exiting with input %d \n ", input);
				Exited = 1;
				break;

			case DATA_TAG:
				//printf("Do Work id %d with input : %d \n", myid, input);
				temp = DoWork(input);
				MPI_Send(&temp, 1, MPI_DOUBLE, MasterP, DATA_TAG, MPI_COMM_WORLD);
				break;

			
			}

		}


	}



	MPI_Finalize();
	return 0;
}

