
#ifndef __ROTATEANIMATION_H__
#define __ROTATEANIMATION_H__

#include "view/animation/Animation.h"

class RotateAnimation : public Animation
{
public:
    RotateAnimation();

    RotateAnimation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    RotateAnimation(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees);

    RotateAnimation(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    RotateAnimation(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Float pivotYType,
        /* [in] */ Float pivotYValue);

    CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

protected:
    CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees);

    CARAPI Init(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    CARAPI Init(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Float pivotYType,
        /* [in] */ Float pivotYValue);

private:
    Float mFromDegrees;
    Float mToDegrees;

    Int32 mPivotXType;
    Int32 mPivotYType;
    Float mPivotXValue;
    Float mPivotYValue;

    Float mPivotX;
    Float mPivotY;
};
#endif //__ROTATEANIMATION_H__
