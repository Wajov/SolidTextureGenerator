#include "Polygon.h"

Polygon::Polygon(std::vector<Point> &points) {
    this->points = points;
}

Polygon::~Polygon() {}

double Polygon::area() {
    double ans = 0;
    Vector standard = CGAL::cross_product(points[1] - points[0], points[2] - points[0]);

    for (int j = 2; j < points.size(); j++) {
        Vector cross = CGAL::cross_product(points[j - 1] - points[0], points[j] - points[0]);
        if (standard * cross > 0)
            ans += sqrt(cross.squared_length());
        else
            ans -= sqrt(cross.squared_length());
    }

    return fabs(ans);
}