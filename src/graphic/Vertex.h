#pragma once
#include "GraphicCommonHeader.h"
#include <vector>

GRAPHIC_NAMESPACE_BEGIN

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

/*Predefined Vertex*/
    static const std::vector<Vertex> quad;
    static const std::vector<unsigned int> quadElement;

    static const std::vector<Vertex> boxQuad;
    static const std::vector<unsigned int> boxElementQuad;

    static const std::vector<Vertex> box;
    static const std::vector<unsigned int> boxElement;
};



GRAPHIC_NAMESPACE_END