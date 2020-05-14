#include "pheatsim.h"

//  Initializes each process's segment, and initializes the heat source for process 1.
void init_sim(int myheight, int simwidth, double new[myheight][simwidth], double starttemp, double sourcetemp, double sourcewidth, int rank) {
	//	Initialize entire array with starttemp
	for (int i = 0; i < myheight; i++) {
		for (int j = 0; j < simwidth; j++) {
			new[i][j] = starttemp;
		}
	}

	//	Initialize the heat source with sourcetemp in process 0 (top strip)
    //  (Actually uses 2nd strip as the top row is unused ghost points)
    if (rank == 0) {
        for (int i = (int)(simwidth * ((1 - sourcewidth)/2));
             i < (int)(simwidth * (((1 - sourcewidth)/2) + sourcewidth)); i++) {
            new[1][i] = sourcetemp;
        }
    }
}

//  Takes average temperature of the surrounding points to generate the new temperature.
//  Function assumes that there are ghost points at each connecting edge which represent the closest row 
//  from an adjacent process.
void calculate_new(int myheight, int simwidth, double new[myheight][simwidth], double old[myheight][simwidth], int rank, int size, double sourcewidth, double sourcetemp) {
    
    /*  I couldn't get rid of a bug where my heat source kept getting overwritten with each iteration through
        the main loop, so I wrote the heat source to the top row every time this function is called from process 0.
        It's not ideal but the output is the same. This is why I pass in sourcewidth and sourcetemp when I normally
        wouldn't need to.  */
    if (rank == 0) {
        for (int i = (int)(simwidth * ((1 - sourcewidth)/2));
             i < (int)(simwidth * (((1 - sourcewidth)/2) + sourcewidth)); i++) {
            old[1][i] = sourcetemp;
        }
    }
    //  Top segment has unused ghost row at top
    if (rank == 0) {
        for (int i = 2; i < myheight - 1; i++) {
            for (int j = 1; j < simwidth - 1; j++) {
                new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]);
            }
        }
    }
    //  Middle segments calculate at all non-edge points
    else if (rank != size - 1 && rank != 0) {
        for (int i = 1; i < myheight - 1; i++) {
            for (int j = 1; j < simwidth - 1; j++) {
                new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]);
            }
        }
    }
    //  Bottom segment has unused ghost row at bottom
    else if (rank == size - 1) {
        for (int i = 1; i < myheight - 2; i++) {
            for (int j = 1; j < simwidth - 1; j++) {
                new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]);
            }
        }
    }
}

//  Straightforward 2d array copy function
void copy_new_to_old(int myheight, int simwidth, double new[myheight][simwidth], double old[myheight][simwidth]) {
	for (int i = 0; i < myheight; i++) {
		for (int j = 0; j < simwidth; j++) {
			old[i][j] = new[i][j];
		}
	}
}

//  Assumes ghost row implementation (extra rows at top and bottom to represent the edge rows of adjacent processes
void row_exchange(int myheight, int simwidth, double strip[myheight][simwidth], int rank, int size, MPI_Status *status) {
    //  I used the row_exchange.c algorithm from the slides and slightly modified the send/receive orders
    if (rank == 0) {
        MPI_Send(strip[myheight-2], simwidth, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
        MPI_Recv(strip[myheight-1], simwidth, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, status);
    } else if (rank != size - 1) {
        MPI_Recv(strip[0],simwidth, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, status);
        MPI_Send(strip[1],simwidth, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD);
        
        MPI_Send(strip[myheight-2], simwidth, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
        MPI_Recv(strip[myheight-1], simwidth, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, status);
    } else if (rank == size - 1) {
        MPI_Recv(strip[0], simwidth, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, status);
        MPI_Send(strip[1], simwidth, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD);
    }
}

//	Assumes pnm header has already been printed
//  Change implementation to skip printing the ghost points
void printsim(FILE *fp, int simheight, int simwidth, double sim[simheight][simwidth], char *colors[]) {
	for (int i = 0; i < simheight; i++) {
    	for (int j = 0; j < simwidth; j++) {
            //  Choose color based on temperature
			if (sim[i][j] <= 20.0) {
				fprintf(fp, "%s ", colors[9] );
			} else if (sim[i][j] >= 20.0 && sim[i][j] < 30.0) {
				fprintf(fp, "%s ", colors[8] );
			} else if (sim[i][j] >= 30.0 && sim[i][j] < 40.0) {
				fprintf(fp, "%s ", colors[7] );
			} else if (sim[i][j] >= 40.0 && sim[i][j] < 50.0) {
				fprintf(fp, "%s ", colors[6] );
			} else if (sim[i][j] >= 50.0 && sim[i][j] < 60.0) {
				fprintf(fp, "%s ", colors[5] );
			} else if (sim[i][j] >= 60.0 && sim[i][j] < 80.0) {
				fprintf(fp, "%s ", colors[4] );
			} else if (sim[i][j] >= 80.0 && sim[i][j] < 120.0) {
				fprintf(fp, "%s ", colors[3] );
			} else if (sim[i][j] >= 120.0 && sim[i][j] < 180.0) {
				fprintf(fp, "%s ", colors[2] );
			} else if (sim[i][j] >= 180.0 && sim[i][j] < 250.0) {
				fprintf(fp, "%s ", colors[1] );
			} else if (sim[i][j] >= 250.0) {
				fprintf(fp, "%s ", colors[0] );
			}
		}
		fprintf(fp, "\n");
    }
}
