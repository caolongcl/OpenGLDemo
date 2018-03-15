#include <app/objectApp.hpp>
#include <iostream>

using namespace std;
using namespace RESTART;

unique_ptr<Camera> ObjectApp::m_camera;
bool ObjectApp::m_leftMouse = false;
bool ObjectApp::m_rightMouse = false;

vector<uint32_t> indices;

ObjectApp::ObjectApp()
        : App("objectApp", WIDTH, HEIGHT)
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

void ObjectApp::initControl()
{
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetScrollCallback(m_window, scroll_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void ObjectApp::loadScene()
{
    unique_ptr<Texture> texture(new Texture("girl.jpg"));
    texture->setDefaultProperty();
    m_texture = std::move(texture);

    unique_ptr<ShaderItem> simpleVertex(new ShaderItem("objectApp/simplevs.glsl", ShaderItem::ShaderType::VERTEX));
    unique_ptr<ShaderItem> simpleFrag(new ShaderItem("objectApp/simplefs.glsl", ShaderItem::ShaderType::FRAGMENT));

    unique_ptr<Shader> simple(new Shader());
    simple->attach(*simpleVertex).attach(*simpleFrag).link();
    m_shaderSimple = std::move(simple);

    BuildObjectGeometry();
    BuildRenderItems();
}

void ObjectApp::draw()
{
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    DrawRenderItems();
}

bool ObjectApp::run()
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

void ObjectApp::mouse_callback(GLFWwindow *window, double xpos, double ypos)
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

void ObjectApp::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
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

void ObjectApp::processInput(GLFWwindow *window)
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

void ObjectApp::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    float fov = m_camera->getFovY();
    float aspect = (float) width / height;
    float near = m_camera->getNearZ();
    float far = m_camera->getFarZ();

    m_camera->setLens(fov, aspect, near, far);
}

void ObjectApp::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
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

void ObjectApp::BuildObjectGeometry()
{
    GeometryGenerator geoGen;
    GeometryGenerator::MeshData box = geoGen.CreateBox(1.5f, 0.5f, 1.5f, 3);
    GeometryGenerator::MeshData grid = geoGen.CreateGrid(20.0f, 30.0f, 60, 40);
    GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
    GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);

    //
    // We are concatenating all the geometry into one big vertex/index buffer.  So
    // define the regions in the buffer each submesh covers.
    //

    // Cache the vertex offsets to each object in the concatenated vertex buffer.
    uint32_t boxVertexOffset = 0;
    uint32_t gridVertexOffset = (uint32_t) box.vertices.size();
    uint32_t sphereVertexOffset = gridVertexOffset + (uint32_t) grid.vertices.size();
    uint32_t cylinderVertexOffset = sphereVertexOffset + (uint32_t) sphere.vertices.size();

    // Cache the starting index for each object in the concatenated index buffer.
    uint32_t boxIndexOffset = 0;
    uint32_t gridIndexOffset = (uint32_t) box.indices.size();
    uint32_t sphereIndexOffset = gridIndexOffset + (uint32_t) grid.indices.size();
    uint32_t cylinderIndexOffset = sphereIndexOffset + (uint32_t) sphere.indices.size();

    // Define the SubmeshGeometry that cover different
    // regions of the vertex/index buffers.

    SubmeshGeometry boxSubmesh;
    boxSubmesh.indexCount = (uint32_t) box.indices.size();
    boxSubmesh.startIndexLocation = boxIndexOffset;
    boxSubmesh.baseVertexLocation = boxVertexOffset;

    SubmeshGeometry gridSubmesh;
    gridSubmesh.indexCount = (uint32_t) grid.indices.size();
    gridSubmesh.startIndexLocation = gridIndexOffset;
    gridSubmesh.baseVertexLocation = gridVertexOffset;

    SubmeshGeometry sphereSubmesh;
    sphereSubmesh.indexCount = (uint32_t) sphere.indices.size();
    sphereSubmesh.startIndexLocation = sphereIndexOffset;
    sphereSubmesh.baseVertexLocation = sphereVertexOffset;

    SubmeshGeometry cylinderSubmesh;
    cylinderSubmesh.indexCount = (uint32_t) cylinder.indices.size();
    cylinderSubmesh.startIndexLocation = cylinderIndexOffset;
    cylinderSubmesh.baseVertexLocation = cylinderVertexOffset;

    //
    // Extract the vertex elements we are interested in and pack the
    // vertices of all the meshes into one vertex buffer.
    //

    auto totalVertexCount =
            box.vertices.size() +
            grid.vertices.size() +
            sphere.vertices.size() +
            cylinder.vertices.size();

    vector<Vertex> vertices(totalVertexCount);

    uint32_t k = 0;
    for (size_t i = 0; i < box.vertices.size(); ++i, ++k)
    {
        vertices[k].pos = box.vertices[i].position;
        vertices[k].color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
    }

    for (size_t i = 0; i < grid.vertices.size(); ++i, ++k)
    {
        vertices[k].pos = grid.vertices[i].position;
        vertices[k].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    for (size_t i = 0; i < sphere.vertices.size(); ++i, ++k)
    {
        vertices[k].pos = sphere.vertices[i].position;
        vertices[k].color = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
    }

    for (size_t i = 0; i < cylinder.vertices.size(); ++i, ++k)
    {
        vertices[k].pos = cylinder.vertices[i].position;
        vertices[k].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    //vector<uint32_t> indices;
    indices.insert(indices.end(), box.indices.begin(), box.indices.end());
    indices.insert(indices.end(), grid.indices.begin(), grid.indices.end());
    indices.insert(indices.end(), sphere.indices.begin(), sphere.indices.end());
    indices.insert(indices.end(), cylinder.indices.begin(), cylinder.indices.end());

    const uint32_t vbByteSize = (uint32_t) vertices.size() * sizeof(Vertex);
    const uint32_t ibByteSize = (uint32_t) indices.size() * sizeof(uint32_t);

    //glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    //glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    cerr << sizeof(Vertex) << " " << vertices.size() << "\n";
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>((0)));
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>((3 * sizeof(float))));
//    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

    //glBindVertexArray(0);

    auto geo = std::make_unique<MeshGeometry>();
    geo->name = "shapeGeo";

    geo->vertexByteStride = sizeof(Vertex);
    geo->vertexBufferByteSize = vbByteSize;
    geo->indexBufferByteSize = ibByteSize;

    geo->drawArgs["box"] = boxSubmesh;
    geo->drawArgs["grid"] = gridSubmesh;
    geo->drawArgs["sphere"] = sphereSubmesh;
    geo->drawArgs["cylinder"] = cylinderSubmesh;

    m_geometries[geo->name] = std::move(geo);
}

