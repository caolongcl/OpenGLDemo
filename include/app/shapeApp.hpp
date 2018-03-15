#pragma once

#include <base.hpp>
#include <core/app.hpp>
#include <core/texture.hpp>
#include <core/shader.hpp>
#include <core/camera.hpp>
#include <memory>

namespace RESTART
{

class ShapeApp : public App
{
public:
    ShapeApp();

    ~ShapeApp() {}

    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    void processInput(GLFWwindow *window);

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static std::unique_ptr<Camera> m_camera;

private:
    void computeCoefs(glm::vec4& bi, float& B, float sigma);

    void initControl();

    bool run();

    void draw();

    void loadScene();

    //std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> m_geometries;
    //std::vector<std::unique_ptr<RenderItem>> m_allRitems;

    std::unique_ptr<Shader> m_shaderSimple;
    std::unique_ptr<Shader> m_shaderBlurX;
    std::unique_ptr<Shader> m_shaderBlurY;
    std::unique_ptr<Texture> m_texture;

    float m_deltaTime;
    float m_lastFrame;

    GLuint vao, vbo, ebo;
};

};