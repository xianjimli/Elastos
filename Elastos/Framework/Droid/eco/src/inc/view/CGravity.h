
#ifndef __CGRAVITY_H__
#define __CGRAVITY_H__

#include "_CGravity.h"

CarClass(CGravity)
{
public:
    CARAPI Apply(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ IRect* container,
        /* [in] */ IRect* outRect);

    CARAPI ApplyEx(
        /* [in] */ Int32 gravity,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ IRect* container,
        /* [in] */ Int32 xAdj,
        /* [in] */ Int32 yAdj,
        /* [in] */ IRect* outRect);

    CARAPI ApplyDisplay(
        /* [in] */ Int32 gravity,
        /* [in] */ IRect* display,
        /* [in] */ IRect* inoutObj);

    CARAPI IsVertical(
        /* [in] */ Int32 gravity,
        /* [out] */ Boolean* isVertical);

    CARAPI IsHorizontal(
        /* [in] */ Int32 gravity,
        /* [out] */ Boolean* isHorizontal);
};

#endif //__CGRAVITY_H__
