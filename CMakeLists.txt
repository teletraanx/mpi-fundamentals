cmake_minimum_required(VERSION 3.16)
project(mpi_fundamentals LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MPI REQUIRED)

add_executable(hello_mpi src/hello_mpi.cpp)
target_link_libraries(hello_mpi PRIVATE MPI::MPI_CXX)

add_executable(vector_sum src/vector_sum.cpp)
target_link_libraries(vector_sum PRIVATE MPI::MPI_CXX)

add_executable(matmul src/matmul.cpp)
target_link_libraries(matmul PRIVATE MPI::MPI_CXX)
