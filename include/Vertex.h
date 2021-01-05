#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class Vertex {
public:
    glm::vec3 position, normal, uv;
    Vertex(glm::vec3 &position, glm::vec3 &normal, glm::vec3 &uv);
    ~Vertex();
};

#endif