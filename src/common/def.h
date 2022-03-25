#ifndef _DEF__
#define _DEF__

#include "assert.h"
#define DECLARE_SINGLETON(T)    \
private:    \
    static T* _instance_; \
public:     \
    static void InitSingleTon() { assert(!_instance_); _instance_ = new T(); } \
    static T* Instance()    \
    {   \
        assert(_instance_ != nullptr);  \
        return _instance_;  \
    }

#define IMPLEMENT_SINGLETON(T)  \
    T* T::_instance_ = nullptr;






// COMPONENT DEFINITIONS

#define COMPONENT_NAMESPACE_BEGIN      \
    namespace Component {                  

#define COMPONENT_NAMESPACE_END        \
    }

#define COMPONENT_NAMESPACE_USING        \
    using namespace Component;


#define COMPONENT_DECLARE                                                                       \
public:                                                                                         \
    size_t get_componentId() const override { return ComponentId; }                             \
    static size_t GetStaticComponentId();                                                       \
    static const size_t ComponentId;

#define COMPONENT_IMPLEMENT(_CLASS_)                                                            \
const size_t _CLASS_::ComponentId = registerComponentId<_CLASS_>(#_CLASS_);                     \
size_t _CLASS_::GetStaticComponentId()                                                          \
{                                                                                               \
    return ComponentId;                                                                         \
}

#define COMPONENT_PROPERTY(_TYPE_, _NAME_, _VALUE_)                                             \
private:                                                                                        \
    _TYPE_ m_##_NAME_ = registerPropertyId<_TYPE_>(#_NAME_, _VALUE_, &m_##_NAME_);              \
public:                                                                                         \
    _TYPE_ get_##_NAME_() const { return m_##_NAME_; }                                          \
    void set_##_NAME_(_TYPE_& val, bool notify = true)                                          \
    {                                                                                           \
        if (m_##_NAME_ != val)                                                                  \
        {                                                                                       \
            m_##_NAME_ = val;                                                                   \
            if (notify)                                                                         \
            {                                                                                   \
                prop_variant v = m_properties[std::hash<std::string>()(#_NAME_)];               \
                notifyPropertyChanged(v);                                                       \
            }                                                                                   \
        }                                                                                       \
    }

#define COMPONENT_PROPERTY_WITHOUT_COMPARE(_TYPE_, _NAME_, _VALUE_)                             \
private:                                                                                        \
    _TYPE_ m_##_NAME_ = registerPropertyId<_TYPE_>(#_NAME_, _VALUE_, &m_##_NAME_);              \
public:                                                                                         \
    _TYPE_ get_##_NAME_() const { return m_##_NAME_; }                                          \
    void set_##_NAME_(_TYPE_& val, bool notify = true)                                          \
    {                                                                                           \
        m_##_NAME_ = val;                                                                       \
        if (notify)                                                                             \
        {                                                                                       \
            prop_variant v = m_properties[std::hash<std::string>()(#_NAME_)];                   \
            notifyPropertyChanged(v);                                                           \
        }                                                                                       \
    }

#define COMPONENT_PROPERTY_WITHOUT_SETTER_AND_GETTER(_TYPE_, _NAME_, _VALUE_)                               \
private:                                                                                                    \
    _TYPE_ m_##_NAME_ = registerPropertyId<_TYPE_>(#_NAME_, _VALUE_, &m_##_NAME_);                          


// ENTITY DEFINITIONS
#define ENTITY_NAMESPACE_BEGIN      \
    namespace Entity {                  

#define ENTITY_NAMESPACE_END        \
    }

#define ENTITY_NAMESPACE_USE        \
    using namespace Entity;

#define ENTITY_DECLARE                                                                          \
public:                                                                                         \
    size_t get_entityId() const override { return EntityId; }                                   \
    static size_t GetStaticEntityId();                                                          \
    static const size_t EntityId;

#define ENTITY_IMPLEMENT(_CLASS_)                                                               \
const size_t _CLASS_::EntityId = registerEntityId<_CLASS_>(#_CLASS_);                        \
size_t _CLASS_::GetStaticEntityId()                                                             \
{                                                                                               \
    return EntityId;                                                                            \
}



// Graphic
#define GRAPHIC_DECLARE                                                                         \
public:                                                                                         \
    size_t get_graphicId() const override { return GraphicId; }                                 \
    static size_t GetStaticGraphicId();                                                         \
    static const size_t GraphicId;

#define GRAPHIC_IMPLEMENT(_CLASS_)                                                              \
const size_t _CLASS_::GraphicId = registerGraphicId<_CLASS_>(#_CLASS_);                         \
size_t _CLASS_::GetStaticGraphicId()                                                            \
{                                                                                               \
    return GraphicId;                                                                           \
}

#endif