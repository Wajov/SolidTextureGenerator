#ifndef VOLUME_HELPER_H
#define VOLUME_HELPER_H

#include <bits/stdc++.h>

#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/boost/graph/convert_nef_polyhedron_to_polygon_mesh.h>
//#include <CGAL/draw_surface_mesh.h>

#include "TypeHelper.h"
#include "ConstantHelper.h"
#include "ArrayHelper.h"
#include "RandomHelper.h"
#include "SurfaceMeshHelper.h"
#include "Shape.h"

double intersectionVolume(SurfaceMesh &mesh1, SurfaceMesh &mesh2);
Range translateRange(Range &range);
void constructVolume();
//void drawVolume();

#endif