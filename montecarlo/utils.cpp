#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "utils.h"

#define OUTNAME_BIN "output.bin"

// Generate a vector containing all pairs of randomly generated numbers
vector<pair_t> vector_pairs(int N, double x_low, double x_high, double y_low, double y_high){
    vector<pair_t> vec(N);

    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> disx(x_low, x_high);
    uniform_real_distribution<> disy(y_low, y_high);
    
    for (int i = 0; i < N; i++){\
        vec[i].x = disx(gen);
        vec[i].y = disy(gen);
    }

    return vec;
}

