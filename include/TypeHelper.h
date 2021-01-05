#ifndef TYPE_HELPER_H
#define TYPE_HELPER_H

#include <bits/stdc++.h>

#include <CGAL/Exact_rational.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/Side_of_triangle_mesh.h>

typedef CGAL::Cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;
typedef Kernel::Segment_3 Segment;
typedef Kernel::Plane_3 Plane;
typedef CGAL::Surface_mesh<Point> SurfaceMesh;
typedef SurfaceMesh::Vertex_index VertexIndex;
typedef SurfaceMesh::Edge_index EdgeIndex;
typedef SurfaceMesh::Face_index FaceIndex;
typedef SurfaceMesh::Halfedge_index HalfedgeIndex;
typedef CGAL::Cartesian<CGAL::Exact_rational> ExactKernel;
typedef ExactKernel::Point_3 ExactPoint;
typedef CGAL::Surface_mesh<ExactPoint> ExactSurfaceMesh;
typedef ExactSurfaceMesh::Vertex_index ExactVertexIndex;
typedef CGAL::AABB_face_graph_triangle_primitive<SurfaceMesh> Primitive;
typedef CGAL::AABB_traits<Kernel, Primitive> Traits;
typedef CGAL::AABB_tree<Traits> Tree;
typedef boost::optional<Tree::Intersection_and_primitive_id<Plane>::Type> PlaneIntersection;
typedef CGAL::Side_of_triangle_mesh<SurfaceMesh, Kernel> SideOfTriangleMesh;
typedef std::pair<double, double> Range;

#endif