
#include "graphics/CPixelXorXfermode.h"
#include <skia/effects/SkPixelXorXfermode.h>

ECode CPixelXorXfermode::constructor(
    /* [in] */ Int32 opColor)
{
    mNativeInstance = NativeCreate(opColor);
    return NOERROR;
}

SkXfermode* CPixelXorXfermode::NativeCreate(
    /* [in] */ Int32 opColor)
{
    return new SkPixelXorXfermode(opColor);
}