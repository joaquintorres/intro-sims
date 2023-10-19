#ifndef ISING__H
#define ISING__H

#include <Eigen/Dense>
#include "types.h"
#define OFFSET_PBC 1 // Doesn't actually handle other offsets!
#define KBOLTZ 1.0 // Replace with actual value if necessary

Eigen::MatrixXi init_grid(ising_sys_t * isys);
void periodic_boundary_conditions(Eigen:: MatrixXi & grid);
#ifdef ENABLE_GL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
void metropolis_montecarlo(Eigen::MatrixXi & grid, ising_sys_t * isys, GLFWwindow* window);
void renderMatrix(int width, int height, Eigen::MatrixXi matrix, int matrix_size);
#else
void metropolis_montecarlo(Eigen::MatrixXi & grid, ising_sys_t * isys); 
#endif 
double energy(Eigen::MatrixXi & grid, ising_sys_t * isys);
double delta_energy(Eigen::MatrixXi & grid, ising_sys_t * isys, int x, int y);
double magnetization(Eigen::MatrixXi & grid, ising_sys_t * isys);
double delta_magnetization(Eigen::MatrixXi & grid, ising_sys_t * isys, int x, int y);
#endif
