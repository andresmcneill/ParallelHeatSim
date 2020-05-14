#include "pheatsim.h"

int main(int argc, char *argv[]) {
    
    int rank, size;
    
    //  Since I'm dealing with static arrays I decided to make these values fixed rather than specify them from an input file
    //  at runtime. Should work with many different dimensions and any number of iterations.
    int simwidth = 1000;		//	Physical dimensions of simulation
    int simheight = 1000;
    int iterations = 1500;		//	Number of times simulation is run.
    
    MPI_Status status;
    
    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	double starttemp = 20.0;    //  Base value for all points but source
	double sourcetemp = 300.0;  //  Source temperature, which remains constant
	double sourcewidth = .4;    //  Proportion of top row which is occupied by the heat source

    int myheight;  //  Number of rows this process has as data
    
    //  Assign number of rows evenly, give last process the remainder of rows if remainder > 0.
    if (rank == size - 1) {
        myheight = (simheight / size) + (simheight % size) + 2;
    } else {
        myheight = (simheight / size) + 2;
    }
    
    //  These are used for the call to MPI_Gatherv() at the end.
    int recvcounts[size];  //  Number of doubles passed in from each process to the root
    int disp[size];        //  displacements for each process's data in the receive buffer (printarray)
    
    for (int i = 0; i < size - 1; i++) {
        recvcounts[i] = (simheight / size) * simwidth;
    }
    recvcounts[size - 1] = ((simheight / size) + (simheight % size)) * simwidth;
    
    int sum = 0;
    disp[0] = sum;
    for (int i = 1; i < size; i++) {
        sum += recvcounts[i-1];
        disp[i] = sum;
    }

    // I used the printcolors.c as a guideline for how I colored and printed the data
	/* Colors are list in order of intensity */
	char * colors[10] = {RED, ORANGE, YELLOW, LTGREEN, GREEN,
		LTBLUE, BLUE, DARKTEAL, BROWN, BLACK };

    double simarray[myheight][simwidth];  //  The "old" array, used for calculating the new values
    double newarray[myheight][simwidth];  //  The "new" array, stores the next set of values

	init_sim(myheight, simwidth, newarray, starttemp, sourcetemp, sourcewidth, rank);
	copy_new_to_old(myheight, simwidth, newarray, simarray);
    
	//	Main loop
	for (int k = 0; k < iterations; k++) {
        copy_new_to_old(myheight, simwidth, newarray, simarray);
        if (size > 1) row_exchange(myheight, simwidth, simarray, rank, size, &status);
        MPI_Barrier(MPI_COMM_WORLD);
		calculate_new(myheight, simwidth, newarray, simarray, rank, size, sourcewidth, sourcetemp);
	}
    copy_new_to_old(myheight, simwidth, newarray, simarray);

    double printarray[simheight][simwidth];  //  Array that holds the entire data set for printing
    
    //  Consolidate data into printarray
    //  The ghost rows are not passed into printarray, the first and last rows of each process's segment are left out of
    //  printarray.
    MPI_Gatherv(&(simarray[1][0]), (myheight - 2) * simwidth, MPI_DOUBLE, printarray,
                recvcounts, disp, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //  Write data
    if (rank == 0) {
        FILE * fp = fopen("pheatsim.pnm", "w");
        /* Print the P3 format header */
        fprintf(fp, "P3\n%d %d\n15\n", simwidth, simheight);
        printsim(fp, simheight, simwidth, printarray, colors);
        fclose(fp);
    }
 
    MPI_Finalize();

	return 0;
}
