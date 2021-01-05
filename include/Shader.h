#ifndef SHADER_H
#define SHADER_H

#include <bits/stdc++.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
private:
    unsigned int program;

public:
    Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    ~Shader();
    void use();
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setVec2(const std::string &name, glm::vec2 &value);
    void setVec3(const std::string &name, glm::vec3 &value);
    void setVec4(const std::string &name, glm::vec4 &value);
    void setMat4(const std::string &name, glm::mat4 &value);
};

#endif