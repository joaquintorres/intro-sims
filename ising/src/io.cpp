#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include <string>
#include "io.h"
#include "types.h"

bool write_grid(const std::string& filename, Eigen::MatrixXi& matrix) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false;
    }

    int rows = matrix.rows();
    int cols = matrix.cols();

    // Write the dimensions of the matrix
    file.write(reinterpret_cast<char*>(&rows), sizeof(int));
    file.write(reinterpret_cast<char*>(&cols), sizeof(int));

    // Write the matrix data
    file.write(reinterpret_cast<const char*>(matrix.data()), sizeof(int) * rows * cols);

    file.close();
    return true;
}

bool read_grid(const std::string& filename, Eigen::MatrixXi& matrix) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return false;
    }

    int rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(int));
    file.read(reinterpret_cast<char*>(&cols), sizeof(int));

    matrix.resize(rows, cols);

    file.read(reinterpret_cast<char*>(matrix.data()), sizeof(int) * rows * cols);

    file.close();
    return true;
}

