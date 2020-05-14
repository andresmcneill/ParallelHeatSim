#include "heatsim.h"

void init_sim(double **new, double starttemp, double sourcetemp, double sourcewidth) {
	//	Initialize entire array with starttemp
	for (int i = 0; i < simheight; i++) {
		for (int j = 0; j < simwidth; j++) {
			new[i][j] = starttemp;
		}
	}
	//	Initialize the heat source with sourcetemp
	for (int i = (int)(simwidth * ((1 - sourcewidth)/2));
		 i < (int)(simwidth * (((1 - sourcewidth)/2) + sourcewidth)); i++) {
		new[0][i] = sourcetemp;
	}
}

void calculate_new(double **new, double **old) {
	for (int i = 1; i < simheight - 1; i++) {
		for (int j = 1; j < simwidth - 1; j++) {
			new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]);
		}
	}
}

void copy_new_to_old(double **new, double **old) {
	for (int i = 0; i < simheight; i++) {
		for (int j = 0; j < simwidth; j++) {
			old[i][j] = new[i][j];
		}
	}
}

//	Assumes pnm header has already been printed
void printsim(FILE * fp, double **sim, char *colors[]) {
	for (int i = 0; i < simheight; i++) {
    	for (int j = 0; j < simwidth; j++) {
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
