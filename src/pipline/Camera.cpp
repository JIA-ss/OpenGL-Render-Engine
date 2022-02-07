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
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);

    //std::cout << m_cameraPos.x << " " << m_cameraPos.y << " " << m_cameraPos.z << std::endl;
    m_viewMat4 = glm::lookAt(m_cameraPos, m_cameraPos + m_front, m_up);
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