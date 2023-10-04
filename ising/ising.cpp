#include <Eigen/Dense>
#include <random>
#include "types.h"

using namespace std;
using Eigen::MatrixXi;


MatrixXi init_grid(ising_sys_t * isys){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, 1);
    MatrixXi grid(isys->n, isys->n);
    int tmp;
    
    for (int i=0; i < isys-> n; i++){
        for (int j=0; j < isys-> n; j++){
            tmp = distribution(gen);
            grid(j, i) = (tmp == 0) ? -1 : 1; // https://eigen.tuxfamily.org/dox-devel/group__TopicStorageOrders.html 
        }
    }
    return grid;
}
