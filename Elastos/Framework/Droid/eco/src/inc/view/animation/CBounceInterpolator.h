
#ifndef __CBOUNCEINTERPOLATOR_H__
#define __CBOUNCEINTERPOLATOR_H__

#include "_CBounceInterpolator.h"

CarClass(CBounceInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mFactor;
};
#endif //__CBOUNCEINTERPOLATOR_H__
