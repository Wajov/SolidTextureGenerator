#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
    this->vertices = vertices;
    this->indices = indices;
}

Mesh::~Mesh() {}

void Mesh::coordinateRange(float &xMin, float &xMax, float &yMin, float &yMax, float &zMin, float &zMax) {
    xMin = yMin = zMin = 1e38f;
    xMax = yMax = zMax = -1e38f;
    for (unsigned int i = 0; i < vertices.size(); i++) {
        xMin = std::min(xMin, vertices[i].position.x);
        xMax = std::max(xMax, vertices[i].position.x);
        yMin = std::min(yMin, vertices[i].position.y);
        yMax = std::max(yMax, vertices[i].position.y);
        zMin = std::min(zMin, vertices[i].position.z);
        zMax = std::max(zMax, vertices[i].position.z);
    }
}

void Mesh::recenter(glm::vec3 &center) {
    for (int i = 0; i < vertices.size(); i++)
        vertices[i].position -= center;
}

void Mesh::bind() {
    unsigned vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &(this->vertices[0]), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &(this->indices[0]), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

void Mesh::render(Shader &shader) {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}