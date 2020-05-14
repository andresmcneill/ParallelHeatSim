#include "heatsim.h"


int main(void) {

	double starttemp = 20.0;
	double sourcetemp = 300.0;
	double sourcewidth = .4;

	scanf("%d %d %d", &simwidth, &simheight, &iterations);

	/* Colors are list in order of intensity */
	char * colors[10] = {RED, ORANGE, YELLOW, LTGREEN, GREEN,
		LTBLUE, BLUE, DARKTEAL, BROWN, BLACK };

	FILE * fp;

	fp = fopen("heatsim.pnm", "w");

    /* Print the P3 format header */
    fprintf(fp, "P3\n%d %d\n15\n", simwidth, simheight);

	double **simarray = (double **)malloc(simheight * sizeof(double *));
	for (int i = 0; i < simheight; i++) {
		simarray[i] = (double *)malloc(simwidth * sizeof(double));
	}

	double **newarray = (double **)malloc(simheight * sizeof(double *));
	for (int i = 0; i < simheight; i++) {
		newarray[i] = (double *)malloc(simwidth * sizeof(double));
	}

	init_sim(newarray, starttemp, sourcetemp, sourcewidth);
	copy_new_to_old(newarray, simarray);


	//	Main loop
	for (int k = 0; k < iterations; k++) {
		copy_new_to_old(newarray, simarray);
		calculate_new(newarray, simarray);
	}

	printsim(fp, simarray, colors);

	for (int i = 0; i < simheight; i++) {
		free(simarray[i]);
	}
	free(simarray);

	for (int i = 0; i < simheight; i++) {
		free(newarray[i]);
	}
	free(newarray);

	fclose(fp);
	/*
	for each iteration {
		CopyNewToOld(new, old);
		CalculateNew(new, old, xsource, ysource);
		Optionally, PrintGridtoFile(new, xsource, ysource);
	}
	*/
	return 0;
}
