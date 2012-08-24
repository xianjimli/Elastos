
#ifndef __CROTATEANIMATION_H__
#define __CROTATEANIMATION_H__

#include "_CRotateAnimation.h"
#include "view/animation/RotateAnimation.h"
#include "view/animation/AnimationMacro.h"

CarClass(CRotateAnimation), public RotateAnimation
{
public:
    IANIMATION_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees);

    CARAPI constructor(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    CARAPI constructor(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Float pivotYType,
        /* [in] */ Float pivotYValue);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    // TODO: Add your private member variables here.
};

#endif // __CROTATEANIMATION_H__
