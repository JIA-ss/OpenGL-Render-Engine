#include "test/graphicTest.h"
#include "system/Engine.h"

void _test()
{
    //Util::testJson();
    //std::cout << std::endl;
    GraphicTest::_shadowMapping_engine();
}

int main()
{
    Engine* engine = Engine::InitEngine();

    _test();

    engine->StartEngine();
    return 1;
}