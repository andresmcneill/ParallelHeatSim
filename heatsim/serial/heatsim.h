//#include <mips.h> FIXME add this in parallel version FIXME
#include <stdio.h>
#include <stdlib.h>

#ifndef HEATSIM_H
#define HEATSIM_H

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

//	These values are specified by input from stdin and are fixed after first assignment.
int simwidth;		//	Physical dimensions of simulation
int simheight;
int iterations;		//	Number of times simulation is run.

void init_sim(double **new, double starttemp, double sourcetemp, double sourcewidth);

//	Serialized calculate_new function
void calculate_new(double **new, double **old);

//	Serialized copy function
void copy_new_to_old(double **new, double **old);

void printsim(FILE *fp, double **simarray, char *colors[]);

#endif
