
#ifndef __TRANSLATEANIMATION_H__
#define __TRANSLATEANIMATION_H__

#include "view/animation/Animation.h"

class TranslateAnimation : public Animation
{
public:
    TranslateAnimation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    TranslateAnimation(
        /* [in] */ Float fromXDelta,
        /* [in] */ Float toXDelta,
        /* [in] */ Float fromYDelta,
        /* [in] */ Float toYDelta);

    TranslateAnimation(
        /* [in] */ Float fromXType,
        /* [in] */ Float fromXValue,
        /* [in] */ Float toXType,
        /* [in] */ Float toXValue,
        /* [in] */ Float fromYType,
        /* [in] */ Float fromYValue,
        /* [in] */ Float toYType,
        /* [in] */ Float toYValue);

    //@Override
    CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

protected:
    TranslateAnimation();

    //@Override
    CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Float fromXDelta,
        /* [in] */ Float toXDelta,
        /* [in] */ Float fromYDelta,
        /* [in] */ Float toYDelta);

    CARAPI Init(
        /* [in] */ Float fromXType,
        /* [in] */ Float fromXValue,
        /* [in] */ Float toXType,
        /* [in] */ Float toXValue,
        /* [in] */ Float fromYType,
        /* [in] */ Float fromYValue,
        /* [in] */ Float toYType,
        /* [in] */ Float toYValue);

private:
    Int32 mFromXType;
    Int32 mToXType;

    Int32 mFromYType;
    Int32 mToYType;

    Float mFromXValue;
    Float mToXValue;

    Float mFromYValue;
    Float mToYValue;

    Float mFromXDelta;
    Float mToXDelta;
    Float mFromYDelta;
    Float mToYDelta;
};
#endif //__TRANSLATEANIMATION_H__
