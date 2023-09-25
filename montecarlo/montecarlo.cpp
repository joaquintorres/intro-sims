#include <iostream>
#include <fstream>
#include <functional>
#include <filesystem>
#include <vector>
#include "utils.h"

#define FILENAME_POS 1
#define NARGS 2

#define XLOW  0.0
#define XHIGH 2.0
#define YLOW  0.0
#define YHIGH 1.0

#define OUTNAME_BIN "accepted.bin"

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

double montecarlo_integration(vector<pair_t> vec, function<double(double)> f){
    ofstream out_file;
    out_file.open(OUTNAME_BIN, ios::binary);
    if (!out_file.is_open())
        cerr << "Could not open " << OUTNAME_BIN << endl;

    for (pair_t pair : vec){
        if (pair.y < f(pair.x)){
            out_file.write(reinterpret_cast<char*>(&pair.x), sizeof(double));
        }
    }

    out_file.close();

    return 0.0;
}

int main(int argc, char * argv[]){
    bool st;
    char * filename;
    ifstream input_file; 
    int npairs;
    vector<pair_t> invec;

    st = validate_arguments(argc, argv, &filename);

    if (!st)
        return 1;
    
    input_file.open(filename);
    input_file >> npairs;
    input_file.close();

    cout << "Number of pairs: " << npairs << endl;
    
    invec = vector_pairs(npairs, XLOW, XHIGH, YLOW, YHIGH);

    auto f = [](double x) -> double {
        return (x - 1) * (x - 1);
    };

    double result = montecarlo_integration(invec, f);

    cout << "Integration result = " << result << endl;
    
    return 0;
}
