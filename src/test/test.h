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

    void _3Dtest(Window* window);
    void _3DtestMuiltiCube(Window* window);
    void _3DMultiSmile(Window* window);

    void camera(Window* window);
    void cameraControl(Window* window);

    void color(Window* window);
    void reflect(Window* window);

    void AssimpLinkTest(Window* window);
    void ModelTest(Window* window);
}