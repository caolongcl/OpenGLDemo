#pragma once

#include <base.hpp>
#include <core/app.hpp>
#include <core/texture.hpp>
#include <core/shader.hpp>
#include <core/camera.hpp>
#include <object/shapes.hpp>
#include <memory>
#include <vector>
#include <unordered_map>

namespace RESTART
{

class ObjectApp : public App
{
public:
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec4 color;
    };

    ObjectApp();

    ~ObjectApp() {}

    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    void processInput(GLFWwindow *window);

    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static std::unique_ptr<Camera> m_camera;

    static bool m_leftMouse;

    static bool m_rightMouse;
private:
    void BuildObjectGeometry();

    void BuildRenderItems();

    void DrawRenderItems();

    void initControl();

    bool run();

    void draw();

    void loadScene();

    void updateRenderItems() {}

    std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> m_geometries;
    std::vector<std::unique_ptr<RenderItem>> m_allRitems;

    std::unique_ptr<Shader> m_shaderSimple;
    std::unique_ptr<Shader> m_shaderBlurX;
    std::unique_ptr<Shader> m_shaderBlurY;
    std::unique_ptr<Texture> m_texture;

    float m_deltaTime;
    float m_lastFrame;

    bool m_isWireFrame;

    GLuint vao, vbo, ebo;
};

};