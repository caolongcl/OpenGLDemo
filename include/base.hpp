#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stb/stb_image.h>

#include <cassert>

#ifndef PROJECT_RESOURCE_DIR
#define PROJECT_RESOURCE_DIR "resource"
#endif

#ifndef RESTART
#define RESTART OpenGLDemo
#endif

#define WIDTH 1920 * 2
#define HEIGHT 1080 * 2

namespace RESTART
{

class MathHelp
{
public:
    static glm::mat4& identityMat4()
    {
        static glm::mat4 i = {glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
                              glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
                              glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
                              glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)};
        return i;
    }
};

}