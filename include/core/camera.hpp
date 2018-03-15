#pragma once

#include <base.hpp>

namespace RESTART
{

class Camera
{
public:

    Camera();

    ~Camera();

    // get/set world camera position.
    const glm::vec3& getPosition() const;

    void setPosition(float x, float y, float z);

    void setPosition(const glm::vec3& v);

    // get camera basis vectors.
    const glm::vec3& getRight() const;

    const glm::vec3& getUp() const;

    const glm::vec3& getLook() const;

    // get frustum properties.
    float getNearZ() const;

    float getFarZ() const;

    float getAspect() const;

    float getFovY() const;

    float getFovX() const;

    // get near and far plane dimensions in view space coordinates.
    float getNearWindowWidth() const;

    float getNearWindowHeight() const;

    float getFarWindowWidth() const;

    float getFarWindowHeight() const;

    // set frustum.
    void setLens(float fovY, float aspect, float zn, float zf);

    // Define camera space via LookAt parameters.
    void LookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& worldUp);

    // get View/Proj matrices.
    const glm::mat4& getView() const;

    const glm::mat4& getProj() const;

    // Strafe/Walk the camera a distance d.
    void strafe(float d);

    void walk(float d);

    // Rotate the camera.
    void pitch(float angle);

    void rotateY(float angle);

    void rotate(float x, float y);

    // After modifying camera position/orientation, call to rebuild the view matrix.
    void updateViewMatrix();

private:
    // Camera coordinate system with coordinates relative to world space.
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_right = {1.0f, 0.0f, 0.0f};
    glm::vec3 m_up = {0.0f, 1.0f, 0.0f};
    glm::vec3 m_look = {0.0f, 0.0f, 1.0f};

    // Cache frustum properties.
    float m_nearZ = 0.0f;
    float m_farZ = 0.0f;
    float m_aspect = 0.0f;
    float m_fovY = 0.0f;
    float m_nearWindowHeight = 0.0f;
    float m_farWindowHeight = 0.0f;

    float m_pitch = 0.0f;
    float m_yaw = -90.0f;

    // Cache View/Proj matrices.
    glm::mat4 m_view;
    glm::mat4 m_proj;

    bool m_isValid;
};

}