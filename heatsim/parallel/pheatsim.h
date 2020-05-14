#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PHEATSIM_H
#define PHEATSIM_H

/* 10 colors plus white are defined.  Many more are possible. */
#define WHITE    "15 15 15 "
#define RED      "15 00 00 "
#define ORANGE   "15 05 00 "
#define YELLOW   "15 10 00 "
#define LTGREEN  "00 13 00 "
#define GREEN    "05 10 00 "
#define LTBLUE   "00 05 10 "
#define BLUE     "00 00 10 "
#define DARKTEAL "00 05 05 "
#define BROWN    "03 03 00 "
#define BLACK    "00 00 00 "

//  Initializes each process's individual horizontal strip of data.
void init_sim(int myheight, int simwidth, double new[myheight][simwidth], double starttemp, double sourcetemp, double sourcewidth, int rank);

//	Parallelized calculate_new function
void calculate_new(int myheight, int simwidth, double new[myheight][simwidth], double old[myheight][simwidth], int rank, int size,/*FIXME this is garbage programming lol*/ double sourcewidth, double sourcetemp);

//	Parallelized copy function
void copy_new_to_old(int myheight, int simwidth, double new[simwidth][myheight], double old[simwidth][myheight]);

void row_exchange(int myheight, int simwidth, double strip[myheight][simwidth], int rank, int size, MPI_Status *status);

void printsim(FILE *fp, int simheight, int simwidth, double simarray[simwidth][simheight], char *colors[]);

#endif