void ObjectApp::BuildRenderItems()
{
    auto boxRitem = std::make_unique<RenderItem>();
    boxRitem->world = glm::translate(boxRitem->world, glm::vec3(0.0f, 0.5f, 0.0f));
    boxRitem->world = glm::scale(boxRitem->world, glm::vec3(2.0f, 2.0f, 2.0));

    boxRitem->geo = m_geometries["shapeGeo"].get();
    boxRitem->indexCount = boxRitem->geo->drawArgs["box"].indexCount;
    boxRitem->startIndexLocation = boxRitem->geo->drawArgs["box"].startIndexLocation;
    boxRitem->baseVertexLocation = boxRitem->geo->drawArgs["box"].baseVertexLocation;

    m_allRitems.push_back(std::move(boxRitem));

    auto gridRitem = std::make_unique<RenderItem>();
    gridRitem->world = MathHelp::identityMat4();
    gridRitem->geo = m_geometries["shapeGeo"].get();
    gridRitem->indexCount = gridRitem->geo->drawArgs["grid"].indexCount;
    gridRitem->startIndexLocation = gridRitem->geo->drawArgs["grid"].startIndexLocation;
    gridRitem->baseVertexLocation = gridRitem->geo->drawArgs["grid"].baseVertexLocation;

    m_allRitems.push_back(std::move(gridRitem));

    for (int i = 0; i < 5; ++i)
    {
        auto leftCylRitem = std::make_unique<RenderItem>();
        auto rightCylRitem = std::make_unique<RenderItem>();
        auto leftSphereRitem = std::make_unique<RenderItem>();
        auto rightSphereRitem = std::make_unique<RenderItem>();

        leftCylRitem->world = glm::translate(leftCylRitem->world, glm::vec3(-5.0f, 1.5f, -10.0f + i * 5.0f));
        rightCylRitem->world = glm::translate(rightCylRitem->world, glm::vec3(+5.0f, 1.5f, -10.0f + i * 5.0f));
        leftSphereRitem->world = glm::translate(leftSphereRitem->world, glm::vec3(-5.0f, 3.5f, -10.0f + i * 5.0f));
        rightSphereRitem->world = glm::translate(rightSphereRitem->world, glm::vec3(+5.0f, 3.5f, -10.0f + i * 5.0f));

        leftCylRitem->geo = m_geometries["shapeGeo"].get();
        leftCylRitem->indexCount = leftCylRitem->geo->drawArgs["cylinder"].indexCount;
        leftCylRitem->startIndexLocation = leftCylRitem->geo->drawArgs["cylinder"].startIndexLocation;
        leftCylRitem->baseVertexLocation = leftCylRitem->geo->drawArgs["cylinder"].baseVertexLocation;

        rightCylRitem->geo = m_geometries["shapeGeo"].get();
        rightCylRitem->indexCount = rightCylRitem->geo->drawArgs["cylinder"].indexCount;
        rightCylRitem->startIndexLocation = rightCylRitem->geo->drawArgs["cylinder"].startIndexLocation;
        rightCylRitem->baseVertexLocation = rightCylRitem->geo->drawArgs["cylinder"].baseVertexLocation;

        leftSphereRitem->geo = m_geometries["shapeGeo"].get();
        leftSphereRitem->indexCount = leftSphereRitem->geo->drawArgs["sphere"].indexCount;
        leftSphereRitem->startIndexLocation = leftSphereRitem->geo->drawArgs["sphere"].startIndexLocation;
        leftSphereRitem->baseVertexLocation = leftSphereRitem->geo->drawArgs["sphere"].baseVertexLocation;

        rightSphereRitem->geo = m_geometries["shapeGeo"].get();
        rightSphereRitem->indexCount = rightSphereRitem->geo->drawArgs["sphere"].indexCount;
        rightSphereRitem->startIndexLocation = rightSphereRitem->geo->drawArgs["sphere"].startIndexLocation;
        rightSphereRitem->baseVertexLocation = rightSphereRitem->geo->drawArgs["sphere"].baseVertexLocation;

        m_allRitems.push_back(std::move(leftCylRitem));
        m_allRitems.push_back(std::move(rightCylRitem));
        m_allRitems.push_back(std::move(leftSphereRitem));
        m_allRitems.push_back(std::move(rightSphereRitem));
    }

    for (auto& item : m_allRitems)
    {
        glGenVertexArrays(1, &item->vao);
        //glGenBuffers(1, &item->ebo);

        glBindVertexArray(item->vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, item->geo->vertexByteStride, reinterpret_cast<GLvoid *>((item->geo->vertexByteStride * item->baseVertexLocation)));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, item->geo->vertexByteStride, reinterpret_cast<GLvoid *>((item->geo->vertexByteStride * item->baseVertexLocation) + 3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, item->ebo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * item->indexCount, sizeof(uint32_t) * item->startIndexLocation + indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }
}

void ObjectApp::DrawRenderItems()
{
    for (auto& item : m_allRitems)
    {
        m_camera->updateViewMatrix();

        m_shaderSimple->activate();
        m_shaderSimple->bind("model", item->world);
        m_shaderSimple->bind("view", m_camera->getView());
        m_shaderSimple->bind("projection", m_camera->getProj());

        glBindVertexArray(item->vao);
        if (m_isWireFrame)
            glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE );
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, item->indexCount, GL_UNSIGNED_INT, reinterpret_cast<GLvoid *>((item->startIndexLocation * sizeof(uint32_t))));
        glBindVertexArray(0);

    }
}