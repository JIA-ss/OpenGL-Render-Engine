#include "CustomSystem.h"
#include <iostream>
#include <Tracy.hpp>
void CustomSystem::Init()
{
    ZoneScopedN("CustomSystem::Init");
}

void CustomSystem::Update()
{
    ZoneScopedN("CustomSystem::Update");
    std::cout << "CustomSystem Update" << std::endl;
}