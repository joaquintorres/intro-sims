#ifndef TYPES__H
#define TYPES__H

#include <string>

using namespace std;

typedef struct {
    int n; // (n, n) grid 
    int steps; // simulation steps
    double temperature; //system temperature in no units 
    double interaction; // J, coupling constant
    double energy; // System energy
    double magnetization; // System magnetization
    string restfile; // filename for storing and restoring the state
} ising_sys_t;

#endif
