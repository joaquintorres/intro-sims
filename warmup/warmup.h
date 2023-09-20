#ifndef WARMUP__H
#define WARMUP__H
#endif

#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    double x;
    double y;
} pair_t;

vector<pair_t> vector_pairs(int N);
void vector_select_pairs(vector<pair_t> vec);
void print_pairs(int N);
