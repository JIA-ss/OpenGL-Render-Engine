#pragma once
#include "GraphicCommonHeader.h"
#include "common/custom_rtti.h"
#include "common/def.h"

#include <vector>
#include <string>
#include <map>
GRAPHIC_NAMESPACE_BEGIN
using namespace custom_rtti;

class GraphicBase
{
protected:
    static std::map<size_t, graphic_meta>* GraphicDerivedClasses;
    static const size_t GraphicId;

    template<typename T>
    static size_t registerGraphicId(const std::string& name);
public:
    static size_t GetStaticGraphicId() { return GraphicId; }
public:
    virtual size_t get_graphicId() const { return GraphicId; }
    virtual void Free() = 0;
    GraphicBase() = default;
};

template<typename T>
size_t GraphicBase::registerGraphicId(const std::string& name)
{
    size_t id = std::hash<std::string>()(name);
    if (GraphicDerivedClasses == nullptr)
        GraphicDerivedClasses = new std::map<size_t, graphic_meta>();

    auto it = GraphicDerivedClasses->find(id);
    if (it != GraphicDerivedClasses->end())
        return id;

    graphic_meta meta{name, sizeof(T)};
    GraphicDerivedClasses->insert(std::make_pair(id,meta));
    return id;
}

GRAPHIC_NAMESPACE_END