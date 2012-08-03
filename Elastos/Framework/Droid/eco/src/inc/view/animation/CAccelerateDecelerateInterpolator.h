
#ifndef __CACCELERATEDECELERATEINTERPOLATOR_H__
#define __CACCELERATEDECELERATEINTERPOLATOR_H__

#include "_CAccelerateDecelerateInterpolator.h"

CarClass(CAccelerateDecelerateInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
};
#endif //__CACCELERATEDECELERATEINTERPOLATOR_H__
