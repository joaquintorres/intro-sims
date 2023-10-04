#ifndef ISING__H
#define ISING__H

#include <Eigen/Dense>
#include "types.h"

using Eigen::MatrixXi;

MatrixXi init_grid(ising_sys_t * isys);

#endif
