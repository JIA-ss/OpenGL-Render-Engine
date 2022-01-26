#include "ioUtil.h"


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


    char* buf = nullptr;
    if (isText)
        buf = new char[len + 1]();
    else
        buf = new char[len]();
    

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
    return buf;
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