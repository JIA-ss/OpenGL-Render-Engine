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

unsigned char* atlasPackData(std::vector<const unsigned char*>& data, std::vector<Resource::sAtlasCell>& meta, unsigned int size, std::vector<Resource::sAtlasCell>& rects);
unsigned char** atlasUnpackData(unsigned char* atlasData, std::vector<Resource::sAtlasCell>& meta, unsigned int& size);

bool atlasCanPack(std::vector<Resource::sAtlasCell>& meta, unsigned int size, std::vector<Resource::sAtlasCell>& rects);
unsigned char* packAtlasWithMeta(const std::vector<Resource::sAtlasCell>& rects, unsigned int size, std::vector<const unsigned char*>& data);
unsigned char* addBorderForTexture(const unsigned char* data, int width, int height, int border);

void writeTextureToPng(const std::string& path, int width, int height, const unsigned char* data);

void testJson();
void JsonWrite();
void JsonRead();

}
