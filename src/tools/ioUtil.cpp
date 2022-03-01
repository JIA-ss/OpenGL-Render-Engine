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
        file = fopen(path, "ab");
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
}



unsigned char* Util::atlasPackData(std::vector<unsigned char*>& data, std::vector<std::vector<int>>& meta, unsigned int& size, std::vector<V4>& atlasMeta)
{
    auto start = std::chrono::system_clock::now();

    stbrp_context context;
    memset(&context, 0, sizeof(stbrp_context));


    unsigned int bufferSize = size * size * 4;
    unsigned char* buffer = new unsigned char[bufferSize];
    memset(buffer, 0, bufferSize);

    std::unique_ptr<stbrp_node[]> nodes(new stbrp_node[size]);
    std::unique_ptr<stbrp_rect[]> rects(new stbrp_rect[data.size()]);


    atlasMeta.resize(data.size());
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        auto& rect = rects[i];
        rect.id = i;
        rect.w = meta[i][0];
        rect.h = meta[i][1];
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "pack memset time: " << elapsed.count() * 1000 << " ms" << std::endl;

    start = std::chrono::system_clock::now();
    stbrp_init_target(&context, size, size, nodes.get(), size);
    end = std::chrono::system_clock::now();
    elapsed = end - start;
    std::cout << "pack init_target time: " << elapsed.count() * 1000 << " ms" << std::endl;

    start = std::chrono::system_clock::now();
    int result = stbrp_pack_rects(&context, rects.get(), data.size());
    end = std::chrono::system_clock::now();
    elapsed = end - start;
    std::cout << "pack pack_rects time: " << elapsed.count() * 1000 << " ms" << std::endl;


    if (result == 0)
        return buffer;

    start = std::chrono::system_clock::now();

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        auto textureRowSize = rects[i].w * 4;
        auto atlasRowSize = size * 4;
        int x = rects[i].x;
        int y = rects[i].y;
        int w = rects[i].w;
        int h = rects[i].h;
        atlasMeta[i] = {x,y,w,h};
        for (auto row = 0; row < h; ++row)
            memcpy(buffer + (row + y) * atlasRowSize + x * 4, data[i] + row * textureRowSize, textureRowSize);

    }

    end = std::chrono::system_clock::now();
    elapsed = end - start;
    std::cout << "pack fill buffer time: " << elapsed.count() * 1000 << " ms" << std::endl;


    start = std::chrono::system_clock::now();
    std::string outputPath = std::string(_RESOURCE_PATH_) + "/atlasTexture/atlas.png";
    // 写入图片
    //stbi_write_png(outputPath.c_str(), size, size, 4, buffer, 0);

    end = std::chrono::system_clock::now();
    elapsed = end - start;
    std::cout << "pack write image time: " << elapsed.count() * 1000 << " ms" << std::endl;
    return buffer;
}


unsigned char** Util::atlasUnpackData(unsigned char* atlasData, std::vector<V4>& meta, unsigned int& size)
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

bool Util::atlasPack(Resource::AtlasTextureResource& atlas)
{
    stbrp_context context;
    memset(&context, 0, sizeof(stbrp_context));

    auto nodeCount = atlas.getRawSize();
    auto width = nodeCount, height = nodeCount;
    std::unique_ptr<stbrp_node[]> nodes(new stbrp_node[nodeCount]);
    std::unique_ptr<stbrp_rect[]> rects(new stbrp_rect[atlas.m_cellCtxs.size()]);

    for (std::size_t i = 0; i < atlas.m_cellCtxs.size(); ++i)
    {
        auto& rect = rects[i];
        auto& ctx = atlas.m_cellCtxs[i];
        rect.id = i;
        rect.w = ctx.cell.w;
        rect.h = ctx.cell.h;
    }

    stbrp_init_target(&context, width, height, nodes.get(), nodeCount);
    int result = stbrp_pack_rects(&context, rects.get(), atlas.m_cellCtxs.size());

    for (std::size_t i = 0; i < atlas.m_cellCtxs.size(); ++i)
    {
        auto& rect = rects[i];
        auto& ctx = atlas.m_cellCtxs[i];

        ctx.packed = rect.was_packed > 0;
        ctx.cell.x = rect.x;
        ctx.cell.y = rect.y;
    }
    return result > 0;
}