
#ifndef __SCALEANIMATION_H__
#define __SCALEANIMATION_H__

#include "view/animation/Animation.h"

class ScaleAnimation : public Animation
{
public:
    ScaleAnimation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    ScaleAnimation(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY);

    ScaleAnimation(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    ScaleAnimation(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Float pivotYType,
        /* [in] */ Float pivotYValue);

    //@Override
    CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

protected:
    ScaleAnimation();

    //@Override
    CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY);

    CARAPI Init(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    CARAPI Init(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Float pivotYType,
        /* [in] */ Float pivotYValue);

private:
    Float mFromX;
    Float mToX;
    Float mFromY;
    Float mToY;

    Int32 mPivotXType;
    Int32 mPivotYType;
    Float mPivotXValue;
    Float mPivotYValue;

    Float mPivotX;
    Float mPivotY;
};
#endif //__SCALEANIMATION_H__
