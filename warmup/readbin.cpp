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
    pair_t pair;

    st = validate_arguments(argc, argv, &filename);
    if (!st)
        return 1;
    
    ifstream in_file;
 
    in_file.open(filename, ios::binary);
    if (!in_file.is_open())
        cerr << "Could not open " << filename << endl;

    while (in_file.read(reinterpret_cast<char*>(&pair), sizeof(pair)))
        cout << "(" << pair.x << "," << pair.y << ")" << endl;

    in_file.close();

    return 0;
}
