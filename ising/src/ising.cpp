#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <random>
#include "ising.h"
#include "types.h"


// Generate initial grid with periodic boundary conditions
Eigen::MatrixXi init_grid(ising_sys_t * isys){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, 1);
    Eigen::MatrixXi grid(isys->n + 2*OFFSET_PBC, isys->n + 2*OFFSET_PBC);
    int tmp;
    
    for (int i=OFFSET_PBC; i <= isys->n; i++){
        for (int j=OFFSET_PBC; j <= isys->n; j++){
            tmp = distribution(gen);
            grid(j, i) = (tmp == 0) ? -1 : 1; // https://eigen.tuxfamily.org/dox-devel/group__TopicStorageOrders.html 
        }
    }
    periodic_boundary_conditions(grid);
    return grid;
}

// Copy to second to last row/column from first row/column, do the same with
// the second column and the last.
void periodic_boundary_conditions(Eigen::MatrixXi & grid){
    grid.row(0) = grid.row(grid.rows() - OFFSET_PBC - 1);
    grid.row(grid.rows() - OFFSET_PBC) = grid.row(OFFSET_PBC);
    grid.col(0) = grid.col(grid.cols() - OFFSET_PBC - 1);
    grid.col(grid.cols() - OFFSET_PBC) = grid.col(OFFSET_PBC);
}

// Explicit Hamiltonian calculated taking into account periodic 
// boundary conditions
double energy(Eigen::MatrixXi & grid, ising_sys_t * isys){
    int result = 0;

    for (int i=OFFSET_PBC; i <= isys->n; i++){
        for (int j=OFFSET_PBC; j <= isys->n; j++){
            result += grid(j,i)*grid(j+1,i);
            result += grid(j,i)*grid(j-1,i);
            result += grid(j,i)*grid(j,i+1);
            result += grid(j,i)*grid(j,i-1);
        }
    }

    return (double) result * (-isys->interaction);
}
