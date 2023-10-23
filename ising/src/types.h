#ifndef TYPES__H
#define TYPES__H

#include <string>

using namespace std;

typedef struct {
    int n; // (n, n) grid 
    int steps; // simulation steps
    int writestep; // interval for writing output
    int mpirank;
    int mpisize;
    double temperature; //system temperature in no units 
    double interaction; // J, coupling constant
    double energy; // System energy
    double magnetization; // System magnetization
    string restfile; // filename for storing and restoring the state
    string ergfile; // filename for storing and restoring the state's energy
    string magfile; // filename for storing and restoring the state's magnetization
    string acceptedfile; // filename for storing the acceptance fraction
} ising_sys_t;

#endif
