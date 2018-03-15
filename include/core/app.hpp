#pragma once

#include <base.hpp>
#include <core/camera.hpp>

#include <string>
#include <memory>

namespace RESTART
{

class App
{
public:
    App();

    App(const std::string& name, int width, int height);

    virtual ~App() { glfwTerminate(); };

protected:
    virtual bool run() = 0;

    virtual void initialize();

    virtual void initControl() = 0;

protected:
    std::string m_name;
    int m_width;
    int m_height;
    GLFWwindow *m_window;

    std::unique_ptr<Camera> m_camera;

private:
    App(const App&) = delete;

    App& operator=(const App&) = delete;
};

};