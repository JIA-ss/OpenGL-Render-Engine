#pragma once
#include "pipline/basicTypes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <functional>
namespace Pipline
{

const glm::vec3 DEFAULT_CAMERA_POS    = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 DEFAULT_CAMERA_UP     = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_CAMERA_FRONT  = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 DEFAULT_CAMERA_TARGET = glm::vec3(0.0f, 0.0f, 0.0f);
const float     DEFAULT_FOV           = 45.0f;
const float     DEFAULT_NEAR_DIS      = 0.1f;
const float     DEFAULT_FAR_DIS       = 100.f;
const float     DEFAULT_CAMERA_DIS    = 3.0f;



class CameraControl
{
    friend class Camera;
public:
    enum CameraActions
    {
        MoveForward = 0,
        MoveBackward = 1,
        MoveLeft = 2,
        MoveRight = 3,
        MoveUp = 4,
        MoveDown = 5,

        TurnUp = 6,
        TurnDown = 7,
        TurnLeft = 8,
        TurnRight = 9,

        ZoomIn = 10,
        ZoomOut = 11,

        End = 12,
    };
private:
    std::vector<std::unordered_map<int, int>> m_actionToKeys;
    std::vector<std::function<void(float)>> m_actionToResposes;
    bool m_enable = false;
    bool m_mouseEnable = true;
    Camera* m_cam;
private:
    void initKeys();
    void initResponses();
    bool isTriggered(CameraActions dir) const;
    void resolveResponse(CameraActions dir, float value = 0.0f) const;
    void processMouseActions() const;
private:
    void moveFrontOrBack(float targetSensitive = 0.0f, bool isFront = true);
    void moveUpOrDown(float targetSensitive = 0.0f, bool isUp = true);
    void moveLeftOrRight(float targetSensitive = 0.0f, bool isLeft = true);

    void turnUpOrDown(float targetSensitive = 0.0f, bool isUp = true);
    void turnLeftOrRight(float targetSensitive = 0.0f, bool isLeft = true);

    void zoomInOrOut(float targetSensitive = 0.0f, bool isZoomIn = true);
public:
    inline void addKeyAction(CameraActions dir, int glfw_key, int glfw_key_state) { m_actionToKeys[dir][glfw_key] = glfw_key_state; }
    void processActions() const;
    inline void enableControl(bool v) { m_enable = v; }
    inline bool isEnable() const { return m_enable; }
    inline void setCamera(Camera* cam) { m_cam = cam; }
    inline Camera* getCamera() const { return m_cam; }

public:
    CameraControl() { initKeys();initResponses(); }
    ~CameraControl() { }
};


class Window;
class Camera
{
private:
    Window* m_window        = nullptr;
    CameraControl m_control;
    glm::vec3 m_cameraPos   = DEFAULT_CAMERA_POS;
    glm::vec3 m_up          = DEFAULT_CAMERA_UP;
    glm::vec3 m_front       = DEFAULT_CAMERA_FRONT;
    glm::vec3 m_target      = DEFAULT_CAMERA_TARGET;

    float m_yaw             = 0.0f;
    float m_pitch           = 0.0f;
    bool m_ViwMatChanged  = true;

    float m_fov             = DEFAULT_FOV;
    float m_nearDistance    = DEFAULT_NEAR_DIS;
    float m_farDistance     = DEFAULT_FAR_DIS;
    float m_cameraDistance  = DEFAULT_CAMERA_DIS;
    bool m_ProjectionMatChanged= true;

    bool m_useTarget        = false;
    float m_sensitive       = 1.0f;
private:
    glm::mat4 m_projectionMat4 = glm::mat4(1.0f);
    glm::mat4 m_viewMat4 = glm::mat4(1.0f);
    void updateProjectionMat4();
    void updateViewMat4();
public:
    void enableControl(bool v) { m_control.enableControl(v); if (v) m_control.setCamera(this); }
    void enableTarget(bool v) { m_useTarget = v; m_ViwMatChanged = true;}
    bool isControlEnable() const { return m_control.isEnable(); }
    void processControl() { m_control.processActions(); }

