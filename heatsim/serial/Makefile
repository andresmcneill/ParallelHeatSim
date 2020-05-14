CXX = gcc
CXXFLAGS = -Wall

all: driver.o \
	heatsim.o
	$(CXX) $(CXXFLAGS) -o heatsim \
	driver.o heatsim.o

driver.o: driver.c heatsim.c heatsim.h
	$(CXX) $(CXXFLAGS) -c driver.c

heatsim.o: heatsim.c heatsim.h
	$(CXX) $(CXXFLAGS) -c heatsim.c

test1:
	rm heatsim.pnm && ./heatsim < init1.txt
