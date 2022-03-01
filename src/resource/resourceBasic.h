#pragma once
#include <memory>
#include <string>
typedef unsigned char ubyte;
typedef signed char sbyte;

namespace Resource
{

enum eResourceType
{
    ResourceTypeBegin = 0,
    shader = 1,
    texture = 2,
    atlasTexture = 3,
    ResourceTypeEnd = 4
};

class ByteBuffer
{
private:
    ubyte* m_buffer = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;
    size_t m_capacityIncrement = 1024;
public:
    inline ByteBuffer(size_t IncCapacity = 1024) : m_capacityIncrement(IncCapacity) {}
    ~ByteBuffer() { clear(); }
    ByteBuffer(const ByteBuffer& other);
    ByteBuffer(ByteBuffer&& other) noexcept;

    ByteBuffer& operator = (const ByteBuffer& other);
    ByteBuffer& operator = (ByteBuffer&& other) noexcept;
public:
    void clear();
    void append(const void* src, size_t size);
    void reverse(size_t capacity);
    inline ubyte* buffer() const { return m_buffer; }
    inline size_t length() const { return m_size; }
    inline bool empty() const { return m_size == 0; }
};

class BaseResource
{
protected:
    bool m_isReady = false;
    ByteBuffer m_rawData;
public:
    BaseResource() { }
public:
    inline size_t getSize() const { return m_rawData.length(); }
    inline bool isReady() const { return m_isReady; }
    const ByteBuffer& rawData() const { return m_rawData; }
    virtual void loadFromBuffer(ByteBuffer&& buf) { m_rawData = std::move(buf); }
    virtual void loadFromBuffer(const ByteBuffer& buf) { m_rawData = buf; }
    virtual void loadFromPath(const char* path) {}
};


class sResourceRef
{
protected:
    std::shared_ptr<BaseResource> m_resource;
    inline std::shared_ptr<BaseResource> ptr() { return isNull() ? nullptr : m_resource; }
    eResourceType m_type;
public:
    static sResourceRef invalid;
public:
    inline bool isNull() const { return m_resource == nullptr || this == &invalid; }
    eResourceType getType() const { return m_type; }
public:
    sResourceRef(std::shared_ptr<BaseResource> res) { m_resource = res; }
    sResourceRef() { }
    sResourceRef(const sResourceRef& other);
    sResourceRef& operator = (const sResourceRef& other);
    inline bool operator == (const sResourceRef& other) { return m_resource == other.m_resource; }
    inline std::shared_ptr<BaseResource> operator -> () { return ptr(); }
};

template <class T>
class tResourceRef : public sResourceRef
{
public:
    tResourceRef() : sResourceRef() {}
    tResourceRef(const sResourceRef& other)
    {
        if (other.getType() == m_type)
        {
            sResourceRef& _this = (*this);
            _this = other;
        }
    }

    inline std::shared_ptr<T> get() const { return std::dynamic_pointer_cast<T>(m_resource); }
    inline std::shared_ptr<T> operator -> () { return get(); }
};

class ResourceManager;
class ResourceFactory
{
    friend ResourceManager;
private:
    template<class T>
    static tResourceRef<T> ImportResource(const char* path);

    static sResourceRef ImportResourceByType(const char* path, eResourceType type);
};

//extern template ShaderRef ResourceFactory::ImportResource<ShaderResource>(const char*);
}