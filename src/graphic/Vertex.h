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

struct Vertex2D
{
    glm::vec2 position;
    glm::vec2 texCoord;

    static const std::vector<Vertex2D> quad2D;
    static const std::vector<unsigned int> quad2DElement;
};

struct VertexStream
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    VertexStream() = default;

    VertexStream(const std::vector<Vertex>& vers, const std::vector<unsigned int>& inds) : 
        vertices(vers), indices(inds) { }

    static const VertexStream quad;
    static const VertexStream boxQuad;
    static const VertexStream box;
};



GRAPHIC_NAMESPACE_END