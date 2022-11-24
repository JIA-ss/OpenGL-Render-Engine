#include "System.h"
#include "Tracy.hpp"
#include <iostream>

IMPLEMENT_SINGLETON(SystemManager)

System::Type System::GetType() const { return m_type; }




void SystemManager::Init()
{

    ZoneScopedN("SystemManager::Init");
    for (auto&[type, system] : m_systems)
    {
        std::cout << system->GetTypeStr() << " Init ..." << std::endl;
        system->Init();
    }
}

void SystemManager::Update()
{
    for (auto&[type, system] : m_systems)
    {
        system->Update();
    }
}

void SystemManager::UnInit()
{
    for (auto it = m_systems.begin(); it != m_systems.end(); it++)
    {
        std::cout << it->second->GetTypeStr() << " UnInit ..." << std::endl;
        it->second->UnInit();
        //delete it->second;
    }

    for (auto it = m_systems.begin(); it != m_systems.end(); it++)
    {
        std::cout << it->second->GetTypeStr() << " Closing ..." << std::endl;
        delete it->second;
    }
}

void SystemManager::AddSystem(System::Type type, System* system)
{
    assert(m_systems.find(type) == m_systems.end());
    m_systems[type] = system;
}

System* SystemManager::GetSystem(System::Type type)
{
    auto it = m_systems.find(type);
    if (it == m_systems.end())
        return nullptr;
    return it->second;
}