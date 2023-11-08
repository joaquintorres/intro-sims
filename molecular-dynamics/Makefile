################
# Compiler settings and flags
FC=gfortran
#FC=mpifort
#OPENMP=-fopenmp
OPENMP=
#FFLAGS= -O0 -Wall -g $(OPENMP)
FFLAGS=-O3 -march=native -ffast-math -fexpensive-optimizations -ftree-vectorize -g $(OPENMP)
################

# Source files and modules
SRC=kinds.f90 physconst.f90 mdsys.f90 io.f90 utils.f90 ljmd.f90 main.f90
OBJ=$(SRC:.f90=.o)
MOD=$(SRC:.f90=.mod)

################
# Targets
ljmd.x: $(OBJ)
	$(FC) -o $@ $(FFLAGS) $(OBJ)

%.o: %.f90
	$(FC) -c $(FFLAGS) $<


clean:
	rm -f ljmd.x *.dat *.xyz *.o *.mod perf.data

bench: bench1 bench2 bench3

bench1: ljmd.x
	time ./ljmd.x < inputs/argon_108.inp

bench2:
	time ./ljmd.x < inputs/argon_2916.inp

bench3:
	time ./ljmd.x < inputs/argon_78732.inp

.PHONY: clean bench bench1 bench2 bench3
