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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

unsigned char* Util::loadTextureFromMemory(unsigned char* buffer, size_t bufferSize, int*width, int* height, int* channels, int other_param)
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