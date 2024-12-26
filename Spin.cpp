//
// Copyright © 2024 Bo Zhou<Bo.Schwarzstein@gmail.com>
// This work is free.You can redistribute it and /or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar.See the COPYING file or http://www.wtfpl.net/ 
// for more details.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>

const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

// Object transform, to be updated in keyboard_callback().
//
glm::mat4x4 object_xform(1);

void keyborad_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }
    else {
        static const float DELTA = 1.0f;
        if (key == GLFW_KEY_X || key == GLFW_KEY_Y || key == GLFW_KEY_Z) {
            // Define rotation axis, X, Y, or Z.
            //
            glm::vec3 axis(0, 0, 0);
            axis[key - GLFW_KEY_X] = 1;

            // Update object transform by rotation quaterion.
            //
            const auto& q = glm::angleAxis(glm::radians(DELTA), axis);
            glm::mat4x4 qm = glm::toMat4(q);
            object_xform = object_xform * qm;

            // Decompose object transform.
            //
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(object_xform, scale, rotation, translation, skew, perspective);

            // Get new rotation in degrees.
            //
            const auto& angles = glm::degrees(glm::eulerAngles(rotation));
            std::cout << "rotation: " << angles.x << ' ' << angles.y << ' ' << angles.z << std::endl;
            std::cout << "translation: " << translation.x << ' ' << translation.y << ' ' << translation.z << std::endl;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Spin", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyborad_callback);

    // Set basic object transform, move object to (0, 0, -4)).
    //
    object_xform = glm::translate(object_xform, glm::vec3(0, 0, -4));

    // Setup camera view transform and perspective transform.
    //
    glm::mat4x4 view_xform = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4x4 persp_xform = glm::perspectiveFov<double>(45, SCR_WIDTH, SCR_HEIGHT, 0.01, 1000);

    // Enter render loop.
    //
    while (!glfwWindowShouldClose(window)) {
        // Enable GL depth test.
        //
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Clear buffers as a new frame.
        //
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Setup projection and view matrix.
        //
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(persp_xform));

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(view_xform));

        // Draw objects with object transform.
        //
        glPushMatrix();
        glMultMatrixf(glm::value_ptr(object_xform));

        glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        glVertex3f(1, 2, -0.5);
        glColor3f(1, 0, 0);
        glVertex3f(2, 2, -0.5);
        glColor3f(1, 0, 0);
        glVertex3f(2, 1, -0.5);
        glColor3f(1, 0, 0);
        glVertex3f(1, 1, -0.5);

        glColor3f(0, 1, 0);
        glVertex3f(1, 1, -0.5);
        glColor3f(0, 1, 0);
        glVertex3f(2, 1, -0.5);
        glColor3f(0, 1, 0);
        glVertex3f(2, 1, 0.5);
        glColor3f(0, 1, 0);
        glVertex3f(1, 1, 0.5);

        glColor3f(0, 0, 1);
        glVertex3f(1, 1, -0.5);
        glColor3f(0, 0, 1);
        glVertex3f(1, 1, 0.5);
        glColor3f(0, 0, 1);
        glVertex3f(1, 2, 0.5);
        glColor3f(0, 0, 1);
        glVertex3f(1, 2, -0.5);
        glEnd();

        glPopMatrix();

        // Draw axis as helper.
        //
        float AXIS_LENGTH = 1e3;
        glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(AXIS_LENGTH, 0, 0);
        glColor3f(0.5, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(-AXIS_LENGTH, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, AXIS_LENGTH, 0);
        glColor3f(0, 0.5, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -AXIS_LENGTH, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, AXIS_LENGTH);
        glColor3f(0, 0, 0.5);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -AXIS_LENGTH);
        glEnd();

        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

    return 0;
}
