#pragma once

#include <string>
#include <map>

struct prop_meta
{
    std::string name;
    size_t size;
    prop_meta() = default;
};
struct prop_variant
{
    prop_meta meta;
    void* ptr;
    prop_variant() = default;
};


struct comp_meta
{
    std::string name;
    size_t size;
    std::map<size_t, prop_meta> props = { };
    comp_meta() = default;
};
struct comp_variant
{
    comp_meta meta;
    void* ptr;
    comp_variant() = default;
};


struct entity_meta
{
    std::string name;
    size_t size;
    std::map<size_t, comp_meta> comps = { };
    entity_meta() = default;
};
struct entity_variant
{
    entity_meta meta;
    void* ptr;
    entity_variant() = default;
};