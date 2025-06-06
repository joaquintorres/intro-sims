#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <Eigen/Dense>
#ifdef ENABLE_MPI
#include <mpi.h>
#endif

#ifdef ENABLE_GL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

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
    input_file >> isys.thermalsteps;
    input_file >> isys.writestep;
    input_file >> isys.temperature;
    input_file >> isys.interaction;
    input_file >> isys.restfile;
    input_file >> isys.ergfile;
    input_file >> isys.magfile;
    input_file >> isys.acceptedfile;

    input_file.close();
    
    isys.energy = 0.0;
    isys.magnetization = 0.0;
    return isys;
}

// Restores state from files
bool restore_state(Eigen:: MatrixXi & grid, ising_sys_t * isys){
    #ifdef ENABLE_MPI
    string restfile = std::to_string(isys->mpirank) + isys->restfile;
    string ergfile = std::to_string(isys->mpirank) + isys->ergfile;
    string magfile = std::to_string(isys->mpirank) + isys->magfile;
    #else
    string restfile = isys->restfile;
    string ergfile = isys->ergfile;
    string magfile = isys->magfile;
    #endif

    if (filesystem::exists(restfile) & filesystem::exists(ergfile) & filesystem::exists(magfile)){
        cout << "Restoring previous state from " << restfile << endl;
        read_grid(restfile, grid);

        cout << "Restoring previous energy from " << ergfile << endl;
        ifstream in_ergfile(ergfile, ios::binary);
        if (!in_ergfile.is_open()) {
            cerr << "Error: Unable to open the binary file." << endl;
            return 1;
        }
        
        in_ergfile.seekg(0, ios::end); // Move the file pointer to the end
        streampos fsize = in_ergfile.tellg(); // Get the file size
        in_ergfile.seekg(fsize - (streampos) sizeof(double)); // Move the file pointer to the position of the last double
        in_ergfile.read(reinterpret_cast<char*>(&isys->energy), sizeof(double));

        cout << "Restoring previous magnetization from " << magfile << endl;
        ifstream in_magfile(magfile, ios::binary);
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
    cout << "Number of steps: " << isys.steps << endl;
    cout << "Number of initial steps discarded: " << isys.thermalsteps << endl;
    cout << "Writesteps: " << isys.writestep << endl;
    cout << "Temperature = " << isys.temperature << endl;
    cout << "J = " << isys.interaction << endl;
    cout << "Restfile: " << isys.restfile << endl;
    cout << "Energy file: " << isys.ergfile << endl;
    cout << "Magnetization file: " << isys.magfile << endl;
    cout << "Accepted fraction file: " << isys.acceptedfile << endl;

#ifdef ENABLE_MPI

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &isys.mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &isys.mpirank);
    
    if (!isys.mpirank)
        cout << "size = " << isys.mpisize << endl;
    
    cout << "rank = " << isys.mpirank << endl;

#endif
    

    st = restore_state(grid, &isys);
    
    // Don't initialize stuff if it's restored 
    if (!st){
      grid = init_grid(&isys);
      #ifdef ENABLE_MPI
      if (!isys.mpirank){
        cout << "Initial grid = " << endl;
        cout << grid << endl;
      }
      #else
      cout << "Initial grid = " << endl;
      cout << grid << endl;
      #endif
      write_grid(isys.restfile, grid);
      isys.energy = energy(grid, &isys);
      isys.magnetization = magnetization(grid, &isys);
    }
#ifdef ENABLE_GL
    const int width = 800;
    const int height = 800;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Matrix Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

#endif

    cout << "Initial Energy = " << isys.energy << endl;

#ifdef ENABLE_GL
    metropolis_montecarlo(grid, &isys, window);
    glfwTerminate();
#else
    metropolis_montecarlo(grid, &isys);
#endif

 
#ifdef ENABLE_MPI
    MPI_Finalize();
#endif
    return 0;
}
