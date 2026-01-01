MPI Fundamentals (C++)

This repository demonstrates fundamental MPI (Message Passing Interface) concepts
using C++ and OpenMPI. The project focuses on distributed-memory parallelism,
collective communication, and basic performance measurement.

=====================================================================

REQUIREMENTS

- Linux (tested on Ubuntu 22.04)
- CMake >= 3.16
- C++ compiler with MPI support
- OpenMPI

=====================================================================

INSTALLING DEPENDENCIES (UBUNTU)

sudo apt update
sudo apt install -y build-essential cmake openmpi-bin libopenmpi-dev

Verify MPI is available:

mpirun --version
mpicxx --version

=====================================================================

REPOSITORY STRUCTURE
```
mpi-fundamentals/
├── src/
│   ├── hello_mpi.cpp
│   ├── vector_sum.cpp
│   └── matmul.cpp
├── CMakeLists.txt
├── README.md
└── build/    (generated, not tracked in git)
```
=====================================================================

BUILD INSTRUCTIONS

All programs are built using CMake.

From the project root (mpi-fundamentals):

mkdir -p build
cd build
cmake ..
cmake --build .

This will produce the following executables inside build/:

- hello_mpi
- vector_sum
- matmul

=====================================================================

RUNNING THE PROGRAMS

All programs are launched using mpirun.

---------------------------------------------------------------------

1. HELLO MPI

Demonstrates MPI initialization, rank identification, and multi-process execution.

mpirun -np 4 ./hello_mpi

Each process prints:
- Its rank
- The total number of MPI ranks
- The hostname it is running on

---------------------------------------------------------------------

2. DISTRIBUTED VECTOR SUM

Each MPI rank computes a partial sum of a vector segment. Partial results are
combined using MPI_Reduce.

mpirun -np 4 ./vector_sum

Output includes:
- The global sum
- Elapsed execution time measured using MPI_Wtime()

---------------------------------------------------------------------

3. PARALLEL MATRIX MULTIPLY

Implements a distributed matrix multiplication using MPI collective operations:

- MPI_Bcast to distribute matrix B
- MPI_Scatter to distribute rows of matrix A
- MPI_Gather to collect the result matrix

mpirun -np 4 ./matmul

Output includes:
- A checksum of the result matrix for correctness
- Elapsed execution time measured using MPI_Wtime()

===================================================================
