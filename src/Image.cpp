#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "Image.h"

Image::Image(const std::string &fileName) {
    int channel;
    unsigned char *data = stbi_load(fileName.data(), &width, &height, &channel, 0);

    unsigned char ***origin = createArray3D<unsigned char>(height, width, 3);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            for (int k = 0; k < 3; k++)
                origin[i][j][k] = data[channel * (width * i + j) + k];
    stbi_image_free(data);

    int max = 0, num = 0;
    int *color = createArray1D<int>(3), *sum = createArray1D<int>(3), ***cnt = createArray3D<int>(256, 256, 256);
    bool **base = createArray2D<bool>(height, width), **vis = createArray2D<bool>(height, width);
    pixels = createArray3D<unsigned char>(height, width, 3);
    std::queue<std::pair<int, int>> q;
    std::vector<std::pair<int, int>> v;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            cnt[origin[i][j][0]][origin[i][j][1]][origin[i][j][2]]++;
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 256; j++)
            for (int k = 0; k < 256; k++)
                if (cnt[i][j][k] > max) {
                    max = cnt[i][j][k];
                    color[0] = i;
                    color[1] = j;
                    color[2] = k;
                }

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            int diff = 0;
            for (int k = 0; k < 3; k++)
                diff += abs(color[k] - (int)origin[i][j][k]);
            if (diff <= DIFF_THRESHOLD) {
                base[i][j] = true;
                num++;
                for (int k = 0; k < 3; k++)
                    sum[k] += (int)origin[i][j][k];
            }
        }

    baseColor = createArray1D<unsigned char>(3);
    for (int i = 0; i < 3; i++)
        baseColor[i] = sum[i] / num;

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (!base[i][j] && !vis[i][j]) {
                num = 0;
                for (int k = 0; k < 3; k++)
                    color[k] = 0;
                vis[i][j] = true;
                q.push({i, j});
                v.clear();
                v.push_back({i, j});
                while (!q.empty()) {
                    int x = q.front().first, y = q.front().second;
                    num++;
                    for (int k = 0; k < 3; k++)
                        color[k] += (int)origin[x][y][k];
                    q.pop();
                    for (int k = 0; k < 4; k++) {
                        int xt = x + D2X[k], yt = y + D2Y[k];
                        if (range(xt, yt) && !base[xt][yt] && !vis[xt][yt]) {
                            vis[xt][yt] = true;
                            q.push({xt, yt});
                            v.push_back({xt, yt});
                        }
                    }
                }
                if (num > AREA_THRESHOLD) {
                    areas.push_back(num);
                    std::vector<unsigned char> colorTemp;
                    for (int k = 0; k < 3; k++)
                        colorTemp.push_back(color[k] /= num);
                    colors.push_back(colorTemp);
                    for (int k = 0; k < v.size(); k++)
                        for (int h = 0; h < 3; h++)
                            pixels[v[k].first][v[k].second][h] = color[h];
                } else
                    for (int k = 0; k < v.size(); k++)
                        for (int h = 0; h < 3; h++)
                            pixels[v[k].first][v[k].second][h] = baseColor[h];
            } else if (base[i][j])
                for (int k = 0; k < 3; k++)
                    pixels[i][j][k] = baseColor[k];

    stbi_write_png("output/average.png", width, height, 3, this->data(), 0);

    mean.setZero();
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            Eigen::Matrix<double, 3, 1> x(origin[i][j][0] - pixels[i][j][0], origin[i][j][2] - pixels[i][j][1], origin[i][j][2] - pixels[i][j][2]);
            mean += x;
        }
    mean /= height * width;
    covar.setZero();
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            Eigen::Matrix<double, 3, 1> x(origin[i][j][0] - pixels[i][j][0], origin[i][j][2] - pixels[i][j][1], origin[i][j][2] - pixels[i][j][2]);
            covar += (x - mean) * (x - mean).transpose();
        }
    covar /= height * width;

    deleteArray3D(origin, height, width);
    deleteArray1D(color);
    deleteArray1D(sum);
    deleteArray3D(cnt, 256, 256);
    deleteArray2D(base, height);
    deleteArray2D(vis, height);
}

Image::~Image() {
    deleteArray3D(pixels, height, width);
    deleteArray1D(baseColor);
}

bool Image::range(int x, int y) {
    return x >= 0 && x < height && y >= 0 && y < width;
}

int Image::getHeight() {
    return height;
}

int Image::getWidth() {
    return width;
}

unsigned char *Image::getBaseColor() {
    return baseColor;
}

Eigen::Matrix<double, 3, 1> Image::getMean() {
    return mean;
}

Eigen::Matrix<double, 3, 3> Image::getCovar() {
    return covar;
}

unsigned char *Image::data() {
    unsigned char *ans = createArray1D<unsigned char>(height * width * 3);
    int index = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            for (int k = 0; k < 3; k++)
                ans[index++] = pixels[i][j][k];
    
    return ans;
}

int *Image::sectionAreas(int &num) {
    num = areas.size();
    int *ans = createArray1D<int>(num);
    for (int i = 0; i < num; i++)
        ans[i] = areas[i];

    return ans;
}

unsigned char **Image::sectionColors(int &num) {
    num = colors.size();
    unsigned char **ans = createArray2D<unsigned char>(num, 3);
    for (int i = 0; i < num; i++)
        for (int j = 0; j < 3; j++)
            ans[i][j] = colors[i][j];
    
    return ans;
}