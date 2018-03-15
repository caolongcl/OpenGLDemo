#include <app/modelApp.hpp>
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace RESTART;

unique_ptr<Camera> ModelApp::m_camera;
bool ModelApp::m_leftMouse = false;
bool ModelApp::m_rightMouse = false;

ModelApp::ModelApp()
        : App("ModelApp", WIDTH, HEIGHT)
{
    unique_ptr<Camera> camera(new Camera());
    m_camera = std::move(camera);

    m_camera->setLens(45.0f, (float) m_width / m_height, 1.0f, 1000.0f);
    m_camera->LookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_lastFrame = 0.0f;
    m_deltaTime = 0.0f;

    m_isWireFrame = false;

    initControl();
    loadScene();
    run();
}

void ModelApp::initControl()
{
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetCursorPos(m_window, WIDTH / 4.0, HEIGHT / 4.0);
    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ModelApp::loadScene()
{
    unique_ptr<ShaderItem> simpleVertex(new ShaderItem("nanosuit/nanosuitvs.glsl", ShaderItem::ShaderType::VERTEX));
    unique_ptr<ShaderItem> simpleFrag(new ShaderItem("nanosuit/nanosuitfs.glsl", ShaderItem::ShaderType::FRAGMENT));

    unique_ptr<Shader> simple(new Shader());
    simple->attach(*simpleVertex).attach(*simpleFrag).link();
    m_shader = std::move(simple);

    m_nanosuit = make_unique<Mesh>("nanosuit/nanosuit.obj");
}

void ModelApp::draw()
{
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera->updateViewMatrix();

    m_shader->activate();

    glm::mat4 model = MathHelp::identityMat4();
    model = glm::translate(model, glm::vec3(0.0f, -5.0f, -10.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.50f, 0.5f));

    m_shader->bind("model", model);
    m_shader->bind("view", m_camera->getView());
    m_shader->bind("projection", m_camera->getProj());

    if (m_isWireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    m_nanosuit->draw(m_shader->get());
}

bool ModelApp::run()
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, WIDTH, HEIGHT);

    while (!glfwWindowShouldClose(m_window))
    {
        float currentFrame = glfwGetTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        processInput(m_window);

        draw();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    return true;
}

void ModelApp::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static bool isFirstMouse = true;
    static double lastX = WIDTH / 2.0;
    static double lastY = HEIGHT / 2.0;

    if (m_leftMouse)
    {
        if (isFirstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            isFirstMouse = false;
        }

        float xoffset = -(xpos - lastX);
        float yoffset = -(lastY - ypos);
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.05;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        m_camera->rotate(xoffset, yoffset);
    }
    else
    {
        isFirstMouse = true;
    }
}

void ModelApp::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_leftMouse = true;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            m_rightMouse = true;
            break;
        default:
            return;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_leftMouse = false;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            m_rightMouse = false;
            break;
        default:
            return;
        }
    }
}

void ModelApp::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    float d = 2.5f * m_deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_camera->walk(d);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_camera->walk(-d);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_camera->strafe(-d);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_camera->strafe(d);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        m_isWireFrame = true;

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        m_isWireFrame = false;
}

void ModelApp::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    float fov = m_camera->getFovY();
    float aspect = (float) width / height;
    float near = m_camera->getNearZ();
    float far = m_camera->getFarZ();

    m_camera->setLens(fov, aspect, near, far);
}

void ModelApp::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    float fov = m_camera->getFovY();
    float aspect = m_camera->getAspect();
    float near = m_camera->getNearZ();
    float far = m_camera->getFarZ();

    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;

    m_camera->setLens(fov, aspect, near, far);
}