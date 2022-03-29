#include "test/graphicTest.h"
#include "engine/Engine.h"
#include "system/RenderSystem.h"
void _test()
{
    //Util::testJson();
    //std::cout << std::endl;
    //GraphicTest::_shadowMapping_engine();
    //GraphicTest::_cube_map_test_();
    //GraphicTest::_component_test();
    //GraphicTest::_model_forward_();
    GraphicTest::_model_deferred_();
}

int main()
{
    Engine* engine = Engine::InitEngine();
    RenderSystem* renderSys = RenderSystem::Get();
    renderSys->SetRenderPath(Render::Deferred);
    _test();

    engine->StartEngine();
    return 1;
}