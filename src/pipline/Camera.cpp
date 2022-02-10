#include "Camera.h"
#include "pipline/window.h"

using namespace Pipline;

void Camera::updateProjectionMat4()
{
    if (!m_ProjectionMatChanged)
        return;
    m_ProjectionMatChanged = false;

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(m_fov), (float)m_window->getWindowWidth() / (float)m_window->getWindowHeight(), m_nearDistance, m_farDistance);
    m_projectionMat4 = projection;
}

void Camera::updateViewMat4()
{
    if (!m_ViwMatChanged)
        return;
    m_ViwMatChanged = false;

    glm::vec3 front;
    //front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    //front.y = sin(glm::radians(m_pitch));
    //front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.x = sin(glm::radians(-m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = -cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);

    //std::cout << m_cameraPos.x << " " << m_cameraPos.y << " " << m_cameraPos.z << std::endl;
    m_viewMat4 = glm::lookAt(m_cameraPos, m_cameraPos + m_front, getCameraFinalUp());
}

void Camera::fovFilter()
{
    m_ProjectionMatChanged = true;
    if (m_fovMax == m_fovMin)
        return;
    if (m_fov > m_fovMax)
        m_fov = m_fovMax;
    else if (m_fov < m_fovMax)
        m_fov = m_fovMin;
}

void Camera::yawFilter()
{
    m_ViwMatChanged = true;
    if (m_yawMax == m_yawMin)
        return;
    if (m_yaw > m_yawMax)
        m_yaw = m_yawMax;
    else if (m_yaw < m_yawMin)
        m_yaw = m_yawMin;
}

void Camera::pitchFilter()
{
    m_ViwMatChanged = true;
    if (m_pitchMax == m_pitchMin)
        return;
    if (m_pitch > m_pitchMax)
        m_pitch = m_pitchMax;
    else if (m_pitch < m_pitchMin)
        m_pitch = m_pitchMin;
}

void CameraControl::initKeys()
{
    m_dirToKeys.resize(dir::End);
    addKeyAction(dir::MoveForward, GLFW_KEY_W, GLFW_PRESS);
    addKeyAction(dir::MoveBackward, GLFW_KEY_S, GLFW_PRESS);
    addKeyAction(dir::MoveDown, GLFW_KEY_LEFT_CONTROL, GLFW_PRESS);
    addKeyAction(dir::MoveUp, GLFW_KEY_SPACE, GLFW_PRESS);
    addKeyAction(dir::MoveLeft, GLFW_KEY_A, GLFW_PRESS);
    addKeyAction(dir::MoveRight, GLFW_KEY_D, GLFW_PRESS);

    addKeyAction(dir::TurnDown, GLFW_KEY_DOWN, GLFW_PRESS);
    addKeyAction(dir::TurnUp, GLFW_KEY_UP, GLFW_PRESS);
    addKeyAction(dir::TurnLeft, GLFW_KEY_LEFT, GLFW_PRESS);
    addKeyAction(dir::TurnRight, GLFW_KEY_RIGHT, GLFW_PRESS);
}

bool CameraControl::isTriggered(dir dir) const
{
    for (auto it : m_dirToKeys[dir])
    {
        if (glfwGetKey(m_cam->getWindow().getGLFWwindow(), it.first) == it.second)
            return true;
    }

    return false;
}

void CameraControl::processActions() const
{
    if (!m_enable)
        return;

    glm::vec3 cameraPos = m_cam->getCameraPos();
    glm::vec3 cameraFront = m_cam->getCameraFront();
    glm::vec3 cameraUp = m_cam->getCameraUp();
    float cameraSpeed = m_cam->getSensitive();
    if (isTriggered(dir::MoveForward))
        cameraPos += cameraSpeed * cameraFront;
    if (isTriggered(dir::MoveBackward))
        cameraPos -= cameraSpeed * cameraFront;
    if (isTriggered(dir::MoveLeft))
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (isTriggered(dir::MoveRight))
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (isTriggered(dir::TurnRight))
        m_cam->addYaw(-10.f);
    if (isTriggered(dir::TurnLeft))
        m_cam->addYaw(10.f);
    if (isTriggered(dir::TurnUp))
        m_cam->addPitch(10.f);
    if (isTriggered(dir::TurnDown))
        m_cam->addPitch(-10.f);

    glm::vec3 oriPos = m_cam->getCameraPos();
    if (oriPos != cameraPos)
        m_cam->setCameraPos(cameraPos);
}