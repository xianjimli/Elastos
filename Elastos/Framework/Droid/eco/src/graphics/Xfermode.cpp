
#include "graphics/Xfermode.h"

// {5DFFFEB0-3161-4BFE-9596-F50C528C69E7}
extern "C" const InterfaceID EIID_Xfermode =
    { 0x5dfffeb0, 0x3161, 0x4bfe, { 0x95, 0x96, 0xf5, 0xc, 0x52, 0x8c, 0x69, 0xe7 } };


Xfermode::~Xfermode()
{
    Finalizer(mNativeInstance);
}

void Xfermode::Finalizer(
    /* [in] */ SkXfermode* xfermode)
{
    xfermode->safeUnref();
}
