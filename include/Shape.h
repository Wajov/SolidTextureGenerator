#ifndef SHAPE_H
#define SHAPE_H

#include <bits/stdc++.h>

#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>
#include <CGAL/convex_hull_3.h>

#include "TypeHelper.h"
#include "ConstantHelper.h"
#include "ArrayHelper.h"
#include "RandomHelper.h"
#include "SurfaceMeshHelper.h"
#include "Section.h"

class Shape {
private:
    SurfaceMesh shape;
    Range objectionRange(Vector &direction);

public:
    Shape(const std::string &fileName);
    ~Shape();
    SurfaceMesh getShape();
    SurfaceMesh getSimplifiedShape();
    double averageDiameter();
    double **sectionAreas();
};

#endif