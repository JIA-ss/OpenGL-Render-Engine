#pragma once

#include <string>
#include <map>

namespace custom_rtti
{

struct common_meta
{
    std::string name;
    size_t size;
    common_meta() = default;
};

struct common_variant
{
    common_meta meta;
    void* ptr;
    common_variant() = default;
};

template<typename T>
struct common_meta_container
{
    std::string name;
    size_t size;
    std::map<size_t, T> props = { };
    common_meta_container<T>() = default;
};

template<typename T>
struct common_variant_container
{
    common_meta_container<T> meta;
    void* ptr;
    common_variant_container<T>() = default;
};

typedef common_meta                             graphic_meta;
typedef common_meta                             prop_meta;
typedef common_meta_container<prop_meta>        comp_meta;
typedef common_meta_container<comp_meta>        entity_meta;

typedef common_variant                          graphic_variant;
typedef common_variant                          prop_variant;
typedef common_variant_container<prop_meta>     comp_variant;
typedef common_variant_container<comp_meta>    entity_variant;

}