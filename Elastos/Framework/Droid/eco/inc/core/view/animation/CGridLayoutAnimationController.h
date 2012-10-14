
#ifndef __CGRIDLAYOUTANIMATIONCONTROLLER_H__
#define __CGRIDLAYOUTANIMATIONCONTROLLER_H__

#include "_CGridLayoutAnimationController.h"
#include "view/animation/GridLayoutAnimationController.h"

CarClass(CGridLayoutAnimationController), public GridLayoutAnimationController
{
public:
    CARAPI GetOrder(
        /* [out] */ Int32* order);

    CARAPI SetOrder(
        /* [in] */ Int32 order);

    CARAPI SetAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    CARAPI SetAnimationEx(
        /* [in] */ IAnimation* animation);

    CARAPI GetAnimation(
        /* [out] */ IAnimation** animation);

    CARAPI SetInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resourceID);

    CARAPI SetInterpolatorEx(
        /* [in] */ IInterpolator* interpolator);

    CARAPI GetInterpolator(
        /* [out] */ IInterpolator** interpolator);

    CARAPI GetDelay(
        /* [out] */ Float* delay);

    CARAPI SetDelay(
        /* [in] */ Float delay);

    CARAPI WillOverlap(
        /* [out] */ Boolean* willOverlap);

    CARAPI Start();

    CARAPI GetAnimationForView(
        /* [in] */ IView* View,
        /* [out] */ IAnimation** animation);

    CARAPI IsDone(
        /* [out] */ Boolean* isDone);

    CARAPI GetColumnDelay(
        /* [out] */ Float* columnDelay);

    CARAPI SetColumnDelay(
        /* [in] */ Float columnDelay);

    CARAPI GetRowDelay(
        /* [out] */ Float* rowDelay);

    CARAPI SetRowDelay(
        /* [in] */ Float rowDelay);

    CARAPI GetDirection(
        /* [out] */ Int32* direction);

    CARAPI SetDirection(
        /* [in] */ Int32 direction);

    CARAPI GetDirectionPriority(
        /* [out] */ Int32* directionPriority);

    CARAPI SetDirectionPriority(
        /* [in] */ Int32 directionPriority);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IAnimation* animation);

    CARAPI constructor(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float columnDelay,
        /* [in] */ Float rowDelay);

private:
    // TODO: Add your private member variables here.
};

#endif // __CGRIDLAYOUTANIMATIONCONTROLLER_H__
