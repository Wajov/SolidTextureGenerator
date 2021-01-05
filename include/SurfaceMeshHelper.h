#ifndef SURFACE_MESH_HELPER_H
#define SURFACE_MESH_HELPER_H

#include <bits/stdc++.h>

#include <Eigen/Dense>

#include "TypeHelper.h"
#include "ConstantHelper.h"

void readMesh(SurfaceMesh &mesh, const std::string &fileName);
void writeMesh(SurfaceMesh &mesh, const std::string &fileName);
void coordinateRange(SurfaceMesh &mesh, Range &xRange, Range &yRange, Range &zRange);
void scaleMesh(SurfaceMesh &mesh, double factor);
void rotateMesh(SurfaceMesh &mesh, Point &posture);
void translateMesh(SurfaceMesh &mesh, Point &position);
void transformToExact(SurfaceMesh &mesh, ExactSurfaceMesh &exactMesh);

#endif