# Heat Simulation in MPI

This code generates an image file of a two-dimensional grid after heat is distributed across it from a heat source at the top of the image. The parallelized version splits the calculations into rows and communicates the values of the edge of the rows between processes.
