#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank = 0;
	int size = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	const int N = 1000000; // total vector size
	int chunk_size = N / size;

	std::vector<int> local_data(chunk_size, 1);

	MPI_Barrier(MPI_COMM_WORLD); // for timing
	double t_start = MPI_Wtime(); // for timing

	// each rank computes its local sum
	int local_sum = std::accumulate(
		local_data.begin(),
		local_data.end(),
		0
	);

	int global_sum = 0;

	// reduce all local sums to rank 0
	MPI_Reduce(
		&local_sum,
		&global_sum,
		1,
		MPI_INT,
		MPI_SUM,
		0,
		MPI_COMM_WORLD
	);

	MPI_Barrier(MPI_COMM_WORLD); //for timing
	double t_end = MPI_Wtime(); 

	if (rank == 0) {
		std::cout << "Global sum = " << global_sum << std::endl;
		std::cout << "Elapsed time = " << (t_end - t_start) << " seconds\n";
	}

	MPI_Finalize();

	return 0;
}
