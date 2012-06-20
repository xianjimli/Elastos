
#include "graphics/CPorterDuffXfermode.h"
#include <SkPorterDuff.h>

PInterface CPorterDuffXfermode::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Xfermode) {
        return reinterpret_cast<PInterface>((Xfermode*)this);
    }
    return _CPorterDuffXfermode::Probe(riid);
}

ECode CPorterDuffXfermode::constructor(
    /* [in] */ PorterDuffMode mode)
{
    mNativeInstance = NativeCreateXfermode(mode);
    return NOERROR;
}

SkXfermode* CPorterDuffXfermode::NativeCreateXfermode(
    /* [in] */ PorterDuffMode mode)
{
    return SkPorterDuff::CreateXfermode((SkPorterDuff::Mode)mode);
}
