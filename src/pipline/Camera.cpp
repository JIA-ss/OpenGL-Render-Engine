#include "Camera.h"
#include "pipline/window.h"
#include "input/InputManager.h"
#include "graphic/GlobalShaderParam.h"
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
    glm::vec3 target = m_cameraPos + m_front;
    if (m_useTarget)
        target = m_target;

    //std::cout << m_cameraPos.x << " " << m_cameraPos.y << " " << m_cameraPos.z << std::endl;
    m_viewMat4 = glm::lookAt(m_cameraPos, target, getCameraFinalUp());
}

void Camera::fovFilter()
{
    m_ProjectionMatChanged = true;
    if (m_fovMax == m_fovMin)
        return;
    if (m_fov > m_fovMax)
        m_fov = m_fovMax;
    else if (m_fov < m_fovMin)
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
    m_actionToKeys.resize(CameraActions::End);
    addKeyAction(CameraActions::MoveForward, GLFW_KEY_W, GLFW_PRESS);
    addKeyAction(CameraActions::MoveBackward, GLFW_KEY_S, GLFW_PRESS);
    addKeyAction(CameraActions::MoveDown, GLFW_KEY_LEFT_CONTROL, GLFW_PRESS);
    addKeyAction(CameraActions::MoveUp, GLFW_KEY_SPACE, GLFW_PRESS);
    addKeyAction(CameraActions::MoveLeft, GLFW_KEY_A, GLFW_PRESS);
    addKeyAction(CameraActions::MoveRight, GLFW_KEY_D, GLFW_PRESS);

    addKeyAction(CameraActions::TurnDown, GLFW_KEY_DOWN, GLFW_PRESS);
    addKeyAction(CameraActions::TurnUp, GLFW_KEY_UP, GLFW_PRESS);
    addKeyAction(CameraActions::TurnLeft, GLFW_KEY_LEFT, GLFW_PRESS);
    addKeyAction(CameraActions::TurnRight, GLFW_KEY_RIGHT, GLFW_PRESS);

    addKeyAction(CameraActions::ZoomIn, GLFW_KEY_Q, GLFW_PRESS);
    addKeyAction(CameraActions::ZoomIn, GLFW_KEY_E, GLFW_PRESS);
}

void CameraControl::initResponses()
{
    m_actionToResposes.resize(CameraActions::End);
    m_actionToResposes[CameraActions::MoveForward] = std::bind(&CameraControl::moveFrontOrBack, this, std::placeholders::_1, true);
    m_actionToResposes[CameraActions::MoveBackward] = std::bind(&CameraControl::moveFrontOrBack, this, std::placeholders::_1, false);
    
    m_actionToResposes[CameraActions::MoveUp] = std::bind(&CameraControl::moveUpOrDown, this, std::placeholders::_1, true);
    m_actionToResposes[CameraActions::MoveDown] = std::bind(&CameraControl::moveUpOrDown, this, std::placeholders::_1, false);
    
    m_actionToResposes[CameraActions::MoveLeft] = std::bind(&CameraControl::moveLeftOrRight, this, std::placeholders::_1, true);
    m_actionToResposes[CameraActions::MoveRight] = std::bind(&CameraControl::moveLeftOrRight, this, std::placeholders::_1, false);

    m_actionToResposes[CameraActions::TurnUp] = std::bind(&CameraControl::turnUpOrDown, this, std::placeholders::_1, true);
    m_actionToResposes[CameraActions::TurnDown] = std::bind(&CameraControl::turnUpOrDown, this, std::placeholders::_1, false);

    m_actionToResposes[CameraActions::TurnLeft] = std::bind(&CameraControl::turnLeftOrRight, this, std::placeholders::_1, true);
    m_actionToResposes[CameraActions::TurnRight] = std::bind(&CameraControl::turnLeftOrRight, this, std::placeholders::_1, false);

    m_actionToResposes[CameraActions::ZoomIn] = std::bind(&CameraControl::zoomInOrOut, this, std::placeholders::_1, true);
    m_actionToResposes[CameraActions::ZoomOut] = std::bind(&CameraControl::zoomInOrOut, this, std::placeholders::_1, false);
}

bool CameraControl::isTriggered(CameraActions dir) const
{
    for (auto it : m_actionToKeys[dir])
    {
        if (glfwGetKey(m_cam->getWindow().getGLFWwindow(), it.first) == it.second)
            return true;
    }

    return false;
}

void CameraControl::moveFrontOrBack(float targetSensitive, bool isFront)
{
    glm::vec3 cameraPos = m_cam->getCameraPos();
    glm::vec3 cameraFront = m_cam->getCameraFront();
    float cameraSpeed = targetSensitive == 0.0f ? m_cam->getSensitive() : targetSensitive;
    if (isFront)
        cameraPos += cameraSpeed * cameraFront;
    else
        cameraPos -= cameraSpeed * cameraFront;
    m_cam->setCameraPos(cameraPos);
}

