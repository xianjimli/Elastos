
#include "graphics/ColorFilter.h"

// {2C6350C4-CC6F-4B0C-AB2D-B43F37E35194}
extern "C" const InterfaceID EIID_ColorFilter =
    { 0x2c6350c4, 0xcc6f, 0x4b0c, { 0xab, 0x2d, 0xb4, 0x3f, 0x37, 0xe3, 0x51, 0x94 } };


ColorFilter::~ColorFilter()
{
    Finalizer(mNativeInstance);
}

void ColorFilter::Finalizer(
    /* [in] */ SkColorFilter* nObj)
{
    nObj->safeUnref();
}
