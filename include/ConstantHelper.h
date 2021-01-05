#ifndef CONSTANT_HELPER_H
#define CONSTANT_HELPER_H

#include <bits/stdc++.h>

const int DIFF_THRESHOLD = 60;
const int AREA_THRESHOLD = 10;
const int D2X[4] = {-1, 1, 0, 0};
const int D2Y[4] = {0, 0, -1, 1};
const double PI = std::acos(-1);
const int GROUP_SIZE = 10;
const int SAMPLE_SIZE = 10000;
const double EPSILON = 1e-20;
const int USED_IMAGE = 6;
const int USED_SHAPES[] = {1};
const double USED_SHAPES_PROP[] = {1};
const int USED_SHAPES_SIZE = sizeof(USED_SHAPES) / sizeof(int);
const double VOLUME_SIZE = 50;
const double VOLUME = VOLUME_SIZE * VOLUME_SIZE * VOLUME_SIZE;
const double SIMP_RATIO = 0.01;
const int RESOLUTION = 64;
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

#endif