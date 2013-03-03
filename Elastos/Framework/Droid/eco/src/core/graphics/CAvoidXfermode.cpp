
#include "graphics/CAvoidXfermode.h"
#include <skia/effects/SkAvoidXfermode.h>

ECode CAvoidXfermode::constructor(
    /* [in] */ Int32 opColor,
    /* [in] */ Int32 tolerance,
    /* [in] */ Int32 mode)
{
    if (tolerance < 0 || tolerance > 255) {
        // throw new IllegalArgumentException("tolerance must be 0..255");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mNativeInstance = NativeCreate(opColor, tolerance, mode);
    return NOERROR;
}

PInterface CAvoidXfermode::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_Xfermode) {
        return reinterpret_cast<PInterface>((Xfermode*)this);
    }
    return _CAvoidXfermode::Probe(riid);
}

SkXfermode* CAvoidXfermode::NativeCreate(
    /* [in] */ Int32 opColor,
    /* [in] */ Int32 tolerance,
    /* [in] */ Int32 nativeMode)
{
    return new SkAvoidXfermode(opColor, tolerance, (SkAvoidXfermode::Mode)nativeMode);
}