#include "VolumeHelper.h"

double intersectionVolume(SurfaceMesh &mesh1, SurfaceMesh &mesh2) {
    try {
        ExactSurfaceMesh exactMesh1, exactMesh2;
        transformToExact(mesh1, exactMesh1);
        transformToExact(mesh2, exactMesh2);
        CGAL::Nef_polyhedron_3<ExactKernel> nef1(exactMesh1), nef2(exactMesh2), nefIntersection;
        nefIntersection = nef1 * nef2;
        SurfaceMesh intersection;
        CGAL::convert_nef_polyhedron_to_polygon_mesh(nefIntersection, intersection);
        CGAL::Polygon_mesh_processing::triangulate_faces(intersection);
        return CGAL::Polygon_mesh_processing::volume(intersection);
    } catch (...) {
        return 1e100;
    }
}

Range translateRange(Range &range) {
    double lower = -range.first, upper = VOLUME_SIZE - range.second;
    if (-VOLUME_SIZE / 5 < upper)
        lower = std::max(lower, -VOLUME_SIZE / 5);
    else
        lower = std::max(lower, upper - VOLUME_SIZE / 5);
    if (VOLUME_SIZE / 5 > lower)
        upper = std::min(upper, VOLUME_SIZE / 5);
    else
        upper = std::min(upper, lower + VOLUME_SIZE / 5);
    return std::make_pair(lower, upper);
}

void constructVolume() {
    std::vector<int> shapes, groups;
    std::vector<Point> postures, positions;
    std::vector<SurfaceMesh> bases, particles;

    std::ifstream fin("output/density.txt");
    double *density = createArray1D<double>(GROUP_SIZE);
    for (int i = 0; i < GROUP_SIZE; i++)
        fin >> density[i];

    for (int i = 0; i < USED_SHAPES_SIZE; i++) {
        bases.push_back(Shape("output/simplified_shape_" + std::to_string(USED_SHAPES[i]) + ".obj").getShape());

        for (int j = 0; j < GROUP_SIZE; j++) {
            int num = round(density[j] * VOLUME * USED_SHAPES_PROP[i]);

            for (int k = 0; k < num; k++) {
                SurfaceMesh particle(bases[i]);
                scaleMesh(particle, (double)(j + 1) / GROUP_SIZE);

                double x, y, z;

                x = randomDouble(0, PI * 2);
                y = randomDouble(0, PI * 2);
                z = randomDouble(0, PI * 2);
                Point posture(x, y, z);
                rotateMesh(particle, posture);

                Range xRange, yRange, zRange;
                coordinateRange(particle, xRange, yRange, zRange);

                x = randomDouble(-xRange.first, VOLUME_SIZE - xRange.second);
                y = randomDouble(-yRange.first, VOLUME_SIZE - yRange.second);
                z = randomDouble(-zRange.first, VOLUME_SIZE - zRange.second);
                Point position(x, y, z);
                translateMesh(particle, position);

                shapes.push_back(i);
                groups.push_back(j);
                postures.push_back(posture);
                positions.push_back(position);
                particles.push_back(particle);
            }
        }
    }

    int iter = 0;
    double volume = 0;
    double **volumes = createArray2D<double>(particles.size(), particles.size());
    for (int i = 0; i < particles.size() - 1; i++)
        for (int j = i + 1; j < particles.size(); j++) {
            volumes[i][j] = volumes[j][i] = intersectionVolume(particles[i], particles[j]);
            volume += volumes[i][j];
        }
    std::cout << "Intersection volume before iteration: " << volume << std::endl;

    while (volume > 0) {
        std::vector<int> indices;
        for (int i = 0; i < particles.size(); i++) {
            bool flag = false;
            for (int j = 0; j < particles.size(); j++)
                if (volumes[i][j] > 0) {
                    flag = true;
                    break;
                }
            if (flag)
                indices.push_back(i);
        }
        int index = indices[randomInt(0, indices.size() - 1)];
        SurfaceMesh particleTemp(bases[shapes[index]]);
        scaleMesh(particleTemp, (double)(groups[index] + 1) / GROUP_SIZE);

        double x, y, z;

        x = randomDouble(-PI * 0.4, PI * 0.4);
        y = randomDouble(-PI * 0.4, PI * 0.4);
        z = randomDouble(-PI * 0.4, PI * 0.4);
        Point posture = postures[index] + Vector(x, y, z);
        rotateMesh(particleTemp, posture);

        Range xRange, yRange, zRange;
        coordinateRange(particleTemp, xRange, yRange, zRange);
        xRange.first += positions[index].x();
        xRange.second += positions[index].x();
        yRange.first += positions[index].y();
        yRange.second += positions[index].y();
        zRange.first += positions[index].z();
        zRange.second += positions[index].z();

        Range range;
        range = translateRange(xRange);
        x = randomDouble(range.first, range.second);
        range = translateRange(yRange);
        y = randomDouble(range.first, range.second);
        range = translateRange(zRange);
        z = randomDouble(range.first, range.second);
        Point position = positions[index] + Vector(x, y, z);
        translateMesh(particleTemp, position);

        double *volumesTemp = createArray1D<double>(particles.size());
        for (int i = 0; i < particles.size(); i++)
            if (i != index)
                volumesTemp[i] = intersectionVolume(particleTemp, particles[i]);

        double volumeTemp = 0;
        for (int i = 0; i < particles.size() - 1; i++)
            for (int j = i + 1; j < particles.size(); j++)
                if (i == index)
                    volumeTemp += volumesTemp[j];
                else if (j == index)
                    volumeTemp += volumesTemp[i];
                else
                    volumeTemp += volumes[i][j];

        if (volumeTemp < volume) {
            volume = volumeTemp;
            for (int i = 0; i < particles.size(); i++)
                volumes[i][index] = volumes[index][i] = volumesTemp[i];
            particles[index] = particleTemp;
            postures[index] = posture;
            positions[index] = position;
        }
        std::cout << "Intersection volume after iteration " << ++iter << ": " << volume << std::endl;

        deleteArray1D(volumesTemp);
    }

    deleteArray1D(density);
    deleteArray2D(volumes, particles.size());

    std::ofstream fout("output/volume.txt");
    for (int i = 0; i < particles.size(); i++)
        fout << shapes[i] << ' ' << groups[i] << ' ' << postures[i] << ' ' << positions[i] << std::endl;
    fout.close();
}

