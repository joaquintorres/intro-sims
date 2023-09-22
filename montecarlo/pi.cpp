#include <vector>
#include "utils.h"
#include <random>

// Monte Carlo sampling of a square 1 by 1 with a radius 1 circle
double montecarlo_pi_random_sampling(int N, double x_low, double x_high, double y_low, double y_high){
    double N_circ = 0.0;
    double x, y, result;

    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> disx(x_low, x_high);
    uniform_real_distribution<> disy(y_low, y_high);
 
    if (!N)
        return -1.0;

    for (int i = 0; i < N; i++){
        x = disx(gen);
        y = disy(gen);
        if (x * x + y * y < 1.0)
            N_circ+=1.0;
    }

    result = 4.0 * N_circ/N;
    
    return result;
}
