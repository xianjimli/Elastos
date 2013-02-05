
#include "graphics/CPixelXorXfermode.h"
#include <skia/effects/SkPixelXorXfermode.h>

ECode CPixelXorXfermode::constructor(
    /* [in] */ Int32 opColor)
{
    mNativeInstance = nativeCreate(opColor);
    return NOERROR;
}

SkXfermode* CPixelXorXfermode::nativeCreate(Int32 opColor)
{
    return new SkPixelXorXfermode(opColor);
}