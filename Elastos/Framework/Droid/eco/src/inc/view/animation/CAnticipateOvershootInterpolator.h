
#ifndef __CANTICIPATEOVERSHOOTINTERPOLATOR_H__
#define __CANTICIPATEOVERSHOOTINTERPOLATOR_H__

#include "_CAnticipateOvershootInterpolator.h"

CarClass(CAnticipateOvershootInterpolator)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float tension);

    CARAPI constructor(
        /* [in] */ Float tension,
        /* [in] */ Float extraTension);
    
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
    
    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mTension;
};
#endif //__CANTICIPATEOVERSHOOTINTERPOLATOR_H__
