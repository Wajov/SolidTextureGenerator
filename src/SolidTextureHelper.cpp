#include "SolidTextureHelper.h"

void sampleSolidTexture() {
    Image image("image/image_" + std::to_string(USED_IMAGE) + ".png");
    unsigned char *baseColor = image.getBaseColor();
    int num;
    unsigned char **sectionColors = image.sectionColors(num);

    std::ifstream fin("output/volume.txt");
    int shape, group;
    Point posture, position;
    std::vector<SurfaceMesh> particles;
    std::vector<std::vector<unsigned char>> colors;

    while (fin >> shape >> group >> posture >> position) {
        SurfaceMesh particle = Shape("output/simplified_shape_" + std::to_string(USED_SHAPES[shape]) + ".obj").getShape();
        scaleMesh(particle, (double)(group + 1) / GROUP_SIZE);
        rotateMesh(particle, posture);
        translateMesh(particle, position);
        particles.push_back(particle);

        std::vector<unsigned char> color;
        int index = randomInt(0, num - 1);
        for (int i = 0; i < 3; i++)
            color.push_back(sectionColors[index][i]);
        colors.push_back(color);
    }
    fin.close();

    std::ofstream fout("output/texture.txt");
    for (int i = 0; i < RESOLUTION; i++) {
        double x = VOLUME_SIZE / RESOLUTION * i;
        for (int j = 0; j < RESOLUTION; j++) {
            double y = VOLUME_SIZE / RESOLUTION * j;
            for (int k = 0; k < RESOLUTION; k++) {
                double z = VOLUME_SIZE / RESOLUTION * k;
                Point point(x, y, z);
                bool flag = false;

                for (int h = 0; h < particles.size(); h++) {
                    SideOfTriangleMesh side(particles[h]);
                    if (side(point) == CGAL::ON_BOUNDED_SIDE) {
                        flag = true;
                        fout << (int)colors[h][0] << ' ' << (int)colors[h][1] << ' ' << (int)colors[h][2] << std::endl;
                        break;
                    }
                }
                if (!flag)
                    fout << (int)baseColor[0] << ' ' << (int)baseColor[1] << ' ' << (int)baseColor[2] << std::endl;
            }
        }
    }
    fout.close();
}