cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
project(ising)

add_executable(ising main.cpp ising.cpp io.cpp)

target_include_directories(ising PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(ising PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../lib/eigen-3.4.0)

target_compile_options(ising PRIVATE -Wall -Wpedantic -O3 -fomit-frame-pointer -ffast-math -march=native -fopenmp)

option(ENABLE_MPI "Enable MPI parallel execution" OFF)
option(ENABLE_GL "Enable OpenGL support" OFF)

if (ENABLE_MPI)
    add_definitions(-DENABLE_MPI=1)
    find_package(MPI REQUIRED)
    set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
    message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} ./build-mpi/ising ${MPIEXEC_POSTFLAGS} ARGS")
    target_link_libraries(ising PUBLIC MPI::MPI_CXX)
endif()

if (ENABLE_GL)
    add_definitions(-DENABLE_GL=1)
    find_package(OpenGL REQUIRED)
    target_link_libraries(ising PUBLIC ${OPENGL_LIBRARIES})
    target_include_directories(ising PUBLIC ${OPENGL_INCLUDE_DIRS})
    find_package(glfw3 REQUIRED)
    find_package(GLEW REQUIRED)

    target_link_libraries(ising PRIVATE glfw GLEW::GLEW)
endif()
