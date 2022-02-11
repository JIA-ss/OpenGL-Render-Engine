#include "Buffer.h"
#include "common/headers.h"
#include <algorithm>
#include "resource/resourceManager.h"

using namespace Pipline; 

void Texture2DInfo::generateTextureId()
{
    if (m_alreadyGenerateId)
        return;
    m_alreadyGenerateId = true;

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    
    for (auto it : m_attribs)
    {
        glTexParameteri(GL_TEXTURE_2D, it.first, it.second);
    }
}

void Texture2DInfo::freeTexture()
{
    if (!m_alreadyGenerateId)
        return;
    m_alreadyGenerateId = false;
    glDeleteTextures(1, &m_textureId);
}

void Texture2DInfo::prepare()
{
    generateTextureId();
    m_texRef = Resource::ResourceManager::Instance()->GetResource(m_textureName.c_str(), Resource::eResourceType::texture);
    if (m_texRef.isNull())
    {
        std::cout << "Texture2DInfo::use::Failed! " << m_textureName << std::endl;
        return;
    }
    GLint imgType = m_texRef->getFormat();
    int width = m_texRef->getWidth();
    int height= m_texRef->getHeight();
    const ubyte* data = m_texRef->getTextureData();

    glTexImage2D(GL_TEXTURE_2D, 0, imgType, width, height, 0, imgType, GL_UNSIGNED_BYTE, data);
    if (m_enableMipMap)
        glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2DInfo::use() const
{
    assert(m_alreadyGenerateId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture2DInfo::changeTexture(const char* name)
{
    freeTexture();
    m_textureName = name;
}

void Texture2DInfo::addAttrib(unsigned int prop, unsigned int value)
{
    m_attribs[prop] = value;
    if (m_alreadyGenerateId)
    {
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexParameteri(GL_TEXTURE_2D, prop, value);
    }
}

void Texture2DInfo::setDefaultAttrib()
{
    addAttrib(GL_TEXTURE_WRAP_S, GL_REPEAT);
    addAttrib(GL_TEXTURE_WRAP_T, GL_REPEAT);
    addAttrib(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    addAttrib(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Buffer::addVertexData(const Vector3& pos, const Color& color)
{
    m_vertexPos.emplace_back(pos);
    m_vertexColor.emplace_back(color);
    m_dirtyFlag = true;
}
void Buffer::addVertexData(const Vector3& pos, const Vector2& texCoord)
{
    m_vertexPos.emplace_back(pos);
    m_texCoord.emplace_back(texCoord);
    m_dirtyFlag = true;
}
void Buffer::addVertexData(const Vector3& pos, const Color& color, const Vector2& texCoord)
{
    m_vertexPos.emplace_back(pos);
    m_vertexColor.emplace_back(color);
    m_texCoord.emplace_back(texCoord);
    m_dirtyFlag = true;
}

void Buffer::setVertexData(const std::vector<Vector3>& pos, const std::vector<Color>& color)
{
    m_vertexPos = pos;
    m_vertexColor = color;
    m_dirtyFlag = true;
}
void Buffer::setVertexData(const std::vector<Vector3>& pos, const std::vector<Vector2>& tex)
{
    m_vertexPos = pos;
    m_texCoord = tex;
    m_dirtyFlag = true;
}
void Buffer::setVertexData(const std::vector<Vector3>& pos, const std::vector<Color>& color, const std::vector<Vector2> tex)
{
    m_vertexPos = pos;
    m_vertexColor = color;
    m_texCoord = tex;
    m_dirtyFlag = true;
}
void Buffer::setVertexPos(float* data, size_t size)
{
    if (!data)
        return;
    m_vertexPos.resize((size / sizeof(float)) / 3);
    memcpy(m_vertexPos.data(), data, size);
    m_dirtyFlag = true;
}

void Buffer::setVertexNormal(float* data, size_t size)
{
    if (!data)  return;
    m_vertexNormal.resize((size/sizeof(float)) / 3);
    memcpy(m_vertexNormal.data(), data, size);
    m_dirtyFlag = true;
}

void Buffer::setVertexColor(float* data, size_t size)
{
    if (!data)
        return;
    m_vertexColor.resize((size / sizeof(float)) / 3);
    memcpy(m_vertexColor.data(), data, size);
    m_dirtyFlag = true;
}

void Buffer::setTexCoord(float* data, size_t size)
{
    if (!data)
        return;
    m_texCoord.resize((size/sizeof(float)) / 2);
    memcpy(m_texCoord.data(), data, size);
    m_dirtyFlag = true;
}

size_t Buffer::dataAlignment()
{
    size_t posNum = m_vertexPos.size();
    assert(posNum != 0);

    size_t normalNum = m_vertexNormal.size();
    size_t colNum = m_vertexColor.size();
    size_t texCoordNum = m_texCoord.size();

    if (posNum == 0)
    {
        m_vertexColor.clear();
        m_texCoord.clear();
        return 0;
    }

    size_t colJudge = colNum == 0 ? texCoordNum : colNum;
    size_t texJudge = texCoordNum == 0 ? colJudge : texCoordNum;
    size_t norMalJudge = normalNum == 0 ? texJudge : normalNum;

    size_t v1 = colJudge < texJudge ? colJudge : texJudge;
    v1 = v1 < norMalJudge ? v1 : norMalJudge;
    size_t validSize = v1 < posNum ? v1 : posNum;

    if (validSize == 0)
    {
        return posNum;
    }

    if (posNum > validSize)
        m_vertexPos.erase(m_vertexPos.begin() + validSize, m_vertexPos.end());
    if (colNum > validSize)
        m_vertexColor.erase(m_vertexColor.begin() + validSize, m_vertexColor.end());
    if (texCoordNum > validSize)
        m_texCoord.erase(m_texCoord.begin() + validSize, m_texCoord.end());
    if (normalNum > validSize)
        m_vertexNormal.erase(m_vertexNormal.begin() + validSize, m_vertexNormal.end());

    return validSize;
}
void Buffer::combineVertexData()
{
    if (!m_dirtyFlag)
        return;
    m_dirtyFlag = false;

    size_t vertexNum = dataAlignment();
    size_t newSize = 0;
    int strid = 3;
    bool hasColorData = !m_vertexColor.empty();
    bool hasTexCoordData = !m_texCoord.empty();
    bool hasNormalData = !m_vertexNormal.empty();
    if (hasColorData)
        strid += 3;
    if (hasTexCoordData)
        strid += 2;
    if (hasNormalData)
        strid += 3;
    newSize = vertexNum * strid;

    if (newSize > m_finalDataCapacity)
    {
        m_finalDataSize = newSize;
        m_finalDataCapacity = newSize;
        m_finalData = std::shared_ptr<float[]>(new float[newSize]());
    }

    size_t tmpIdx = 0;
    for (size_t row = 0; row < vertexNum; ++row)
    {
        for (int i = 0; i < 3; i++)
            m_finalData[row * strid + i] = m_vertexPos[row][i];
        tmpIdx = 3;
        if (hasColorData)
        {
            for (int i = 0; i < 3; i++)
                m_finalData[row * strid + i + tmpIdx] = m_vertexColor[row][i];
            tmpIdx += 3;
        }
        if (hasTexCoordData)
        {
            for (int i = 0; i < 2; i++)
                m_finalData[row * strid + i + tmpIdx] = m_texCoord[row][i];
            tmpIdx += 2;
        }
        if (hasNormalData)
        {
            for (int i = 0; i < 3; i++)
                m_finalData[row * strid + i + tmpIdx] = m_vertexNormal[row][i];
            tmpIdx += 3;
        }
    }

    m_finalDataSize = newSize;
}

void Buffer::prepare()
{
    combineVertexData();

    //for (int i = 0; i < m_finalDataSize; i++)
    //{
    //    std::cout << m_finalData[i] << '\t';
    //    if ((i+1) % 5 == 0)
    //        std::cout << std::endl;
    //}

    if (m_finalDataSize == 0)
    {
        m_isReady = false;
        return;
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_finalDataSize * sizeof(float), m_finalData.get(), GL_STATIC_DRAW);

    if (!m_indices.empty())
    {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
    }
    /*index：第几个属性，从0开始取，0，1，2，顺序自己定义，例如顶点位置，纹理，法线
    这里只有顶点位置，也只能讨论顶点位置，所以为0
    size：一个顶点所有数据的个数，这里每个顶点又两个浮点数属性值，所以是2
    type：顶点描述数据的类型，这里position数组中的数据全部为float，所以是GL_FLOAT
    normalized：是否需要显卡帮忙把数据归一化到-1到+1区间，这里不需要，所以设置GL_FALSE
    stride：一个顶点占有的总的字节数，这里为两个float，所以是sizeof(float)*2
    pointer：当前指针指向的vertex内部的偏离字节数，可以唯一的标识顶点某个属性的偏移量
    */

    int attribIndex = 0;
    int strid = 3;
    bool hasColorAttrib = !m_vertexColor.empty();
    bool hasTexCoordAttrib = !m_texCoord.empty();
    bool hasNormalAttrib = !m_vertexNormal.empty();
    if (hasColorAttrib)
        strid += 3;
    if (hasTexCoordAttrib)
        strid += 2;
    if (hasNormalAttrib)
        strid += 3;
    
    int idx = 0;
    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strid * sizeof(float), (void*)idx);
    glEnableVertexAttribArray(attribIndex++);
    idx += 3;

    if (hasColorAttrib)
    {
        //颜色属性
        glVertexAttribPointer(attribIndex,3,GL_FLOAT, GL_FALSE, strid*sizeof(float), (void*)(idx*sizeof(float)));
        glEnableVertexAttribArray(attribIndex++);
        idx += 3;
    }

    if (hasTexCoordAttrib)
    {
        //uv属性
        glVertexAttribPointer(attribIndex,2,GL_FLOAT,GL_FALSE, strid*sizeof(float), (void*)(idx*sizeof(float)));
        glEnableVertexAttribArray(attribIndex++);
        idx += 2;
    }

    if (hasNormalAttrib)
    {
        //normal属性
        glVertexAttribPointer(attribIndex,3,GL_FLOAT,GL_FALSE, strid*sizeof(float), (void*)(idx*sizeof(float)));
        glEnableVertexAttribArray(attribIndex++);
        idx += 3;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    for (size_t i = 0; i < m_textures.size(); i++)
    {
        m_textures[i].prepare();
    }
    m_isReady = true;
}

void Buffer::use() const
{
    assert(m_isReady);
    for (size_t i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        m_textures[i].use();
    }
    glBindVertexArray(m_vao);
}

void Buffer::draw(unsigned int start, unsigned int num) const
{
    assert(m_isReady);
    if (m_indices.empty())
    {
        num = num == 0 ? m_vertexPos.size() : num;
        glDrawArrays(GL_TRIANGLES, start, num);
    }
    else
    {
        num = num == 0 ? m_indices.size() : num;
        glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, 0);
    }
}