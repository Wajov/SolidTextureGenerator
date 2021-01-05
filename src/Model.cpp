#include "Model.h"

Model::Model(const std::string &fileName) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Failed to load model:" << std::endl << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);

    float xMin, xMax, yMin, yMax, zMin, zMax;
    xMin = yMin = zMin = 1e38f;
    xMax = yMax = zMax = -1e38f;
    for (auto & mesh : meshes) {
        float xMinTemp, xMaxTemp, yMinTemp, yMaxTemp, zMinTemp, zMaxTemp;
        mesh.coordinateRange(xMinTemp, xMaxTemp, yMinTemp, yMaxTemp, zMinTemp, zMaxTemp);
        xMin = std::min(xMin, xMinTemp);
        xMax = std::max(xMax, xMaxTemp);
        yMin = std::min(yMin, yMinTemp);
        yMax = std::max(yMax, yMaxTemp);
        zMin = std::min(zMin, zMinTemp);
        zMax = std::max(zMax, zMaxTemp);
    }
    glm::vec3 center((xMin + xMax) / 2, (yMin + yMax) / 2, (zMin + zMax) / 2);
    for (auto & mesh : meshes) {
        mesh.recenter(center);
        mesh.bind();
    }

    int num = 0;
    unsigned char *data = createArray1D<unsigned char>(RESOLUTION * RESOLUTION * RESOLUTION * 3);
    std::ifstream fin("output/texture.txt");
    Image image("image/image_" + std::to_string(USED_IMAGE) + ".png");
    Eigen::Matrix<double, 3, 1> mean = image.getMean();
    Eigen::Matrix<double, 3, 3> covar = image.getCovar();
    for (int i = 0; i < RESOLUTION; i++)
        for (int j = 0; j < RESOLUTION; j++)
            for (int k = 0; k < RESOLUTION; k++) {
                Eigen::Matrix<double, 3, 1> noise = randomNormal3D(mean, covar);
                for (int h = 0; h < 3; h++) {
                    int temp;
                    fin >> temp;
                    temp += round(noise(h, 0));
                    temp = std::max(temp, 0);
                    temp = std::min(temp, 255);
                    data[num++] = temp;
                }
            }
    fin.close();

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_3D, textureId);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, RESOLUTION, RESOLUTION, RESOLUTION, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_3D);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Model::~Model() {}

void Model::render(Shader &shader) {
    for (auto & mesh : meshes)
        mesh.render(shader);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    glm::mat4 trans(1.0f);
    trans = glm::rotate(trans, (float)PI * 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
    trans = glm::rotate(trans, (float)PI * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    trans = glm::rotate(trans, (float)PI * 0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        glm::vec3 uv(glm::vec4(position * glm::vec3(2.5f), 1.0f) * trans);
        vertices.push_back(Vertex(position, normal, uv));
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);

    return Mesh(vertices, indices);
}