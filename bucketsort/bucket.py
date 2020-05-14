from mpi4py import MPI
import numpy as np


comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

N = 64

bucketSize = int(N/size)

rawNum = np.zeros(N, dtype=int)
sortNum = None
localBucket = []

if rank == 0:
    rawNum = np.random.randint(N, size = N)

comm.Bcast(rawNum, root = 0)

localMin = rank * bucketSize
localMax = (rank + 1) * bucketSize
counter = int(0)

for i in range(N):
    if (rawNum[i] >= localMin) and (rawNum[i] < localMax):
        localBucket.append(rawNum[i])
        counter += 1

localBucket = np.array(localBucket)

localBucket = np.sort(localBucket)

procCount = np.zeros(size, dtype=int)
disp = np.zeros(size, dtype=int)

counter = np.array(counter)

comm.Gather(counter, procCount, root = 0)

if rank == 0:
    disp[0] = 0
    sortNum = np.zeros(N, dtype=int)
    for i in range(size-1):
        disp[i+1] = disp[i] + procCount[i]
    
comm.Gatherv(localBucket, [sortNum,tuple(procCount),tuple(disp),MPI.LONG], root = 0)

if rank == 0:
    print('Before sort:')
    for x in rawNum:
        print(x, end = ' ')
    print('')
    print('After sort:')
    for x in sortNum:
        print(x, end = ' ')
    print('')