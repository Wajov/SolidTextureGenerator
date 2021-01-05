cmake_minimum_required(VERSION 3.0)
project(SolidTextureGenerator)

include_directories(include)
aux_source_directory(src SOURCE)
add_executable(SolidTextureGenerator ${SOURCE})

# CGAL
find_package(CGAL REQUIRED COMPONENTS Core Qt5)
add_definitions(-DCGAL_USE_BASIC_VIEWER)

# Eigen
find_package(Eigen3)
include_directories(${EIGEN3_INCLUDE_DIR})

# OpenGL
find_package(OpenGL REQUIRED)

# GLAD
add_subdirectory(lib/glad)

# GLFW
find_package(glfw3 REQUIRED)

# Assimp
find_package(assimp REQUIRED)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
target_link_libraries(
    SolidTextureGenerator
    CGAL::CGAL
    CGAL::CGAL_Core
    CGAL::CGAL_Qt5
    glad
    glfw
    assimp
)