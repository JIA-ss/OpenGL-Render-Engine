#include "Component.h"

COMPONENT_NAMESPACE_USING

std::map<size_t, std::map<size_t, std::string>> sComponent::ComponentDirevedClasses = std::map<size_t, std::map<size_t, std::string>>();
const size_t sComponent::ComponentId = registerComponentId("Component");
size_t sComponent::GetStaticComponentId()
{
    return ComponentId;
}

size_t sComponent::registerComponentId(const std::string& name)
{
    size_t id = std::hash<std::string>()(name);
    ComponentDirevedClasses[id] = std::map<size_t, std::string>();
    return id;
}


std::vector<std::pair<std::string, void*>> sComponent::get_properties()
{
    size_t componentId = get_componentId();
    std::vector<std::pair<std::string, void*>> res;
    for (auto&&[propId, propVal] : m_properties)
    {
        res.emplace_back(ComponentDirevedClasses[componentId][propId], propVal);
    }
    return res;
}

size_t sComponent::get_componentId() const { return ComponentId; }

bool sComponent::isType(const std::string& name)
{
    return get_componentId() == std::hash<std::string>()(name);
}