
#include "graphics/CPixelXorXfermode.h"
#include <skia/effects/SkPixelXorXfermode.h>


ECode CPixelXorXfermode::constructor(
    /* [in] */ Int32 opColor)
{
    mNativeInstance = NativeCreate(opColor);
    return NOERROR;
}

PInterface CPixelXorXfermode::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_Xfermode) {
        return reinterpret_cast<PInterface>((Xfermode*)this);
    }
    return _CPixelXorXfermode::Probe(riid);
}

SkXfermode* CPixelXorXfermode::NativeCreate(
    /* [in] */ Int32 opColor)
{
    return new SkPixelXorXfermode(opColor);
}
