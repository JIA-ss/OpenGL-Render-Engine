#include "ioUtil.h"
#include "stdlib.h"
#include <nlohmann/json.hpp>

#if defined(_MSC_VER)
    #include <direct.h>
    #define GetCurrentDir _getcwd
#elif defined(__unix__)
    #include <unistd.h>
    #define GetCurrentDir getcwd
#else
#endif


#define STBRP_STATIC
#define STB_RECT_PACK_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_rect_pack.h>
#include <stb_image.h>
#include <stb_image_write.h>

#ifndef _RESOURCE_PATH_
    #define _RESOURCE_PATH_ "[CMAKE ERROR] [PLEASE SET _RESOURCE_PATH_ MACRO]"
#endif

std::string Util::readFile(const char* filePath)
{
    std::ifstream istream(filePath, std::ios::binary);

    if (!istream.is_open())
    {
        std::cout << "Util::readFile" << filePath << "Failed" << std::endl;
        return "";
    }

    std::string res;
    res.assign(std::istreambuf_iterator<char>(istream), std::istreambuf_iterator<char>());
    return res;
/*
    //or stringstream
    std::stringstream fileContent;
    fileContent << istream.rdbuf();
    istream.close();
    res = fileContent.str();
    return res;
*/
}

void Util::readFile(const char* path, std::vector<unsigned char>& dst)
{
    std::ifstream istream(path, std::ios::binary);
    if (!istream.is_open())
    {
        std::cout << "Util::readFile" << path << "Failed" << std::endl;
        return ;
    }
    dst.clear();
    dst.assign(std::istreambuf_iterator<char>(istream), std::istreambuf_iterator<char>());
    dst.push_back('\0');
}

void Util::readFile(const char* path, std::vector<char>& dst)
{
    std::ifstream istream(path, std::ios::binary);
    if (!istream.is_open())
    {
        std::cout << "Util::readFile" << path << "Failed" << std::endl;
        return ;
    }
    dst.clear();
    dst.assign(std::istreambuf_iterator<char>(istream), std::istreambuf_iterator<char>());
    dst.push_back('\0');
}


char* Util::readFile_Effective(const char* path)
{
    std::ifstream istream(path, std::ios::binary);
    istream.seekg(0, std::ios::end);
    int length = istream.tellg();

    //std::cout << length << std::endl;
    istream.seekg(0, std::ios::beg);


    char* buffer = new char [length + 1];
    istream.read(buffer, length);
    istream.close();
    buffer[length] = '\0';
    return buffer;
}

