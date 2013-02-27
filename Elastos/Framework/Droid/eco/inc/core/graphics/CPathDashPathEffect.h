
#ifndef __CPATHDASHPATHEFFECT_H__
#define __CPATHDASHPATHEFFECT_H__

#include "_CPathDashPathEffect.h"
#include "graphics/PathEffect.h"
#include "graphics/CPath.h"

CarClass(CPathDashPathEffect), public PathEffect
{
public:
    CARAPI constructor(
        /* [in] */ IPath * pShape,
        /* [in] */ Float advance,
        /* [in] */ Float phase,
        /* [in] */ PathDashPathEffectStyle style);

private:
    static CARAPI_(SkPathEffect*) NativeCreate(
        /* [in] */ SkPath* native_path,
        /* [in] */ Float advance,
        /* [in] */ Float phase,
        /* [in] */ Int32 native_style);
};

#endif // __CPATHDASHPATHEFFECT_H__
