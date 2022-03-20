#pragma once
#include "graphic/GraphicInclude.h"
#include "pipline/window.h"
using namespace Pipline;
namespace GraphicTest
{

void simpleImage(Window* window);

void cube(Window* window);

void _3d(Window* window);

void _model(Window* window);

void _depthTest(Window* window);

void _stencilTest(Window* window);

void _blend(Window* window);

void _faceCulling(Window* window);

void _frameBuffer(Window* window);

void _renderQueue(Window* window);

void _shadowMapping(Window* window);

void _shadowMapping_engine();
}