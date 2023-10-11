#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <Eigen/Dense>

#include "ising.h"
#include "io.h"
#include "types.h"

#define FILENAME_POS 1
#define NARGS 2

using namespace std;

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

// Parses input and initializes variables
ising_sys_t parse_input(char * filename){
    ising_sys_t isys;
    ifstream input_file;

    input_file.open(filename);
    input_file >> isys.n;
    input_file >> isys.steps;
    input_file >> isys.writestep;
    input_file >> isys.temperature;
    input_file >> isys.interaction;
    input_file >> isys.restfile;
    input_file >> isys.ergfile;
    input_file >> isys.magfile;

    input_file.close();
    
    isys.energy = 0.0;
    isys.magnetization = 0.0;
    return isys;
}

// Restores state from files
bool restore_state(Eigen:: MatrixXi & grid, ising_sys_t * isys){
    if (filesystem::exists(isys->restfile) & filesystem::exists(isys->ergfile) & filesystem::exists(isys->magfile)){
        cout << "Restoring previous state from " << isys->restfile << endl;
        read_grid(isys->restfile, grid);

        cout << "Restoring previous energy from " << isys->ergfile << endl;
        ifstream in_ergfile(isys->ergfile, ios::binary);
        if (!in_ergfile.is_open()) {
            cerr << "Error: Unable to open the binary file." << endl;
            return 1;
        }
        
        in_ergfile.seekg(0, ios::end); // Move the file pointer to the end
        streampos fsize = in_ergfile.tellg(); // Get the file size
        in_ergfile.seekg(fsize - (streampos) sizeof(double)); // Move the file pointer to the position of the last double
        in_ergfile.read(reinterpret_cast<char*>(&isys->energy), sizeof(double));

        cout << "Restoring previous magnetization from " << isys->magfile << endl;
        ifstream in_magfile(isys->magfile, ios::binary);
        if (!in_magfile.is_open()) {
            cerr << "Error: Unable to open the binary file." << endl;
            return 1;
        }
        cout << isys->energy << endl;

        in_magfile.seekg(0, ios::end); // Move the file pointer to the end
        streampos msize = in_magfile.tellg(); // Get the file size
        in_magfile.seekg(msize - (streampos) sizeof(double)); // Move the file pointer to the position of the last double
        in_magfile.read(reinterpret_cast<char*>(&isys->magnetization), sizeof(double));
        cout << isys->magnetization << endl;
        
        
        return true;

    }
    else {
        return false;
    }
}

int main(int argc, char * argv[]){
    bool st;
    char * filename;
    ifstream input_file; 
    ising_sys_t isys;
    Eigen::MatrixXi grid;
    
    st = validate_arguments(argc, argv, &filename);

    if (!st)
        return 1;
    
    isys = parse_input(filename); 

    cout << "N = " << isys.n << endl;
    cout << "Number of steps " << isys.steps << endl;
    cout << "Writesteps: " << isys.writestep << endl;
    cout << "Temperature = " << isys.temperature << endl;
    cout << "J = " << isys.interaction << endl;
    cout << "Restfile: " << isys.restfile << endl;
    cout << "Energy file: " << isys.ergfile << endl;
    cout << "Magnetization file: " << isys.magfile << endl;

    st = restore_state(grid, &isys);
    
    // Don't initialize stuff if it's restored 
    if (!st){
      grid = init_grid(&isys);
      cout << "Initial grid = " << endl;
      // cout << grid << endl;
      write_grid(isys.restfile, grid);
      isys.energy = energy(grid, &isys);
      isys.magnetization = magnetization(grid, &isys);
    }

    cout << "Initial Energy = " << isys.energy << endl;
    metropolis_montecarlo(grid, &isys);

    return 0;
}
