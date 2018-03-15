#include <core/app.hpp>

using namespace std;
using namespace RESTART;

App::App()
        : m_name("unnamed App"),
          m_width(WIDTH), m_height(HEIGHT)
{
    initialize();
}

App::App(const std::string& name, int width, int height)
        : m_name(name), m_width(width), m_height(height)
{
    initialize();
}

void App::initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);

    if (m_window == nullptr)
    {
        fprintf(stderr, "Failed to Create OpenGL Context");
        glfwTerminate();
        terminate();
    }

    glfwMakeContextCurrent(m_window);
    gladLoadGL();

    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    fprintf(stderr, "App %s width %d height %d\n", m_name.c_str(), m_width, m_height);
}