#pragma once
#include "pipline/basicTypes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Pipline
{

const glm::vec3 DEFAULT_CAMERA_POS    = glm::vec3(0.0f, 0.0f, -3.0f);
const glm::vec3 DEFAULT_CAMERA_UP     = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_CAMERA_FRONT  = glm::vec3(0.0f, 0.0f, -1.0f);
const float     DEFAULT_FOV           = 45.0f;
const float     DEFAULT_NEAR_DIS      = 0.1f;
const float     DEFAULT_FAR_DIS       = 100.f;
const float     DEFAULT_CAMERA_DIS    = 3.0f;

class Window;
class Camera
{
private:
    Window* m_window        = nullptr;
    glm::vec3 m_cameraPos   = DEFAULT_CAMERA_POS;
    glm::vec3 m_up          = DEFAULT_CAMERA_UP;
    glm::vec3 m_front       = DEFAULT_CAMERA_FRONT;

    float m_yaw             = 0.0f;
    float m_pitch           = 0.0f;
    bool m_ViwMatChanged  = true;

    float m_fov             = DEFAULT_FOV;
    float m_nearDistance    = DEFAULT_NEAR_DIS;
    float m_farDistance     = DEFAULT_FAR_DIS;
    float m_cameraDistance  = DEFAULT_CAMERA_DIS;
    bool m_ProjectionMatChanged= true;

    float m_sensitive       = 1.0f;
private:
    glm::mat4 m_projectionMat4 = glm::mat4(1.0f);
    glm::mat4 m_viewMat4 = glm::mat4(1.0f);
    void updateProjectionMat4();
    void updateViewMat4();
public:
    inline Pipline::Window& getWindow() const { return *m_window; }
    inline void setWindow(Window* window) { m_window = window; }

    inline void setSensitive(float v) { m_sensitive = v; m_ViwMatChanged = true; }
    inline float getSensitive() const { return m_sensitive; }

    inline void setCameraPos(const glm::vec3& pos) { m_cameraPos = pos; m_ViwMatChanged = true; }
    inline void setCameraPos(const Vector3& pos) { m_cameraPos = glm::vec3(pos.x, pos.y, pos.z); m_ViwMatChanged = true; }
    inline glm::vec3 getCameraPos() const { return m_cameraPos; }/*  */

    inline void setCameraFront(const glm::vec3& pos) { m_front = pos; }
    inline void setCameraFront(const Vector3& pos) { m_front = glm::vec3(pos.x, pos.y, pos.z); }
    inline glm::vec3 getCameraFront() const { return m_front; }

    inline float getCameraDistance() const { return m_cameraDistance; }
    inline void setCameraDistance(float dis) { m_cameraDistance = dis; m_ViwMatChanged = true; }
public:
    inline float getYaw() const { return m_yaw; }
    inline float getPitch() const { return m_pitch; }
    inline void setYaw(float y) { m_yaw = y; yawFilter(); }
    inline void setPitch(float p) { m_pitch = p; pitchFilter(); }

    inline void addYaw(float yaw) { m_yaw += yaw * m_sensitive; yawFilter(); }
    inline void addPitch(float pitch) { m_pitch += pitch * m_sensitive; pitchFilter(); }

public:
    inline float getFov() const { return m_fov; }
    inline void setFov(float fov) { m_fov = fov; fovFilter(); }
    inline float getNearDistance() const { return m_nearDistance; }
    inline void setNearDistance(float dis) { m_nearDistance = dis; m_ProjectionMatChanged = true; }
    inline float getFarDistance() const { return m_farDistance; }
    inline void setFarDistance(float dis) { m_farDistance = dis; m_ProjectionMatChanged = true; }

private:
    float m_fovMax = 0.0f;
    float m_fovMin = 0.0f;
    
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

    Camera() = default;
    Camera(Window* window) : m_window(window) { }
    inline bool isValid() const { m_window != nullptr; }
};


}