#include "GraphicBase.h"

GRAPHIC_NAMESPACE_USING

std::map<size_t, graphic_meta>* GraphicBase::GraphicDerivedClasses = new (std::map<size_t, graphic_meta>)();
const size_t GraphicBase::GraphicId = GraphicBase::registerGraphicId<GraphicBase>("GraphicBase");