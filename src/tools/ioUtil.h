#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "resource/types/AtlasTextureResource.h"
namespace Util
{
struct V4 
{
    int x,y,w,h;
};

std::string readFile(const char* filePath);

void readFile(const char* path, std::vector<unsigned char>& dst);

void readFile(const char* path, std::vector<char>& dst);

char* readFile_Effective(const char* path);

char* readFile_Native(const char* path, size_t& size, bool isText = true);

bool writeFile_Native(const char* path, const char* src, int sz);

std::filesystem::path getSrcPath();

std::filesystem::path getResourcePath();

unsigned char* loadTextureFromFile(const char* path, int* width, int* height, int* channels, int other_param);

unsigned char* loadTextureFromMemory(const unsigned char* buffer, size_t bufferSize, int*width, int* height, int* channels, int other_param);

void freeTextureBuffer(void* buffer);

unsigned char* atlasPackData(std::vector<unsigned char*>& data, std::vector<std::vector<int>>& meta, unsigned int& size, std::vector<V4>& rects);
unsigned char** atlasUnpackData(unsigned char* atlasData, std::vector<V4>& meta, unsigned int& size);
bool atlasPack(Resource::AtlasTextureResource& atlas);


void testJson();


}
