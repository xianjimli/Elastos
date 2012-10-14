
#ifndef __COVERSHOOTINTERPOLATOR_H__
#define __COVERSHOOTINTERPOLATOR_H__

#include "_COvershootInterpolator.h"

CarClass(COvershootInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float tension);
    
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
    
    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mTension;
};
#endif //__COVERSHOOTINTERPOLATOR_H__
