
#include "graphics/CAvoidXfermode.h"
#include <skia/effects/SkAvoidXfermode.h>

ECode CAvoidXfermode::constructor(
    /* [in] */ Int32 opColor,
    /* [in] */ Int32 tolerance,
    /* [in] */ Int32 mode)
{
    if (tolerance < 0 || tolerance > 255) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mNativeInstance = NativeCreate(opColor, tolerance, mode);
    return NOERROR;
}

SkXfermode* CAvoidXfermode::NativeCreate(Int32 opColor,
    /* [in] */ Int32 tolerance,
    /* [in] */ Int32 nativeMode)
{
    return new SkAvoidXfermode(opColor, tolerance, (SkAvoidXfermode::Mode)nativeMode);
}