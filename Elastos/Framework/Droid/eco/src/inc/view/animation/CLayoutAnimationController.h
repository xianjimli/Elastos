
#ifndef __CLAYOUTANIMATIONCONTROLLER_H__
#define __CLAYOUTANIMATIONCONTROLLER_H__

#include "_CLayoutAnimationController.h"
#include "view/animation/LayoutAnimationController.h"

CarClass(CLayoutAnimationController), public LayoutAnimationController
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

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IAnimation* animation);

    CARAPI constructor(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float delay);

private:
    // TODO: Add your private member variables here.
};

#endif // __CLAYOUTANIMATIONCONTROLLER_H__