//void drawVolume() {
//    std::ifstream fin("output/volume.txt");
//
//    int shape, group;
//    Point posture, position;
//    SurfaceMesh ans;
//    std::vector<SurfaceMesh> particles;
//
//    while (fin >> shape >> group >> posture >> position) {
//        SurfaceMesh particle = Shape("shape/shape_" + std::to_string(USED_SHAPES[shape]) + ".obj").getShape();
//        scaleMesh(particle, (double)(group + 1) / GROUP_SIZE);
//        rotateMesh(particle, posture);
//        translateMesh(particle, position);
//
//        particles.push_back(particle);
//    }
//    fin.close();
//
//    for (int i = 0; i < particles.size(); i++) {
//        for (VertexIndex vertex : particles[i].vertices()) {
//            Point point = particles[i].point(vertex);
//            if (point.x() < 0 || point.x() > VOLUME_SIZE || point.y() < 0 || point.y() > VOLUME_SIZE || point.z() < 0 || point.z() > VOLUME_SIZE)
//                std::cout << point << std::endl;
//        }
//
//        for (int j = i + 1; j < particles.size(); j++)
//            if (intersectionVolume(particles[i], particles[j]) > 0)
//                std::cout << i << ' ' << j << std::endl;
//
//        CGAL::Polygon_mesh_processing::corefine_and_compute_union(ans, particles[i], ans);
//    }
//    CGAL::draw(ans);
//}
