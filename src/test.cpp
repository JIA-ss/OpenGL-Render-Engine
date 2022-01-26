#include "test.h"
#include "resource/resourceManager.h"
#include "resource/resourceBasic.h"
void test::test()
{
    Resource::ShaderRef ref = Resource::ResourceManager::Instance()->GetResource("3D.fs", Resource::shader);
    std::cout << ref->getContent() << std::endl;
}