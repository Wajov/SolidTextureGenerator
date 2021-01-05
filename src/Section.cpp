#include "Section.h"

Section::Section(std::vector<Segment> &segments) {
    for (int i = 0; i < segments.size(); i++) {
        Point source = segments[i].source(), target = segments[i].target();
        int u = vertexIndex(source), v = vertexIndex(target);
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    for (int i = 0; i < vertices.size(); i++)
        assert(graph[i].size() == 2);

    flag = createArray1D<bool>(vertices.size());
    for (int i = 0; i < vertices.size(); i++)
        if (!flag[i]) {
            points.clear();
            search(i);
            polygons.push_back(Polygon(points));
        }
}

Section::~Section() {
    deleteArray1D(flag);
}

int Section::vertexIndex(Point &vertex) {
    for (int i = 0; i < vertices.size(); i++)
        if ((vertices[i] - vertex).squared_length() < EPSILON)
            return i;
    vertices.push_back(vertex);
    graph.push_back(std::vector<int>());

    return vertices.size() - 1;
}

void Section::search(int x) {
    flag[x] = true;
    points.push_back(vertices[x]);
    for (int i = 0; i < graph[x].size(); i++)
        if (!flag[graph[x][i]])
            search(graph[x][i]);
}

double *Section::areas(int &num) {
    double *ans = createArray1D<double>(polygons.size());

    for (int i = 0; i < polygons.size(); i++)
        ans[i] = polygons[i].area();
    
    num = polygons.size();
    return ans;
}