CXX = mpicc
CXXFLAGS = -Wall

all: pdriver.o \
	pheatsim.o
	$(CXX) $(CXXFLAGS) -o pheatsim \
	pdriver.o pheatsim.o

pdriver.o: pdriver.c pheatsim.c pheatsim.h
	$(CXX) $(CXXFLAGS) -c pdriver.c

pheatsim.o: pheatsim.c pheatsim.h
	$(CXX) $(CXXFLAGS) -c pheatsim.c

test1:
	mpirun -np 1 --mca mpi_cuda_support 0 pheatsim

test2:
	mpirun -np 2 --mca mpi_cuda_support 0 pheatsim
    
test3:
	mpirun -np 3 --mca mpi_cuda_support 0 pheatsim
    
test4:
	mpirun -np 8 --mca mpi_cuda_support 0 pheatsim