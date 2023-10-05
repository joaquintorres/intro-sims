#ifndef ISING__H
#define ISING__H

#include <Eigen/Dense>
#include "types.h"
#define OFFSET_PBC 1 // Doesn't actually handle other offsets!

Eigen::MatrixXi init_grid(ising_sys_t * isys);
void periodic_boundary_conditions(Eigen:: MatrixXi & grid);
#endif
