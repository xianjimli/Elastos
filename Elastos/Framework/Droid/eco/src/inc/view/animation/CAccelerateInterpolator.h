
#ifndef __CACCELERATEINTERPOLATOR_H__
#define __CACCELERATEINTERPOLATOR_H__

#include "_CAccelerateInterpolator.h"

CarClass(CAccelerateInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float factor);
    
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
    
    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mFactor;
    Double mDoubleFactor;
};
#endif //__CACCELERATEINTERPOLATOR_H__
