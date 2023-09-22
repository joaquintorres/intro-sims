#include <iostream>
#include <string>
#include <vector>

#include "utils.h"
#include "pi.h"

#define N_POS 1
#define NARGS 2

#define LOWBOUND  0.0
#define HIGHBOUND 1.0

using namespace std;

bool validate_arguments(int argc, char * argv[], int * N){
    if (argc != NARGS){
        cerr << "Wrong number of arguments!" << endl;
        cerr << "Syntax: pi <number of steps>" << endl;
        return false;
    }
    else {
        *N = stoi(argv[N_POS]);
        return true;
    }
}


int main(int argc, char * argv[]){
    int npairs;
    double pi;
    bool st;
    vector<pair_t> points;

    st = validate_arguments(argc, argv, &npairs);

    if (!st)
        return 1;

    cout << "Number of pairs: " << npairs << endl;
    

    pi = montecarlo_pi_random_sampling(npairs, LOWBOUND, HIGHBOUND, LOWBOUND, HIGHBOUND);

    cout << "pi = " << pi << endl;

    return 0;
}
