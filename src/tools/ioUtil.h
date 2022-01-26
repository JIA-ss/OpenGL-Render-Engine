#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace Util
{

std::string readFile(const char* filePath);

void readFile(const char* path, std::vector<unsigned char>& dst);

void readFile(const char* path, std::vector<char>& dst);

char* readFile_Effective(const char* path);

char* readFile_Native(const char* path, size_t& size, bool isText = true);

bool writeFile_Native(const char* path, const char* src, int sz);
}
