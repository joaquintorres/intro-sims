#ifndef IO__H
#define IO__H
#include <Eigen/Dense>
#include <fstream>
#include <string>
#include "types.h"

bool write_grid(const std::string& filename, Eigen::MatrixXi& matrix);
bool read_grid(const std::string& filename, Eigen::MatrixXi& matrix);

#endif
