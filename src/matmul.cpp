#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>

static long long checksum_ll(const std::vector<double>& v) {
	// sum of elements rounded to long long
	double s = 0.0;
	for (double x : v) s += x;
	return static_cast<long long>(s + 0.5);
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int rank = 0;
	int size = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	const int N = 256; // A is NxN, B is NxN, C is NxN
	
	// require N divisible by size for scatter
	if (N % size != 0) {
		if (rank == 0) {
			std::cerr << "Error: N must be divisible by number of ranks. "
				 << "N=" << N << " size=" << size << "\n";
		}
		MPI_Finalize();
		return 1;
	}

	const int rows_per_rank = N / size;
	const int local_A_elems = rows_per_rank * N;
	const int local_C_elems = rows_per_rank * N;

	std::vector<double> A, B(N * N), C;
	std::vector<double> local_A(local_A_elems);
	std::vector<double> local_C(local_C_elems, 0.0);

	if (rank == 0) {
		A.resize(N * N);
		C.resize(N * N);

		// fill A and B with simple deterministic values
		// A[i,j] = i + j, B[i,j] = i - j
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
			A[i * N + j] = static_cast<double>(i + j);
			B[i * N + j] = static_cast<double>(i - j);
			}
		}
	}


	// broadcast B to all ranks
	MPI_Bcast(B.data(), N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	// scatter rows of A: each rank gets rows_per_rank rows
	MPI_Scatter(
		rank == 0 ? A.data() : nullptr,
		local_A_elems,
		MPI_DOUBLE,
		local_A.data(),
		local_A_elems,
		MPI_DOUBLE,
		0,
		MPI_COMM_WORLD
	);

	MPI_Barrier(MPI_COMM_WORLD); // timing
	double t_start = MPI_Wtime();

	// local matmul: local_C = local_A * B
	// local_A is rows_per_rank x N, B is NxN, local_C is rows_per_rank x N
	for (int i = 0; i < rows_per_rank; i++) {
		for (int k = 0; k < N; k++) {
			double a_ik = local_A[i * N + k];
			// multiply that A element by row k of B
			for (int j = 0; j < N; j++) {
				local_C[i * N + j] += a_ik * B[k * N + j];
			}
		}
	}	

	// gather local C chunks into C on rank 0
	MPI_Gather(
		local_C.data(),
		local_C_elems,
		MPI_DOUBLE,
		rank == 0 ? C.data() : nullptr,
		local_C_elems,
		MPI_DOUBLE,
		0,
		MPI_COMM_WORLD
	);

	MPI_Barrier(MPI_COMM_WORLD); // timing
	double t_end = MPI_Wtime();

	if (rank == 0) {
		std::cout << "Matrix multiply done (N=" << N << ", ranks=" << size << ").\n";
		std::cout << "Checksum(C) = " << checksum_ll(C) << "\n";
		std::cout << "Elapsed time = " << (t_end - t_start) << " seconds\n";
	}

	MPI_Finalize();

	return 0;

}
