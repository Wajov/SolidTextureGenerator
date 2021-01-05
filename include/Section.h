#ifndef SECTION_H
#define SECTION_H

#include <bits/stdc++.h>

#include "TypeHelper.h"
#include "ConstantHelper.h"
#include "ArrayHelper.h"
#include "Polygon.h"

class Section {
private:
    bool *flag;
    std::vector<Point> vertices, points;
    std::vector<std::vector<int>> graph;
    std::vector<Polygon> polygons;
    int vertexIndex(Point &vertex);
    void search(int x);

public:
    Section(std::vector<Segment> &segments);
    ~Section();
    double *areas(int &num);
};

#endif