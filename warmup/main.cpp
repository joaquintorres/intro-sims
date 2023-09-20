#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "warmup.h"

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


int main(int argc, char * argv[]){
    bool st;
    char * filename;
    ifstream input_file; 
    int npairs;
    vector<pair_t> outvec;

    st = validate_arguments(argc, argv, &filename);

    if (!st)
        return 1;
    
    input_file.open(filename);
    input_file >> npairs;
    input_file.close();

    cout << "Number of pairs: " << npairs << endl;
    
    //print_pairs(npairs);

    outvec = vector_pairs(npairs);
    vector_select_pairs(outvec);

    return 0;
}
