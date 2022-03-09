#include "Vertex.h"

GRAPHIC_NAMESPACE_USING


const std::vector<Vertex> Vertex::quad = {
    {glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)},
    {glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)},
    {glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)},
    {glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0)},
};

const std::vector<unsigned int> Vertex::quadElement = {
    2,3,0,
    0,1,2
};

#define oneThird (1 / 3.0)
const std::vector<Vertex> Vertex::boxQuad = {
    {
        {glm::vec3(0.5, 0.5, 0.5), glm::vec2(0.5, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0.25, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0.25, 1.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(0.5, 0.5, -0.5), glm::vec2(0.5, 1.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},

        {glm::vec3(0.5, 0.5, 0.5), glm::vec2(0.5, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(0.5, -0.5, 0.5), glm::vec2(0.5, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0.25, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0.25, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},

        {glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0.25, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0.0, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0.25, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},

        {glm::vec3(0.5, -0.5, 0.5), glm::vec2(0.5, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0.25, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0.25, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(0.5, -0.5, -0.5), glm::vec2(0.5, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},

        {glm::vec3(0.5, 0.5, 0.5), glm::vec2(0.5, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(0.5, -0.5, 0.5), glm::vec2(0.5, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(0.5, -0.5, -0.5), glm::vec2(0.75, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(0.5, 0.5, -0.5), glm::vec2(0.75, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},

        {glm::vec3(0.5, 0.5, -0.5), glm::vec2(0.75, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(0.5, -0.5, -0.5), glm::vec2(0.75, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, -0.5, -0.5), glm::vec2(1, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
        {glm::vec3(-0.5, 0.5, -0.5), glm::vec2(1, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
    }};

const std::vector<Vertex> Vertex::box = {
    {glm::vec3(0.5, 0.5, 0.5), glm::vec2(0.5, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
    {glm::vec3(-0.5, 0.5, 0.5), glm::vec2(0.25, 2 * oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
    {glm::vec3(-0.5, 0.5, -0.5), glm::vec2(0.25, 1.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
    {glm::vec3(0.5, 0.5, -0.5), glm::vec2(0.5, 1.0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
    {glm::vec3(0.5, -0.5, 0.5), glm::vec2(0.5, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
    {glm::vec3(-0.5, -0.5, 0.5), glm::vec2(0.25, oneThird), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
    {glm::vec3(-0.5, -0.5, -0.5), glm::vec2(0.25, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
    {glm::vec3(0.5, -0.5, -0.5), glm::vec2(0.5, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)},
};

const std::vector<unsigned int> Vertex::boxElementQuad = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    8, 9, 10, 11,
    12, 13, 14, 15,
    16, 17, 18, 19,
    20, 21, 22, 23};

const std::vector<unsigned int> Vertex::boxElement = {
    0, 1, 2, 0, 2, 3,
    0, 4, 1, 1, 4, 5,
    4, 5, 6, 4, 6, 7,
    3, 7, 2, 7, 6, 2,
    2, 1, 0, 0, 1, 5,
    0, 3, 7, 0, 7, 4};