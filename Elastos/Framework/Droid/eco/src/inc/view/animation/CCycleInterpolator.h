
#ifndef __CCYCLEINTERPOLATOR_H__
#define __CCYCLEINTERPOLATOR_H__

#include "_CCycleInterpolator.h"

CarClass(CCycleInterpolator)
{
public:
    CARAPI constructor(
        /* [in] */ Float cycles);
    
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
    
    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mCycles;
};
#endif //__CCYCLEINTERPOLATOR_H__
