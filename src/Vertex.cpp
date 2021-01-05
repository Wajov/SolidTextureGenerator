#include "Vertex.h"

Vertex::Vertex(glm::vec3 &position, glm::vec3 &normal, glm::vec3 &uv) {
    this->position = position;
    this->normal = normal;
    this->uv = uv;
}

Vertex::~Vertex() {}