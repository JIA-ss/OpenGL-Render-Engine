#pragma once
#include "System.h"

class CustomSystem: public System
{
    SYSTEM_STATIC_TYPE(CustomSystem, System::Type::CustomSystem)
public:
    CustomSystem() : System(System::Type::CustomSystem) { }
    void Init() override;
    void Update() override;
};