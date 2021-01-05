#include "ShapeFeatureHelper.h"

void calculateShapeFeature() {
    for (int i = 1; i < 10; i++) {
        std::ofstream fout;
        std::string number = std::to_string(i);
        Shape shape("shape/shape_" + number + ".obj");

        std::cout << "Calculating simplified shape of shape " + number + "..." << std::endl;
        SurfaceMesh mesh(shape.getSimplifiedShape());
        writeMesh(mesh, "output/simplified_shape_" + number + ".obj");
        std::cout << "Finished." << std::endl;

        std::cout << "Calculating average diameter of shape " + number + "..." << std::endl;
        fout.open("output/diameter_" + number + ".txt");
        fout << shape.averageDiameter() << std::endl;
        std::cout << "Finished." << std::endl;
        fout.close();

        std::cout << "Calculating section areas of shape " + number + "..." << std::endl;
        fout.open("output/areas_" + number + ".txt");
        double **areas = shape.sectionAreas();
        for (int i = 0; i < GROUP_SIZE; i++) {
            for (int j = 0; j < SAMPLE_SIZE; j++)
                fout << areas[i][j] << ' ';
            fout << std::endl;
        }
        deleteArray2D(areas, GROUP_SIZE);
        std::cout << "Finished." << std::endl;
        fout.close();
    }
}