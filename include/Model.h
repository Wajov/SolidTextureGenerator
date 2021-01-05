#ifndef MODEL_H
#define MODEL_H

#include <bits/stdc++.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Eigen/Dense>

#include "ConstantHelper.h"
#include "ArrayHelper.h"
#include "RandomHelper.h"
#include "Mesh.h"
#include "Image.h"

class Model {
private:
    std::vector<Mesh> meshes;
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

public:
    Model(const std::string &fileName);
    ~Model();
    void render(Shader &shader);
};

#endif