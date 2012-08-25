
#ifndef __CANTICIPATEINTERPOLATOR_H__
#define __CANTICIPATEINTERPOLATOR_H__

#include "_CAnticipateInterpolator.h"

CarClass(CAnticipateInterpolator)
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
#endif //__CANTICIPATEINTERPOLATOR_H__
