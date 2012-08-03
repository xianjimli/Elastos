
#include "view/animation/CGridLayoutAnimationController.h"

ECode CGridLayoutAnimationController::GetOrder(
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order);
    *order = GridLayoutAnimationController::GetOrder();

    return NOERROR;
}

ECode CGridLayoutAnimationController::SetOrder(
    /* [in] */ Int32 order)
{
    return GridLayoutAnimationController::SetOrder(order);
}

ECode CGridLayoutAnimationController::SetAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return GridLayoutAnimationController::SetAnimation(context, resourceID);
}

ECode CGridLayoutAnimationController::SetAnimationEx(
    /* [in] */ IAnimation* animation)
{
    return GridLayoutAnimationController::SetAnimation(animation);
}

ECode CGridLayoutAnimationController::GetAnimation(
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    AutoPtr<IAnimation> temp = GridLayoutAnimationController::GetAnimation();
    *animation = temp;
    if (*animation) {
        (*animation)->AddRef();
    }

    return NOERROR;
}

ECode CGridLayoutAnimationController::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return GridLayoutAnimationController::SetInterpolator(context, resourceID);
}

ECode CGridLayoutAnimationController::SetInterpolatorEx(
    /* [in] */ IInterpolator* interpolator)
{
    return GridLayoutAnimationController::SetInterpolator(interpolator);
}

ECode CGridLayoutAnimationController::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    AutoPtr<IInterpolator> temp = GridLayoutAnimationController::GetInterpolator();
    *interpolator = temp;
    if (*interpolator) {
        (*interpolator)->AddRef();
    }

    return NOERROR;
}

ECode CGridLayoutAnimationController::GetDelay(
    /* [out] */ Float* delay)
{
    VALIDATE_NOT_NULL(delay);
    *delay = GridLayoutAnimationController::GetDelay();

    return NOERROR;
}

ECode CGridLayoutAnimationController::SetDelay(
    /* [in] */ Float delay)
{
    return GridLayoutAnimationController::SetDelay(delay);
}

ECode CGridLayoutAnimationController::WillOverlap(
    /* [out] */ Boolean* willOverlap)
{
    VALIDATE_NOT_NULL(willOverlap);
    *willOverlap = GridLayoutAnimationController::WillOverlap();

    return NOERROR;
}

ECode CGridLayoutAnimationController::Start()
{
    return GridLayoutAnimationController::Start();
}

ECode CGridLayoutAnimationController::GetAnimationForView(
    /* [in] */ IView* view,
    /* [out] */ IAnimation** animation)
{
    VALIDATE_NOT_NULL(animation);
    AutoPtr<IAnimation> temp = GridLayoutAnimationController::GetAnimationForView(view);
    *animation = temp;
    if (*animation) {
        (*animation)->AddRef();
    }

    return NOERROR;
}

ECode CGridLayoutAnimationController::IsDone(
    /* [out] */ Boolean* isDone)
{
    VALIDATE_NOT_NULL(isDone);
    *isDone = GridLayoutAnimationController::IsDone();

    return NOERROR;
}

ECode CGridLayoutAnimationController::GetColumnDelay(
    /* [out] */ Float* columnDelay)
{
    VALIDATE_NOT_NULL(columnDelay);
    *columnDelay = GridLayoutAnimationController::GetColumnDelay();

    return NOERROR;
}

ECode CGridLayoutAnimationController::SetColumnDelay(
    /* [in] */ Float columnDelay)
{
    return GridLayoutAnimationController::SetColumnDelay(columnDelay);
}

ECode CGridLayoutAnimationController::GetRowDelay(
    /* [out] */ Float* rowDelay)
{
    VALIDATE_NOT_NULL(rowDelay);
    *rowDelay = GridLayoutAnimationController::IsDone();

    return NOERROR;
}

ECode CGridLayoutAnimationController::SetRowDelay(
    /* [in] */ Float rowDelay)
{
    return GridLayoutAnimationController::SetRowDelay(rowDelay);
}

ECode CGridLayoutAnimationController::GetDirection(
    /* [out] */ Int32* direction)
{
    VALIDATE_NOT_NULL(direction);
    *direction = GridLayoutAnimationController::IsDone();

    return NOERROR;
}

ECode CGridLayoutAnimationController::SetDirection(
    /* [in] */ Int32 direction)
{
    return GridLayoutAnimationController::SetDirection(direction);
}

ECode CGridLayoutAnimationController::GetDirectionPriority(
    /* [out] */ Int32* directionPriority)
{
    VALIDATE_NOT_NULL(directionPriority);
    *directionPriority = GridLayoutAnimationController::IsDone();

    return NOERROR;
}

ECode CGridLayoutAnimationController::SetDirectionPriority(
    /* [in] */ Int32 directionPriority)
{
    return GridLayoutAnimationController::SetDirectionPriority(directionPriority);
}

ECode CGridLayoutAnimationController::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return GridLayoutAnimationController::Init(context, attrs);
}

ECode CGridLayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation)
{
    return GridLayoutAnimationController::Init(animation);
}

ECode CGridLayoutAnimationController::constructor(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float columnDelay,
    /* [in] */ Float rowDelay)
{
    return GridLayoutAnimationController::Init(animation, columnDelay, rowDelay);
}
