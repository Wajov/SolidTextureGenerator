#include "SurfaceMeshHelper.h"

void readMesh(SurfaceMesh &mesh, const std::string &fileName) {
    char c;
    int u, v, w;
    double x, y, z, sx, sy, sz;
    std::ifstream fin(fileName);
    std::vector<VertexIndex> vertices;

    sx = sy = sz = 0;
    while (fin >> c)
        if (c == 'v') {
            fin >> x >> y >> z;
            vertices.push_back(mesh.add_vertex(Point(x, y, z)));
            sx += x;
            sy += y;
            sz += z;
        } else if (c == 'f') {
            fin >> u >> v >> w;
            mesh.add_face(vertices[u - 1], vertices[v - 1], vertices[w - 1]);
        }

    Vector avg = Vector(sx, sy, sz) / mesh.number_of_vertices();
    for (VertexIndex vertex : mesh.vertices())
        mesh.point(vertex) -= avg;
    fin.close();
}

void writeMesh(SurfaceMesh &mesh, const std::string &fileName) {
    int num = 0;
    std::map<VertexIndex, int> vertices;
    std::ofstream fout(fileName);

    for (VertexIndex vertex : mesh.vertices()) {
        vertices[vertex] = ++num;
        fout << "v " << mesh.point(vertex) << std::endl;
    }
    for (FaceIndex face : mesh.faces()) {
        fout << 'f';
        HalfedgeIndex halfedge, end;
        halfedge = end = mesh.halfedge(face);

        do {
            fout << ' ' << vertices[mesh.source(halfedge)];
            halfedge = mesh.next(halfedge);
        } while (halfedge != end);

        fout << std::endl;
    }
    fout.close();
}

void coordinateRange(SurfaceMesh &mesh, Range &xRange, Range &yRange, Range &zRange) {
    xRange.first = yRange.first = zRange.first = 1e100;
    xRange.second = yRange.second = zRange.second = -1e100;
    for (VertexIndex vertex : mesh.vertices()) {
        Point point = mesh.point(vertex);
        xRange.first = std::min(xRange.first, point.x());
        xRange.second = std::max(xRange.second, point.x());
        yRange.first = std::min(yRange.first, point.y());
        yRange.second = std::max(yRange.second, point.y());
        zRange.first = std::min(zRange.first, point.z());
        zRange.second = std::max(zRange.second, point.z());
    }
}

void scaleMesh(SurfaceMesh &mesh, double factor) {
    for (VertexIndex vertex : mesh.vertices()) {
        Point point = mesh.point(vertex);
        mesh.point(vertex) = Point(point.x() * factor, point.y() * factor, point.z() * factor);
    }
}

void rotateMesh(SurfaceMesh &mesh, Point &posture) {
    Eigen::AngleAxisd xVector(posture.x(), Eigen::Vector3d(1, 0, 0));
    Eigen::AngleAxisd yVector(posture.y(), Eigen::Vector3d(0, 1, 0));
    Eigen::AngleAxisd zVector(posture.z(), Eigen::Vector3d(0, 0, 1));
    Eigen::Matrix3d matrix = zVector.matrix() * yVector.matrix() * xVector.matrix();

    for (VertexIndex vertex : mesh.vertices()) {
        Point point = mesh.point(vertex);
        Eigen::Vector3d vector(point.x(), point.y(), point.z());
        vector = matrix * vector;
        mesh.point(vertex) = Point(vector(0), vector(1), vector(2));
    }
}

void translateMesh(SurfaceMesh &mesh, Point &position) {
    Vector vector(position.x(), position.y(), position.z());
    for (VertexIndex vertex : mesh.vertices())
        mesh.point(vertex) += vector;
}

void transformToExact(SurfaceMesh &mesh, ExactSurfaceMesh &exactMesh) {
    std::map<VertexIndex, ExactVertexIndex> vertices;
    for (VertexIndex vertex : mesh.vertices()) {
        Point point = mesh.point(vertex);
        vertices[vertex] = exactMesh.add_vertex(ExactPoint(point.x(), point.y(), point.z()));
    }
    for (FaceIndex face : mesh.faces()) {
        VertexIndex u, v, w;
        HalfedgeIndex halfedge = mesh.halfedge(face);
        u = mesh.source(halfedge);
        halfedge = mesh.next(halfedge);
        v = mesh.source(halfedge);
        halfedge = mesh.next(halfedge);
        w = mesh.source(halfedge);
        exactMesh.add_face(vertices[u], vertices[v], vertices[w]);
    }
}