char* Util::readFile_Native(const char* path, size_t& size, bool isText)
{
    FILE* file = fopen(path, "rb+");
    if (file == nullptr)
    {
        //fclose(file);
        std::cout << "Read File Failed: " << path << strerror(errno) << std::endl;
        size = 0;
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    //std::cout << len << std::endl;
    fseek(file, 0, SEEK_SET);

    if (len == 0)
    {
        size = len;
        fclose(file);
        return nullptr;
    }


    unsigned char* buf = nullptr;
    if (isText)
        buf = new unsigned char[len + 1]();
    else
        buf = new unsigned char[len]();
    

    size_t res = fread(buf, 1, len, file);
    if (res < len)
    {
        std::cout << "Read File Failed: " << path << std::endl;
        delete[] buf;
        size = 0;
        fclose(file);
        return nullptr; 
    }
    size = len;
    if (isText) 
    {
        buf[size++] = '\0';
    }

    fclose(file);
    return (char*)buf;
}

bool Util::writeFile_Native(const char* path, const char* src, int sz)
{
    struct stat stat_buf;
    errno = 0;
    stat(path, &stat_buf);
    FILE* file = nullptr;
    if (errno != 0)
    {
        if (errno == ENOENT)
            std::cout << "file not exist\n will create a new file" << std::endl;
        file = fopen(path, "wb");
    }
    else
    {
        //file = fopen(path, "ab");

        // cover original content
        file = fopen(path, "w+b");
    }

    if (file == nullptr)
    {
        fclose(file);
        return false;
    }

    size_t res = fwrite(src, 1, sz, file);
    if (res != sz)
    {
        fclose(file);
        std::cout << "failed to write" << std::endl;
        return false; 
    }
    fclose(file);
    return true;
}

std::filesystem::path Util::getSrcPath()
{
    char buf[500];
    GetCurrentDir(buf, 500);
    std::filesystem::path curPath(buf);
    std::cout << _pgmptr << std::endl;
    return curPath.parent_path();
}

std::filesystem::path Util::getResourcePath()
{
    return std::filesystem::path(_RESOURCE_PATH_);
}

unsigned char* Util::loadTextureFromFile(const char* path, int* width, int* height, int* channels, int other_param)
{
    return stbi_load(path, width, height, channels, other_param);
}

unsigned char* Util::loadTextureFromMemory(const unsigned char* buffer, size_t bufferSize, int*width, int* height, int* channels, int other_param)
{
    return stbi_load_from_memory(buffer, bufferSize, width, height, channels, other_param);
}

void Util::freeTextureBuffer(void* buffer)
{
    stbi_image_free(buffer);
}

void Util::testJson()
{
    char* content = nullptr;
    size_t size = 0;
    content = readFile_Native((std::string(_RESOURCE_PATH_) + "/configs/test.config").c_str(), size);
    if (content == nullptr)
    {
        std::cout << "testJson Failed (test.config not exist)" << std::endl;
        return;
    }
    nlohmann::json configArgs;
    configArgs = nlohmann::json::parse(content);
    for (auto itor = configArgs.items().begin(); itor != configArgs.items().end(); ++itor)
    {
        std::string curKey = itor.key();
        std::string value = itor.value().get<std::string>();
        std::cout << curKey << " " << value << std::endl;
    }

    //std::cout << content << std::endl;
    JsonWrite();
    JsonRead();
}

void Util::JsonWrite()
{
    nlohmann::json j;
    nlohmann::json container;
    for (int i = 0; i < 5; i++)
    {
        nlohmann::json element;
        std::string idx = "element" + std::to_string(i);
        element[idx] = i;
        container.push_back(element);
    }
    j["container"] = container;

    j["raw_key"] = "raw_value";

    std::string content = j.dump();
    writeFile_Native((std::string(_RESOURCE_PATH_) + "/configs/jsonRW.config").c_str(), content.data(), content.size());
}
void Util::JsonRead()
{
    size_t size;
    char* content = readFile_Native((std::string(_RESOURCE_PATH_) + "/configs/jsonRW.config").c_str(), size, true);
    std::cout << content << std::endl;
    nlohmann::json j = nlohmann::json::parse(content);
    std::string raw_value = j["raw_key"].get<std::string>();
    std::cout << raw_value;
}

bool Util::atlasCanPack(std::vector<Resource::sAtlasCell>& meta, unsigned int size, std::vector<Resource::sAtlasCell>& rects)
{

    stbrp_context context;
    memset(&context, 0, sizeof(stbrp_context));

    std::unique_ptr<stbrp_node[]> nodes(new stbrp_node[size]);
    std::unique_ptr<stbrp_rect[]> stb_rects(new stbrp_rect[meta.size()]);


    rects.resize(meta.size());
    for (std::size_t i = 0; i < meta.size(); ++i)
    {
        auto& rect = stb_rects[i];
        rect.id = i;
        rect.w = meta[i].w;
        rect.h = meta[i].h;
    }



    stbrp_init_target(&context, size, size, nodes.get(), size);


    int result = stbrp_pack_rects(&context, stb_rects.get(), meta.size());

    if (result <= 0)
        return false;

    for (std::size_t i = 0; i < meta.size(); ++i)
    {
        auto textureRowSize = stb_rects[i].w * 4;
        auto atlasRowSize = size * 4;
        unsigned int x = stb_rects[i].x;
        unsigned int y = stb_rects[i].y;
        unsigned int w = stb_rects[i].w;
        unsigned int h = stb_rects[i].h;
        rects[i] = {"", "", (unsigned int)i,x,y,w,h,(unsigned int)stb_rects[i].was_packed};
    }

    return true;
}

unsigned char* Util::packAtlasWithMeta(const std::vector<Resource::sAtlasCell>& rects, unsigned int size, std::vector<const unsigned char*>& data)
{
    
    unsigned int bufferSize = size * size * 4;
    unsigned char* buffer = new unsigned char[bufferSize];
    memset(buffer, 0, bufferSize);

    
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        auto textureRowSize = rects[i].w * 4;
        auto atlasRowSize = size * 4;
        int x = rects[i].x;
        int y = rects[i].y;
        int w = rects[i].w;
        int h = rects[i].h;
        for (auto row = 0; row < h; ++row)
            memcpy(buffer + (row + y) * atlasRowSize + x * 4, data[i] + row * textureRowSize, textureRowSize);
    }

    return buffer;
}

unsigned char* Util::atlasPackData(std::vector<const unsigned char*>& data, std::vector<Resource::sAtlasCell>& meta, unsigned int size, std::vector<Resource::sAtlasCell>& atlasMeta)
{
    if (atlasCanPack(meta,size, atlasMeta))
        return packAtlasWithMeta(atlasMeta, size, data);
    return nullptr;
}


unsigned char** Util::atlasUnpackData(unsigned char* atlasData, std::vector<Resource::sAtlasCell>& meta, unsigned int& size)
{
    unsigned char** res = new unsigned char*[meta.size()];
    for (int i = 0; i < meta.size(); i++)
    {
        auto textureRowSize = meta[i].w * 4;
        auto atlasRowSize = size * 4;
        res[i] = new unsigned char[meta[i].w * meta[i].h * 4];

        for (auto row = 0; row < meta[i].h; ++row)
            memcpy(res[i] + row * textureRowSize, atlasData + (row + meta[i].y) * atlasRowSize + meta[i].x * 4, textureRowSize);
        
        
        char curIdx = '0' + i;
        std::string outputPath = std::string(_RESOURCE_PATH_) + "/atlasTexture/atlasUnpack" + curIdx + ".png";
        // 写入图片
        //stbi_write_png(outputPath.c_str(), meta[i].w, meta[i].h, 4, res[i], 0);
    }
    return res;
}

unsigned char* Util::addBorderForTexture(const unsigned char* data, int width, int height, int border)
{
    int newHeight = height + border * 2;
    int newWidth = width + border * 2;
    unsigned char* newTexData = new unsigned char[newWidth * newHeight * 4];
    memset(newTexData, 255, newHeight * newWidth * 4);

    for (auto row = border; row < height + border; row++)
    {
        memcpy(newTexData + row * newWidth * 4 + border * 4, data + (row - border) * width * 4, width * 4);
    }

    return newTexData;
}

void Util::writeTextureToPng(const std::string& path, int width, int height, const unsigned char* data)
{
    stbi_write_png(path.c_str(), width, height, 4, data, 0);
}