    void update();
    inline Pipline::Window& getWindow() const { return *m_window; }
    inline void setWindow(Window* window) { m_window = window; }

    inline void setSensitive(float v) { m_sensitive = v; m_ViwMatChanged = true; }
    inline float getSensitive() const { return m_sensitive; }

    inline void setCameraPos(const glm::vec3& pos) 
    { 
        if (m_cameraPos == pos) return; 
        m_cameraPos = pos; 
        m_ViwMatChanged = true; 
    }
    inline void setCameraPos(const Vector3& pos) { setCameraPos(glm::vec3(pos.x, pos.y, pos.z)); }
    inline glm::vec3 getCameraPos() const { return m_cameraPos; }

    inline void setCameraFront(const glm::vec3& pos) { m_front = pos; m_ViwMatChanged = true; }
    inline void setCameraFront(const Vector3& pos) { m_front = glm::vec3(pos.x, pos.y, pos.z); m_ViwMatChanged = true; }
    inline glm::vec3 getCameraFront() const { return m_front; }

    inline void setCameraUp(const glm::vec3& pos) { m_up = pos; }
    inline glm::vec3 getCameraUp() const { return m_up; }

    inline float getCameraDistance() const { return m_cameraDistance; }
    inline void setCameraDistance(float dis) { m_cameraDistance = dis; m_ViwMatChanged = true; }
public:
    inline float getYaw() const { return m_yaw; }
    inline float getPitch() const { return m_pitch; }
    inline void setYaw(float y) { m_yaw = y; yawFilter(); }
    inline void setPitch(float p) { m_pitch = p; pitchFilter(); }

    inline void addYaw(float yaw) { m_yaw += yaw * m_sensitive; yawFilter(); }
    inline void addPitch(float pitch) { m_pitch += pitch * m_sensitive; pitchFilter(); }
    inline void addFov(float offset) { m_fov += offset * m_sensitive; fovFilter(); }
public:
    inline float getFov() const { return m_fov; }
    inline void setFov(float fov) { m_fov = fov; fovFilter(); }
    inline float getNearDistance() const { return m_nearDistance; }
    inline void setNearDistance(float dis) { m_nearDistance = dis; m_ProjectionMatChanged = true; }
    inline float getFarDistance() const { return m_farDistance; }
    inline void setFarDistance(float dis) { m_farDistance = dis; m_ProjectionMatChanged = true; }

    inline glm::vec3 getCameraDirection() const { return glm::normalize(-m_front);}
    inline glm::vec3 getCameraRight() const { return glm::normalize(glm::cross(m_up, -m_front)); }
    inline glm::vec3 getCameraFinalUp() const { return glm::cross(getCameraDirection(), getCameraRight()); }
private:
    float m_fovMax = 50.0f;
    float m_fovMin = 10.0f;
    
    float m_yawMax = 0.0f;
    float m_yawMin = 0.0f;

    float m_pitchMax = 0.0f;
    float m_pitchMin = 0.0f;

    void fovFilter();
    void yawFilter();
    void pitchFilter();
public:
    inline void setFovRange(float min, float max) { m_fovMin = min; m_fovMax = max; m_ProjectionMatChanged = true; }
    inline void setYawRange(float min, float max) { m_yawMin = min; m_yawMax = max; m_ViwMatChanged = true; }
    inline void setPitchRange(float min, float max) { m_pitchMin = min; m_pitchMax = max; m_ViwMatChanged = true; }
public:
    glm::mat4 getProjectionMat4() { updateProjectionMat4(); return m_projectionMat4; }
    glm::mat4 getViewMat4() { updateViewMat4(); return m_viewMat4; }
public:

    Camera() { m_up = glm::cross(getCameraDirection(), getCameraRight()); };
    Camera(Window* window) : m_window(window) { }
    inline bool isValid() const { m_window != nullptr; }
};

}