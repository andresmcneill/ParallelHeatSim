#include <stdio.h>
#include <stdlib.h>
//#include <mpi.h> FIXME include this
#include <math.h>
#include <time.h>

/*	Program overview:
	Given np processes, number of samples per process should be samples/np
	Calculate partial count on each process, then use MPI_Reduce to gather and
	sum the partial counts.
	Print the elapsed time on process 0.
	Print the estimated value for pi on process 0.

 */

int main(int argc, char *argv[]) {
	int i, count;	//	Points inside the unit quarter circle
	double x, y;	//	Coordinates of points
	int samples;	//	Samples number of points to generate
	double pi;		//	Estimate of pi
	int rank, numtasks;

	samples = atoi(argv[1]);

	count = 0;
	for (i = 0; i < samples; i++) {
		x = (double) rand() / RAND_MAX;
		y = (double) rand() / RAND_MAX;
		if (x*x + y*y <= 1)
			count++;
	}

	pi = 4.0 * (double)count/(double)samples;
	printf("Count = %d, Samples = %d, Estimate of pi = %7.5f\n", count, samples, pi);
	return 0;
}
