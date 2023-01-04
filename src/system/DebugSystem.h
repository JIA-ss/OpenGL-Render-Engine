#pragma once
#include "System.h"

class DebugSystem : public System
{
    SYSTEM_STATIC_TYPE(DebugSystem, System::Type::DebugSystem)
public:
    DebugSystem() : System(System::Type::DebugSystem) { }
    void Init() override;
public:
    void DrawGUI();
private:
    void DrawSceneHierarchy();
    void DrawResourceHierarchy();
};