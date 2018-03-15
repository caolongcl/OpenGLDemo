#include <app/shapeApp.hpp>
#include <iostream>

using namespace std;
using namespace RESTART;

unique_ptr<Camera> ShapeApp::m_camera;

ShapeApp::ShapeApp()
        : App("ShapeApp", WIDTH, HEIGHT)
{
    unique_ptr<Camera> camera(new Camera());
    m_camera = std::move(camera);

    m_camera->setLens(45.0f, (float) m_width / m_height, 0.1f, 100.0f);
    m_camera->LookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_lastFrame = 0.0f;
    m_deltaTime = 0.0f;

    initControl();
    loadScene();
    run();
}

void ShapeApp::initControl()
{
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetScrollCallback(m_window, scroll_callback);

    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ShapeApp::loadScene()
{
    unique_ptr<Texture> texture(new Texture("girl.jpg"));
    texture->setDefaultProperty();
    m_texture = std::move(texture);
    m_texture->genMipmap();

    unique_ptr<ShaderItem> simpleVertex(new ShaderItem("shapeApp/simple.vs", ShaderItem::ShaderType::VERTEX));
    unique_ptr<ShaderItem> simpleFrag(new ShaderItem("shapeApp/simple.fs", ShaderItem::ShaderType::FRAGMENT));

    unique_ptr<Shader> simple(new Shader());
    simple->attach(*simpleVertex).attach(*simpleFrag).link();
    m_shaderSimple = std::move(simple);

//    glm::vec4 bi{};
//    float B;
//    float sigma = 0.1;
//
//    computeCoefs(bi, B, sigma);

    unique_ptr<ShaderItem> blurXFrag(new ShaderItem("shapeApp/blurX.fs", ShaderItem::ShaderType::FRAGMENT));

    unique_ptr<Shader> blurX(new Shader());
    blurX->attach(*simpleVertex).attach(*blurXFrag).link();
    m_shaderBlurX = std::move(blurX);

    unique_ptr<ShaderItem> blurYFrag(new ShaderItem("shapeApp/blurY.fs", ShaderItem::ShaderType::FRAGMENT));

    unique_ptr<Shader> blurY(new Shader());
    blurY->attach(*simpleVertex).attach(*blurYFrag).link();
    m_shaderBlurY = std::move(blurY);

    GLfloat vertices[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };
    GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void ShapeApp::draw()
{
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera->updateViewMatrix();

    m_shaderSimple->activate();
    m_shaderSimple->bind("sampler", 0);

    glm::mat4 model = MathHelp::identityMat4();
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    //model = glm::rotate(model, glm::radians(30.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    m_shaderSimple->bind("model", model);

    m_shaderSimple->bind("view", m_camera->getView());
    m_shaderSimple->bind("projection", m_camera->getProj());

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

bool ShapeApp::run()
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 1920, 1080);

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

void ShapeApp::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    static bool isFirstMouse = true;
    static double lastX = WIDTH / 2.0;
    static double lastY = HEIGHT / 2.0;

    if (isFirstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        isFirstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_camera->rotate(xoffset, yoffset);
}

void ShapeApp::processInput(GLFWwindow *window)
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
}

void ShapeApp::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    float fov = m_camera->getFovY();
    float aspect = (float) width / height;
    float near = m_camera->getNearZ();
    float far = m_camera->getFarZ();

    m_camera->setLens(fov, aspect, near, far);

    cerr << "size_frame\n";
}

void ShapeApp::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
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

void ShapeApp::computeCoefs(glm::vec4& bi, float& B, float sigma)
{
    float q, q2, q3;

    q = 0;

    if (sigma >= 2.5)
    {
        q = 0.98711 * sigma - 0.96330;
    }
    else if ((sigma >= 0.5) && (sigma < 2.5))
    {
        q = 3.97156 - 4.14554 * (float) sqrt((double) 1 - 0.26891 * sigma);
    }
    else
    {
        q = 0.1147705018520355224609375;
    }

    q2 = q * q;
    q3 = q * q2;

    bi.w = (1.57825 + (2.44413 * q) + (1.4281 * q2) + (0.422205 * q3));
    bi.x = ((2.44413 * q) + (2.85619 * q2) + (1.26661 * q3));
    bi.y = (-((1.4281 * q2) + (1.26661 * q3)));
    bi.z = ((0.422205 * q3));
    B = 1.0 - ((bi.x + bi.y + bi.z) / bi.w);

//    fprintf(stderr, "%10.9f\n", bi.w);
//    fprintf(stderr, "%10.9f\n", bi.x);
//    fprintf(stderr, "%10.9f\n", bi.y);
//    fprintf(stderr, "%10.9f\n", bi.z);
//    fprintf(stderr, "%10.9f\n", B);
}