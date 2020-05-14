#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

/*	Andres McNeill
	cpsc 4770
	Fall 2019
	Assignment 1
 */

int main(int argc, char *argv[]) {
	int i, count;	//	Points inside the unit quarter circle
	double x, y;	//	Coordinates of points
	int samples;	//	Samples number of points to generate
	double pi;		//	Estimate of pi
	int rank, numtasks;
	int size;
	int totalcount;
	double start, stop;	//	Timekeeping vars

	start = MPI_Wtime();

	samples = atoi(argv[1]);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	numtasks = samples/size;

	if (rank == 0) {
		numtasks += samples % size;
	}

	count = 0;
	for (i = 0; i < numtasks; i++) {
		x = (double) rand() / RAND_MAX;
		y = (double) rand() / RAND_MAX;
		if (x*x + y*y <= 1)
			count++;
	}

	MPI_Reduce(&count, &totalcount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		pi = 4.0 * (double)totalcount/(double)samples;
		stop = MPI_Wtime();
		printf("Elapsed time: %7.7f\n", stop - start);
		printf("Count = %d, Samples = %d, Estimate of pi = %7.5f\n", totalcount, samples, pi);
	}

	MPI_Finalize();
	return 0;
}
