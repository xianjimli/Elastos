
#ifndef __CDECELERATEINTERPOLATOR_H__
#define __CDECELERATEINTERPOLATOR_H__

#include "_CDecelerateInterpolator.h"

CarClass(CDecelerateInterpolator)
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
};
#endif //__CDECELERATEINTERPOLATOR_H__
