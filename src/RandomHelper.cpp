#include "RandomHelper.h"

int randomInt(int lower, int upper) {
    std::uniform_int_distribution<int> distribution(lower, upper);
    return distribution(engine);
}

double randomDouble(double lower, double upper) {
    std::uniform_real_distribution<double> distribution(lower, upper);
    return distribution(engine);
}

Vector randomDirection() {
    double x = randomNormal1D(0, 1), y = randomNormal1D(0, 1), z = randomNormal1D(0, 1);
    Vector direction(x, y, z);

    return direction / sqrt((double)direction.squared_length());
}

double randomNormal1D(double mean, double covar) {
    std::normal_distribution<double> distribution(mean, covar);
    return distribution(engine);
}

Eigen::Matrix<double, 3, 1> randomNormal3D(Eigen::Matrix<double, 3, 1> &mean, Eigen::Matrix<double, 3, 3> &covar) {
    Eigen::LLT<Eigen::Matrix<double, 3, 3>> llt(covar);
    Eigen::Matrix<double, 3, 3> A = llt.matrixL();
    Eigen::Matrix<double, 3, 1> z;
    for (int i = 0; i < 3; i++)
        z(i, 0) = randomNormal1D(0, 1);

    return A * z + mean;
}