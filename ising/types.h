#ifndef TYPES__H
#define TYPES__H

#include <string>

using namespace std;

typedef struct {
    int n; // (n, n) grid 
    int steps; // simulation steps
    string restfile; // filename for storing and restoring the state
} ising_sys_t;

#endif
