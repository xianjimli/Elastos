
#ifndef __CTRANSLATEANIMATION_H__
#define __CTRANSLATEANIMATION_H__

#include "_CTranslateAnimation.h"
#include "view/animation/TranslateAnimation.h"
#include "view/animation/AnimationMacro.h"

CarClass(CTranslateAnimation), public TranslateAnimation
{
public:
    IANIMATION_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet * attrs);

    CARAPI constructor(
        /* [in] */ Float fromXDelta,
        /* [in] */ Float toXDelta,
        /* [in] */ Float fromYDelta,
        /* [in] */ Float toYDelta);

    CARAPI constructor(
        /* [in] */ Float fromXType,
        /* [in] */ Float fromXValue,
        /* [in] */ Float toXType,
        /* [in] */ Float toXValue,
        /* [in] */ Float fromYType,
        /* [in] */ Float fromYValue,
        /* [in] */ Float toYType,
        /* [in] */ Float toYValue);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTRANSLATEANIMATION_H__
