
#ifndef __CPATHDASHPATHEFFECT_H__
#define __CPATHDASHPATHEFFECT_H__

#include "_CPathDashPathEffect.h"
#include "graphics/PathEffect.h"
#include "graphics/CPath.h"

CarClass(CPathDashPathEffect), public PathEffect
{
public:
    CARAPI constructor(
        /* [in] */ IPath* shape,
        /* [in] */ Float advance,
        /* [in] */ Float phase,
        /* [in] */ PathDashPathEffectStyle style);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

private:
    static CARAPI_(SkPathEffect*) NativeCreate(
        /* [in] */ SkPath* nativePath,
        /* [in] */ Float advance,
        /* [in] */ Float phase,
        /* [in] */ Int32 nativeStyle);
};

#endif // __CPATHDASHPATHEFFECT_H__
