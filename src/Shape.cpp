#include "Shape.h"

Shape::Shape(const std::string &fileName) {
    readMesh(shape, fileName);
}

Shape::~Shape() {}

std::pair<double, double> Shape::objectionRange(Vector &direction) {
    std::vector<double> objections;

    for (VertexIndex vertex : shape.vertices()) {
        Point point = shape.point(vertex);
        objections.push_back(direction * Vector(point.x(), point.y(), point.z()));
    }
    sort(objections.begin(), objections.end());
    
    return std::make_pair(*objections.begin(), *objections.rbegin());
}

SurfaceMesh Shape::getShape() {
    return shape;
}

SurfaceMesh Shape::getSimplifiedShape() {
    SurfaceMesh ans(shape);
    CGAL::Surface_mesh_simplification::Count_ratio_stop_predicate<SurfaceMesh> stop(SIMP_RATIO);
    CGAL::Surface_mesh_simplification::edge_collapse(ans, stop);

    return ans;
}

double Shape::averageDiameter() {
    SurfaceMesh hull;
    std::vector<Point> points;

    for (VertexIndex vertex : shape.vertices())
        points.push_back(shape.point(vertex));
    CGAL::convex_hull_3(points.begin(), points.end(), hull);

    double sum = 0;
    std::map<EdgeIndex, VertexIndex> temp;

    for (FaceIndex face : hull.faces()) {
        HalfedgeIndex halfedge, end;
        halfedge = end = hull.halfedge(face);
        do {
            VertexIndex opposite = hull.target(hull.next(halfedge));
            EdgeIndex edge = hull.edge(halfedge);
            if (temp[edge] == SurfaceMesh::null_vertex())
                temp[edge] = opposite;
            else {
                Point source = hull.point(hull.source(halfedge));
                Point target = hull.point(hull.target(halfedge));
                double radias = fabs(CGAL::approximate_dihedral_angle(source, target, hull.point(opposite), hull.point(temp[edge]))) / 180 * PI;
                sum += sqrt((source - target).squared_length()) * (PI - radias);
            }

            halfedge = hull.next(halfedge);
        } while (halfedge != end);
    }

    return sum / (4 * PI);
}

double **Shape::sectionAreas() {
    Range range = std::make_pair(1e100, -1e100);
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        Vector direction = randomDirection();
        Range temp = objectionRange(direction);
        range.first = std::min(range.first, temp.first);
        range.second = std::max(range.second, temp.second);
    }

    double **areas = createArray2D<double>(GROUP_SIZE, SAMPLE_SIZE);

    scaleMesh(shape, 1.0 / GROUP_SIZE);
    for (int group = 0; group < GROUP_SIZE; group++) {
        if (group > 0)
            scaleMesh(shape, (double)(group + 1) / group);
        Tree tree(faces(shape).first, faces(shape).second, shape);

        for (int sample = 0; sample < SAMPLE_SIZE; ) {
            Vector direction = randomDirection();
            direction *= randomDouble(range.first, range.second);
            Point point(direction.x(), direction.y(), direction.z());
            Plane plane(point, direction);
            std::vector<PlaneIntersection> intersections;
            std::vector<Segment> segments;
            tree.all_intersections(plane, std::back_inserter(intersections));

            if (intersections.empty())
                continue;
            for (PlaneIntersection intersection : intersections) {
                Segment *segment = boost::get<Segment>(&(intersection->first));
                if (segment)
                    segments.push_back(*segment);
            }

            Section section(segments);
            int num;
            double *temp;
            temp = section.areas(num);
            for (int i = 0; i < num && sample < SAMPLE_SIZE; i++, sample++)
                areas[group][sample] = temp[i];
            deleteArray1D(temp);
        }
    }

    return areas;
}