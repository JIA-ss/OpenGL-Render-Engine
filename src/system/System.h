#pragma once
#include "common/def.h"

#include <string>
#include <map>
#include <Tracy.hpp>

#define SYSTEM_STATIC_TYPE(_CLASS_, _TYPE_)                                             \
public:                                                                                 \
    static System::Type GetStaticType() { return _TYPE_; }                              \
    static std::string GetStaticTypeStr() { return #_TYPE_; }                           \
    static _CLASS_* Get() {                                                             \
        static _CLASS_* system = nullptr;                                               \
        if (system == nullptr)                                                          \
            system = SystemManager::Instance()->GetSystem<_CLASS_>();                   \
        return system;                                                                  \
    }   \
    std::string GetTypeStr() override { return GetStaticTypeStr(); }        


class SystemManager;
class System
{
    friend class SystemManager;
public:
    enum Type
    {
        Start = 0,
        WindowSystem,
        InputSystem,
        FileWatcherSystem,
        ResourceSystem,
        EntitySystem,
        UISystem,
        RenderSystem,

        CustomSystem,
        End
    };
public:
    System(Type type) : m_type(type) { }
    virtual void Init() { };
    virtual void Update() { };
    virtual void UnInit() { };
    virtual std::string GetTypeStr() = 0;
    Type GetType() const;
protected:
    Type m_type;
};

class SystemManager
{
    DECLARE_SINGLETON(SystemManager)
public:
    void Init();
    void Update();
    void UnInit();

    void AddSystem(System::Type type, System* system);
    System* GetSystem(System::Type type);

    template<typename T>
    T* AddSystem();
    template<typename T>
    T* GetSystem();
private:
    std::map<System::Type, System*> m_systems;
};


template<typename T>
T* SystemManager::GetSystem()
{
    return static_cast<T*>(GetSystem(T::GetStaticType()));
}

template<typename T>
T* SystemManager::AddSystem()
{
    T* system = new T();
    AddSystem(T::GetStaticType(), system);
    return system;
}