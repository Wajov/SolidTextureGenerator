#ifndef MODEL_HELPER_H
#define MODEL_HELPER_H

#include <bits/stdc++.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ConstantHelper.h"
#include "Shader.h"
#include "Model.h"

static int lastX = INT_MIN, lastY = INT_MIN;
static float factor = 1.0f;
static bool press;
static glm::mat4 rotate(1.0f);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double x, double y);
void scrollCallback(GLFWwindow *window, double x, double y);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void drawModel(const std::string &fileName);

#endif
