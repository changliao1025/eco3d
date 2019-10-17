#pragma once
// the global variables for all other modules
// 50==================================================
// c library
#include <cmath>
// 50==================================================
// c++ library
#include <vector>
// 50==================================================
#include "global.h"
// 50==================================================
using namespace std;
// 50==================================================

// 50==================================================
// simple math
double calculate_mean(std::vector<double> vData_in);
double calculate_mean(std::vector<double> vData_in, double missing_value);
double calculate_mean(std::vector<double> vData_in, double max_value,
                      double min_value);
// matrix solution
void tridia(int n, vector<double> &a, vector<double> &b, vector<double> &c,
            vector<double> &r, vector<double> &u);
