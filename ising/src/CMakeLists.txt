cmake_minimum_required(VERSION 3.10)
project(ising)

add_executable(ising main.cpp ising.cpp io.cpp)

target_include_directories(ising PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(ising PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/eigen-3.4.0)

target_compile_options(ising PRIVATE -Wall -Wpedantic -O3 -fomit-frame-pointer -ffast-math -march=native -fopenmp)

option(ENABLE_MPI "Enable MPI parallel execution" OFF)

if (ENABLE_MPI)
    find_package(MPI REQUIRED)
    set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
    message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} ./build-mpi/ising ${MPIEXEC_POSTFLAGS} ARGS")
    target_link_libraries(ising PUBLIC MPI::MPI_CXX)
endif()
