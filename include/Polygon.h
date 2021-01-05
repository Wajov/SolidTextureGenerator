#ifndef POLYGON_H
#define POLYGON_H

#include <bits/stdc++.h>

#include "TypeHelper.h"

class Polygon {
private:
    std::vector<Point> points;

public:
    Polygon(std::vector<Point> &points);
    ~Polygon();
    double area();
};

#endif