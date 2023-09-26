#ifndef UTILS__H 
#define UTILS__H 

#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    double x;
    double y;
} pair_t;

vector<pair_t> vector_pairs(int N, double x_low, double x_high, double y_low, double y_high);
vector<pair_t> vector_pairs_importance_sampling(int N, double x_mean, double x_std, double y_low, double y_high);
#endif
