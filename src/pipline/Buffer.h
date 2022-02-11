#pragma once
#include <vector>
#include "basicTypes.h"
#include <unordered_map>
#include "resource/types/TextureResource.h"
#include <share.h>
namespace Pipline
{
class Texture2DInfo
{
private:
    unsigned int m_textureId = 0;
    std::string m_textureName = "";
    bool m_enableMipMap = true;
    std::unordered_map<unsigned int, unsigned int> m_attribs;
    Resource::TextureRef m_texRef;
private:
    bool m_alreadyGenerateId = false;
    void freeTexture();
    void generateTextureId();
public:
    void prepare();
    void use() const;
    void setDefaultAttrib();
public:
    Texture2DInfo(const char* name, bool enableMipMap = true) : m_textureName(name), m_enableMipMap(enableMipMap) {}
    Texture2DInfo() = delete;
    ~Texture2DInfo() { }
    void changeTexture(const char* name);
    void addAttrib(unsigned int prop, unsigned int value);
};

class Buffer
{
private:
    unsigned int m_vao, m_vbo, m_ebo;
    std::vector<Texture2DInfo> m_textures;
    bool m_isReady = false;
private:
    std::shared_ptr<float[]> m_finalData = nullptr;
    size_t m_finalDataSize = 0;
    size_t m_finalDataCapacity = 0;
    bool m_dirtyFlag = false;
    std::vector<Vector3> m_vertexPos;
    std::vector<Color> m_vertexColor;
    std::vector<Vector3> m_vertexNormal;
    std::vector<Vector2> m_texCoord;
    std::vector<unsigned int> m_indices;
private:
    size_t dataAlignment();
    void combineVertexData();
public:
    void setVertexPos(float* data, size_t data_size);
    inline void setVertexPos(const std::vector<Vector3>& data) { m_vertexPos = data; m_dirtyFlag = true; }
    inline void setVertexPos(std::vector<Vector3>&& data) { m_vertexPos = std::move(data); m_dirtyFlag = true; }
    inline void addVertexPos(const Vector3& data) { m_vertexPos.emplace_back(data); m_dirtyFlag = true; }

    void setVertexNormal(float* data, size_t data_size);
    inline void setVertexNormal(const std::vector<Vector3>& data) { m_vertexNormal = data; m_dirtyFlag = true; }
    inline void setVertexNormal(std::vector<Vector3>&& data) { m_vertexNormal = std::move(data); m_dirtyFlag = true; }
    inline void addVertexNormal(const Vector3& data) { m_vertexNormal.emplace_back(data); m_dirtyFlag = true; }

    void setVertexColor(float* data, size_t data_size);
    inline void setVertexColor(const std::vector<Color>& data) { m_vertexColor = data; m_dirtyFlag = true;}
    inline void setVertexColor(std::vector<Color>&& data) { m_vertexColor = std::move(data); m_dirtyFlag = true; }
    inline void addVertexColor(const Color& data) { m_vertexColor.emplace_back(data); m_dirtyFlag = true; }

    void setTexCoord(float* data, size_t data_size);
    void setTexCoord(const std::vector<Vector2>& data) { m_texCoord = data; m_dirtyFlag = true; }
    inline void setTexCoord(std::vector<Vector2>&& data) { m_texCoord = std::move(data); m_dirtyFlag = true; }
    inline void addTexCoord(const Vector2& data) { m_texCoord.emplace_back(data); m_dirtyFlag = true; }

    inline void addTexture(const Texture2DInfo& tex) { m_textures.emplace_back(tex); m_dirtyFlag = true; }
    inline void addTexture(Texture2DInfo&& tex) { m_textures.emplace_back(tex); m_dirtyFlag = true; }
public:
    void addVertexData(const Vector3& pos, const Color& color);
    void addVertexData(const Vector3& pos, const Vector2& texCoord);
    void addVertexData(const Vector3& pos, const Color& color, const Vector2& texCoord);

    void setVertexData(const std::vector<Vector3>& pos, const std::vector<Color>& color);
    void setVertexData(const std::vector<Vector3>& pos, const std::vector<Vector2>& tex);
    void setVertexData(const std::vector<Vector3>& pos, const std::vector<Color>& color, const std::vector<Vector2>);
public:
    void prepare();
    void use() const;
    void draw(unsigned int start = 0, unsigned int num = 0) const;
    inline bool isReady() const { return m_isReady; }
public:
    void setIndices(unsigned int* data, size_t data_size);
    void setIndices(const std::vector<unsigned int>& data) { m_indices = data; }
    inline void setIndices(std::vector<unsigned int>&& data) { m_indices = std::move(data); }
public:
    Buffer() { }
    ~Buffer() { }
};


} // namespace Pipline
