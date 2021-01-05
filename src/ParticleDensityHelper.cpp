#include "ParticleDensityHelper.h"

int groupIndex(double area, double maxArea) {
    return (int)ceil(sqrt(area / maxArea) * GROUP_SIZE) - 1;
}

void calculateParticleDensity() {
    Image image("image/image_" + std::to_string(USED_IMAGE) + ".png");
    int num;
    int *imageAreas = image.sectionAreas(num);
    int imageMaxArea = *std::max_element(imageAreas, imageAreas + num);

    double *diameter = createArray1D<double>(USED_SHAPES_SIZE);
    double ***shapeAreas = createArray3D<double>(USED_SHAPES_SIZE, GROUP_SIZE, SAMPLE_SIZE);
    double shapeMaxArea = 0;
    for (int i = 0; i < USED_SHAPES_SIZE; i++) {
        std::string number = std::to_string(USED_SHAPES[i]);
        std::ifstream fin;
        
        fin.open("output/diameter_" + number + ".txt");
        fin >> diameter[i];
        fin.close();

        fin.open("output/areas_" + number + ".txt");
        for (int j = 0; j < GROUP_SIZE; j++)
            for (int k = 0; k < SAMPLE_SIZE; k++) {
                fin >> shapeAreas[i][j][k];
                shapeMaxArea = std::max(shapeMaxArea, shapeAreas[i][j][k]);
            }
        fin.close();
    }

    int *cnt;

    Eigen::Matrix<double, GROUP_SIZE, 1> N_A;
    cnt = createArray1D<int>(GROUP_SIZE);
    for (int i = 0; i < num; i++)
        cnt[groupIndex(imageAreas[i], imageMaxArea)]++;
    for (int i = 0; i < GROUP_SIZE; i++)
        N_A(i, 0) = cnt[i] / (image.getHeight() * image.getWidth() * shapeMaxArea / imageMaxArea);
    deleteArray1D(cnt);
    
    Eigen::Matrix<double, GROUP_SIZE, GROUP_SIZE> K, sum;
    sum.setZero();
    for (int i = 0; i < USED_SHAPES_SIZE; i++) {
        for (int j = 0; j < GROUP_SIZE; j++) {
            cnt = createArray1D<int>(GROUP_SIZE);
            for (int k = 0; k < SAMPLE_SIZE; k++)
                cnt[groupIndex(shapeAreas[i][j][k], shapeMaxArea)]++;
            for (int k = 0; k < GROUP_SIZE; k++)
                K(k, j) = (double)cnt[k] / SAMPLE_SIZE * (j + 1) / GROUP_SIZE;
            deleteArray1D(cnt);
        }
        sum += K * diameter[i] * USED_SHAPES_PROP[i];
    }

    deleteArray1D(imageAreas);
    deleteArray1D(diameter);
    deleteArray3D(shapeAreas, USED_SHAPES_SIZE, GROUP_SIZE);

    Eigen::Matrix<double, GROUP_SIZE, 1> N_V = sum.inverse() * N_A;
    for (int i = 0; i < GROUP_SIZE; i++)
        N_V(i, 0) = std::max(N_V(i, 0), 0.0);

    std::ofstream fout("output/density.txt");
    for (int i = 0; i < GROUP_SIZE - 1; i++)
        fout << N_V(i, 0) << ' ';
    fout << N_V(GROUP_SIZE - 1, 0) << std::endl;
    fout.close();
}
