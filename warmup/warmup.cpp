#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "warmup.h"

#define LOWBOUND 0.0
#define UPBOUND  1.0
#define OUTNAME  "output.dat"
#define OUTNAME_BIN "output.bin"

// Generate a vector containing all pairs of randomly generated numbers
vector<pair_t> vector_pairs(int N){
    vector<pair_t> vec(N);

    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(LOWBOUND, UPBOUND);
    
    for (int i = 0; i < N; i++){\
        vec[i].x = dis(gen);
        vec[i].y = dis(gen);
    }

    return vec;
}

// Select pairs from existing vector
void vector_select_pairs(const vector<pair_t> vec){
    ofstream out_file;
    out_file.open(OUTNAME_BIN, ios::binary);
    if (!out_file.is_open())
        cerr << "Could not open " << OUTNAME << endl;

    for (pair_t pair : vec){
        if (pair.x < pair.y){
            //cout << "(" << pair.x << ", " << pair.y << ")" << endl;
            out_file.write(reinterpret_cast<char*>(&pair), sizeof(pair));
        }
    }

    out_file.close();
}

// Just print the pairs
void print_pairs(int N){
    pair_t pair;
    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(LOWBOUND, UPBOUND);
    
    ofstream out_file;
    
    out_file.open(OUTNAME_BIN, ios::binary);
    if (!out_file.is_open())
        cerr << "Could not open " << OUTNAME << endl;

    for (int i; i < N; i++) {
        pair.x = dis(gen);
        pair.y = dis(gen);
        
        if (pair.x < pair.y){
            //cout << "(" << pair.x << ", " << pair.y << ")" << endl;
            out_file.write(reinterpret_cast<char*>(&pair), sizeof(pair));
        }
    }

    out_file.close();
}
