#include "test/graphicTest.h"
#include "engine/Engine.h"

void _test()
{
    //Util::testJson();
    //std::cout << std::endl;
    //GraphicTest::_shadowMapping_engine();
    GraphicTest::_cube_map_test_();
}

int main()
{
    Engine* engine = Engine::InitEngine();

    _test();

    engine->StartEngine();
    return 1;
}