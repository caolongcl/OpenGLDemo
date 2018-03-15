#include <core/camera.hpp>

using namespace std;
using namespace RESTART;

Camera::Camera()
{
    //setLens(0.25f * M_PI, 1.0f, 1.0f, 1000.0f);
}

Camera::~Camera()
{
}

const glm::vec3& Camera::getPosition() const
{
    return m_position;
}

void Camera::setPosition(float x, float y, float z)
{
    m_position = glm::vec3(x, y, z);
    m_isValid = false;
}

void Camera::setPosition(const glm::vec3& v)
{
    m_position = v;
    m_isValid = false;
}

const glm::vec3& Camera::getRight() const
{
    return m_right;
}

const glm::vec3& Camera::getUp() const
{
    return m_up;
}

const glm::vec3& Camera::getLook() const
{
    return m_look;
}

float Camera::getNearZ() const
{
    return m_nearZ;
}

float Camera::getFarZ() const
{
    return m_farZ;
}

float Camera::getAspect() const
{
    return m_aspect;
}

float Camera::getFovY() const
{
    return m_fovY;
}

float Camera::getFovX() const
{
    float halfWidth = 0.5f * getNearWindowWidth();
    return 2.0f * atan(halfWidth / m_nearZ);
}

float Camera::getNearWindowWidth() const
{
    return m_aspect * m_nearWindowHeight;
}

float Camera::getNearWindowHeight() const
{
    return m_nearWindowHeight;
}

float Camera::getFarWindowWidth() const
{
    return m_aspect * m_farWindowHeight;
}

float Camera::getFarWindowHeight() const
{
    return m_farWindowHeight;
}

void Camera::setLens(float fovY, float aspect, float zn, float zf)
{
    m_fovY = fovY;
    m_aspect = aspect;
    m_nearZ = zn;
    m_farZ = zf;

    m_nearWindowHeight = 2.0f * m_nearZ * tanf(0.5f * m_fovY);
    m_farWindowHeight = 2.0f * m_farZ * tanf(0.5f * m_fovY);

    m_proj = glm::perspective(glm::radians(m_fovY), m_aspect, m_nearZ, m_farZ);
}

void Camera::LookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& worldUp)
{
    m_position = pos;

    m_look = target;
    m_look -= pos;
    m_look = glm::normalize(m_look);

    glm::vec3 z(-m_look.x, -m_look.y, -m_look.z);

    m_right = glm::normalize(glm::cross(worldUp, z));

    m_up = glm::cross(m_look, z);
    
    m_isValid = false;
}

const glm::mat4& Camera::getView() const
{
    return m_view;
}

const glm::mat4& Camera::getProj() const
{
    return m_proj;
}

void Camera::strafe(float d)
{
    // m_position += d*m_right

    glm::vec3 delta = m_right;
    delta *= d;

    m_position += delta;

    m_isValid = false;
}

void Camera::walk(float d)
{
    // m_position += d*m_look

    glm::vec3 delta = m_look;
    delta *= d;

    m_position += delta;

    m_isValid = false;
}

void Camera::pitch(float angle)
{
    // Rotate up and look vector about the right vector.
/*
    glm::mat4 r;

    r = glm::rotate(r, glm::radians(angle), m_right);
    float

    XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
    XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), R));
*/
    m_pitch += angle;

    if(m_pitch > 89.0f)
        m_pitch =  89.0f;
    if(m_pitch < -89.0f)
        m_pitch = -89.0f;

    m_isValid = false;
}

void Camera::rotateY(float angle)
{
    // Rotate the basis vectors about the world y-axis.
/*
    glm::mat4& R = XMMatrixRotationY(angle);

    XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), R));
    XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
    XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), R));
*/
    m_yaw += angle;

    m_isValid = false;
}

void Camera::rotate(float x, float y)
{
    m_pitch += y;

    if(m_pitch > 89.0f)
        m_pitch =  89.0f;
    if(m_pitch < -89.0f)
        m_pitch = -89.0f;

    m_yaw += x;

    glm::vec3 front{};
    front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    front.y = sin(glm::radians(m_pitch));
    front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    front = glm::normalize(front);

    m_look = front;glm::vec3(-front.x, -front.y, -front.z);

    m_isValid = false;
}

void Camera::updateViewMatrix()
{
    if (!m_isValid)
    {
        glm::vec3 target = m_position;
        target += m_look;

        m_view = glm::lookAt(m_position, target, glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

