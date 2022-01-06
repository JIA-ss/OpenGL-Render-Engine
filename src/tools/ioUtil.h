#ifndef _IO_UTIL_
#define _IO_UTIL_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

namespace Util
{

std::string readFile(const char* filePath)
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


}

#endif // _IO_UTIL_