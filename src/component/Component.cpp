#include "Component.h"
#include <iostream>
COMPONENT_NAMESPACE_USING

std::map<size_t, comp_meta> sComponent::ComponentDirevedClasses = std::map<size_t, comp_meta>();
const size_t sComponent::ComponentId = registerComponentId<sComponent>("Component");
size_t sComponent::GetStaticComponentId()
{
    return ComponentId;
}

void* sComponent::get_property(size_t prop_id)
{
    auto it = m_properties.find(prop_id);
    if (it == m_properties.end())
        return nullptr;
    return it->second.ptr;
}

std::map<size_t, prop_variant> sComponent::get_properties()
{
    return m_properties;
}

size_t sComponent::get_componentId() const { return ComponentId; }

bool sComponent::isType(const std::string& name)
{
    return get_componentId() == std::hash<std::string>()(name);
}

sComponent* sComponent::Clone()
{
    size_t compId = get_componentId();
    comp_meta compMeta = ComponentDirevedClasses[compId];
    void* newComp = new char[compMeta.size];
    memcpy(newComp, this, compMeta.size);

    sComponent* comp = static_cast<sComponent*>(newComp);
    comp->m_entity = nullptr;
    auto&& props = get_properties();
    for (auto&&[propId, propVariant] : props)
    {
        void* newProp = comp->get_property(propId);
        if (newProp != nullptr)
        {
            memcpy(newProp, propVariant.ptr, propVariant.meta.size);
        }
        else
        {
            assert(false);
        }
    }
    return comp;
}

void sComponent::notifyPropertyChanged(const prop_variant& prop)
{
    comp_meta meta = ComponentDirevedClasses[get_componentId()];
    std::cout << "[Component Value Changed] ";
    std::cout << meta.name << "::" << prop.meta.name << std::endl;
}

void sComponent::SetActive(bool v)
{
    if (v != m_enable)
    {
        m_enable = v;
        m_enable ? OnEnable() : OnDisable();
    }
}

bool sComponent::IsActive()
{
    return m_enable;
}