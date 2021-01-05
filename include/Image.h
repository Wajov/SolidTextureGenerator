#ifndef IMAGE_H
#define IMAGE_H

#include <bits/stdc++.h>

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <Eigen/Dense>

#include "ConstantHelper.h"
#include "ArrayHelper.h"

class Image {
private:
    int height, width;
    unsigned char ***pixels, *baseColor;
    std::vector<int> areas;
    std::vector<std::vector<unsigned char>> colors;
    Eigen::Matrix<double, 3, 1> mean;
    Eigen::Matrix<double, 3, 3> covar;
    bool range(int x, int y);

public:
    Image(const std::string &fileName);
    ~Image();
    int getHeight();
    int getWidth();
    unsigned char *getBaseColor();
    Eigen::Matrix<double, 3, 1> getMean();
    Eigen::Matrix<double, 3, 3> getCovar();
    unsigned char *data();
    int *sectionAreas(int &num);
    unsigned char **sectionColors(int &num);
};

#endif