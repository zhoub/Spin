cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(Spin)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(Spin Spin.cpp)
target_include_directories(Spin INTERFACE glfw)
target_link_libraries(Spin OpenGL::GL glfw glm::glm)
