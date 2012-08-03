
#ifndef __CLINEARINTERPOLATOR_H__
#define __CLINEARINTERPOLATOR_H__

#include "_CLinearInterpolator.h"

CarClass(CLinearInterpolator)
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
#endif //__CLINEARINTERPOLATOR_H__
