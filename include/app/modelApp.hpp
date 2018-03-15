#pragma once

#include <base.hpp>
#include <core/app.hpp>
#include <core/shader.hpp>
#include <core/camera.hpp>
#include <mesh.hpp>
#include <memory>
#include <vector>

namespace RESTART
{

class ModelApp : public App
{
public:
    ModelApp();

    ~ModelApp() {}

    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    void processInput(GLFWwindow *window);

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static std::unique_ptr<Camera> m_camera;

    static bool m_leftMouse;

    static bool m_rightMouse;
private:

    void initControl();

    bool run();

    void draw();

    void loadScene();

    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Mesh> m_nanosuit;

    float m_deltaTime;
    float m_lastFrame;

    bool m_isWireFrame;
};

};