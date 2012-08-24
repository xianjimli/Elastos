
#include "graphics/Rasterizer.h"

// {2D3A6B44-0939-4A44-A9C2-6C1E9E71D8FC}
extern "C" const InterfaceID EIID_Rasterizer =
{ 0x2d3a6b44, 0x939, 0x4a44, { 0xa9, 0xc2, 0x6c, 0x1e, 0x9e, 0x71, 0xd8, 0xfc } };


Rasterizer::~Rasterizer()
{
    Finalizer(mNativeInstance);
}

void Rasterizer::Finalizer(
    /* [in] */ SkRasterizer* nObj)
{
    nObj->safeUnref();
}
