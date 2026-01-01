#include <mpi.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank = -1;
	int size = -1;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	char hostname[256] = {0};
	gethostname(hostname, sizeof(hostname));

	std::cout << "Hello from rank " << rank
		<< " of " << size
		<< " on host " << hostname
		<< std::endl;

	MPI_Finalize();

	return 0;
}
