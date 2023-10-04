#include <iostream>
#include <fstream>
#include <filesystem>
#include <Eigen/Dense>

#include "ising.h"
#include "types.h"

#define FILENAME_POS 1
#define NARGS 2

using namespace std;
using Eigen::MatrixXi;

bool validate_arguments(int argc, char * argv[], char * filename[]){
    if (argc != NARGS){
        cerr << "Wrong number of arguments!" << endl;
        cerr << "Syntax: warmup <input_dir>" << endl;
        return false;
    }
    else if (!filesystem::exists(argv[FILENAME_POS])){
        cerr << "File " << argv[FILENAME_POS] << " not found" << endl;
        return false;
    }
    else {
        *filename = argv[FILENAME_POS];
        return true;
    }
}

ising_sys_t parse_input(char * filename){
    ising_sys_t isys;
    ifstream input_file;

    input_file.open(filename);
    input_file >> isys.n;
    input_file >> isys.steps;
    input_file >> isys.temperature;
    input_file >> isys.restfile;

    input_file.close();
    
    return isys;
}

int main(int argc, char * argv[]){
    bool st;
    char * filename;
    ifstream input_file; 
    ising_sys_t isys;
    MatrixXi grid;
    
    st = validate_arguments(argc, argv, &filename);

    if (!st)
        return 1;
    
    isys = parse_input(filename); 

    cout << "N = " << isys.n << endl;
    cout << "Number of steps " << isys.steps << endl;
    cout << "Temperature = " << isys.temperature << endl;
    cout << "Restfile: " << isys.restfile << endl;

    grid = init_grid(&isys);

    cout << "Initial grid = " << endl;
    cout << grid << endl;

    return 0;
}
