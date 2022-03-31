#pragma once
#include "resource/resourceBasic.h"

namespace Resource
{

class ShaderResource : public BaseResource
{
private:
    std::string m_name;
    std::string m_path;
public:
    const std::string& getName() const { return m_name; }
    const std::string& getPath() const { return m_path; }
    const char* getContent() const { return (char*)rawData().buffer(); }
    virtual bool loadFromPath(const char* path) override;
};

typedef tResourceRef<ShaderResource> ShaderRef;

}