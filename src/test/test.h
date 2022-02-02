#pragma once

#include "tools/ioUtil.h"
#include "pipline/window.h"
#include "pipline/Shader.h"
using namespace Pipline;
namespace test{
    void drawTriangle(Window* window);
    void drawTexture(Window* window);
    void drawTextureWithBuff(Window* window);

    void drawTwoTexture(Window* window);
    Shader drawTwoTextureWithBuff(Window* window);

    void transformTest(Window* window);
}