void CameraControl::moveUpOrDown(float targetSensitive, bool isUp)
{
    glm::vec3 cameraPos = m_cam->getCameraPos();
    glm::vec3 cameraUp = m_cam->getCameraFinalUp();
    float cameraSpeed = targetSensitive == 0.0f ? m_cam->getSensitive() : targetSensitive;
    if (isUp)
        cameraPos += cameraSpeed * cameraUp;
    else
        cameraPos -= cameraSpeed * cameraUp;
    m_cam->setCameraPos(cameraPos);
}

void CameraControl::moveLeftOrRight(float targetSensitive, bool isLeft)
{
    glm::vec3 cameraPos = m_cam->getCameraPos();
    glm::vec3 cameraFront = m_cam->getCameraFront();
    glm::vec3 cameraUp = m_cam->getCameraFinalUp();
    float cameraSpeed = targetSensitive == 0.0f ? m_cam->getSensitive() : targetSensitive;
    if (isLeft)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    else
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    m_cam->setCameraPos(cameraPos);
}

void CameraControl::turnUpOrDown(float targetSensitive, bool isUp)
{
    float value = targetSensitive == 0.0f ? 10.f : targetSensitive;
    value = isUp ? value : -value;
    m_cam->addPitch(value);
}

void CameraControl::turnLeftOrRight(float targetSensitive, bool isLeft)
{
    float value = targetSensitive == 0.0f ? 10.f : targetSensitive;
    value = isLeft ? value : -value;
    m_cam->addYaw(value);
}

void CameraControl::zoomInOrOut(float targetSensitive, bool isZoomIn)
{
    float value = targetSensitive == 0.0f ? 10.f : targetSensitive;
    value = isZoomIn ? value : -value;
    m_cam->addFov(value);
}



void CameraControl::processMouseActions() const
{
    if (!m_mouseEnable)
        return;
    bool mouseMove = glfwGetMouseButton(m_cam->getWindow().getGLFWwindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    Window& window = m_cam->getWindow();
    Vector2 lastMousePos = window.getInputMgr()->getPreMousePos();
    Vector2 curMousePos = window.getInputMgr()->getCurMousePos();
    glm::vec2 posOffset(curMousePos.x - lastMousePos.x, curMousePos.y - lastMousePos.y);
    
    auto proxZero = [](float v)-> float { 
        return (v > -0.01f && v < 0.01f) ? 0.0f : v; 
    };

    if (mouseMove)
    {
        //std::cout << "mouse Pos: " << curMousePos.x << '\t' << curMousePos.y << std::endl;
        float v = proxZero(posOffset.y);
        if (v != 0.0f)
            resolveResponse(CameraActions::TurnUp, v);
        v = proxZero(posOffset.x);
        if (v != 0.0f)
            resolveResponse(CameraActions::TurnLeft, v);
    }

    Vector2 curScrollValue = window.getInputMgr()->getScrollValue();
    if (proxZero(curScrollValue.y) != 0.0f)
    {
        //std::cout << "scroll: " << curScrollValue.x << '\t' << curScrollValue.y << std::endl;
        if (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            glm::vec3 camCurPos = m_cam->getCameraPos();
            float width = m_cam->getWindow().getWindowWidth();
            float height = m_cam->getWindow().getWindowHeight();
            float z = (camCurPos + m_cam->getCameraFront()).z;
            glm::vec3 dir = glm::normalize(glm::vec3(curMousePos.x - width/2, height/2 - curMousePos.y, z));
            camCurPos += curScrollValue.y * m_cam->getSensitive() * dir;
            m_cam->setCameraPos(camCurPos);
        }
        else
            resolveResponse(CameraActions::ZoomIn, - proxZero(curScrollValue.y));
    }
}

void CameraControl::resolveResponse(CameraActions dir, float value) const
{
    m_actionToResposes[dir](value);
}

void CameraControl::processActions() const
{
    if (!m_enable)
        return;
    processMouseActions();
    for (int i = 0; i < CameraActions::End; ++i)
    {
        if (isTriggered((CameraActions)i))
            resolveResponse((CameraActions)i);
    }
}

void Camera::update()
{
    processControl();
    Graphic::GlobalShaderParam* gsp = Graphic::GlobalShaderParam::Get();

    gsp->SubData("GlobalMatrices", 0, sizeof(glm::mat4), glm::value_ptr(getViewMat4()));
    gsp->SubData("GlobalMatrices", sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(getProjectionMat4()));

    gsp->SubData("GlobalPositions", 0, sizeof(glm::vec3), glm::value_ptr(getCameraPos()));
}