#ifndef RANDOM_HELPER_H
#define RANDOM_HELPER_H

#include <bits/stdc++.h>

#include <Eigen/Dense>

#include "TypeHelper.h"

static std::default_random_engine engine(time(nullptr));

int randomInt(int lower, int upper);
double randomDouble(double lower, double upper);
Vector randomDirection();
double randomNormal1D(double mean, double covar);
Eigen::Matrix<double, 3, 1> randomNormal3D(Eigen::Matrix<double, 3, 1> &mean, Eigen::Matrix<double, 3, 3> &covar);

#endif