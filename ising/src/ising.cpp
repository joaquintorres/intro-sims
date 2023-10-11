#include <cmath>
#include <iostream>
#include <fstream>
#include <random>
#include <Eigen/Dense>
#include "io.h"
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


// Iterates Metropolis Monte Carlo
void metropolis_montecarlo(Eigen::MatrixXi & grid, ising_sys_t * isys){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> spinflip_dis(OFFSET_PBC, isys->n);
    uniform_real_distribution<double> uni(0.0, 1.0);
    
    int spinflip_x, spinflip_y;
    double delta_erg, delta_mag, mc_sample;
    double beta = 1.0 / (KBOLTZ * isys->temperature);

    std::ofstream ergfile;
    std::ofstream magfile;
    ergfile.open(isys->ergfile, std::ios_base::app | std::ios::binary); // append instead of overwrite
    magfile.open(isys->magfile, std::ios_base::app | std::ios::binary);

    for (int i=0; i < isys->steps; i++){
        spinflip_x = spinflip_dis(gen);
        spinflip_y = spinflip_dis(gen);

        delta_erg = delta_energy(grid, isys, spinflip_x, spinflip_y);
        delta_mag = delta_magnetization(grid, isys, spinflip_x, spinflip_y);

        if (delta_erg < 0){
            grid(spinflip_x,spinflip_y) = -grid(spinflip_x,spinflip_y); // Flips spin if energy is lower
            // Update mean values
            isys->energy += delta_erg;
            isys->magnetization += delta_mag;
        }
        else{
            mc_sample = uni(gen);
            // Acceptance probabilty is exp(-beta * delta_energy)
            if (mc_sample < std::exp(-beta * delta_erg)){
                grid(spinflip_x,spinflip_y) = -grid(spinflip_x,spinflip_y);
                // Update mean values
                isys->energy += delta_erg;
                isys->magnetization += delta_mag;
            }
        }
        // Write mean values
        if (i % isys->writestep == 0){
            ergfile.write(reinterpret_cast<char*>(&isys->energy), sizeof(double));
            magfile.write(reinterpret_cast<char*>(&isys->magnetization), sizeof(double));
        }
    }
    write_grid(isys->restfile, grid);
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

// Returns energy difference when a spin flip is performed
double delta_energy(Eigen::MatrixXi & grid, ising_sys_t * isys, int x, int y){
    int result = 0;
    // first neighbors
    result += grid(x+1,y);
    result += grid(x-1,y);
    result += grid(x,y+1);
    result += grid(x,y-1);

    result *= grid(x,y);

    return (double) result * 2.0 * isys->interaction;
}

// Explicit magnetization calculated taking into account periodic 
// boundary conditions
double magnetization(Eigen::MatrixXi & grid, ising_sys_t * isys){
    int result = 0;

    for (int i=OFFSET_PBC; i <= isys->n; i++){
        for (int j=OFFSET_PBC; j <= isys->n; j++){
            result += grid(j,i);
        }
    }

    return (double) result;
}


// Returns magnetization difference when a spin flip is performed
double delta_magnetization(Eigen::MatrixXi & grid, ising_sys_t * isys, int x, int y){
    return -2.0 * (double) grid(x,y);
}
