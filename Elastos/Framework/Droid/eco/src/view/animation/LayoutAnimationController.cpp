
#include "view/animation/LayoutAnimationController.h"
#include "view/animation/AnimationUtils.h"
#include "view/animation/Animation.h"
#include "view/animation/CLinearInterpolator.h"
#include "view/CViewGroupLayoutParams.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

static Int32 R_Styleable_LayoutAnimation[] = {
    0x01010141, 0x010101cc, 0x010101cd, 0x010101ce
};

LayoutAnimationController::LayoutAnimationController()
{}

/**
 * Creates a new layout animation controller from external resources.
 *
 * @param context the Context the view  group is running in, through which
 *        it can access the resources
 * @param attrs the attributes of the XML tag that is inflating the
 *        layout animation controller
 */
LayoutAnimationController::LayoutAnimationController(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

/**
 * Creates a new layout animation controller with the specified delay
 * and the specified animation.
 *
 * @param animation the animation to use on each child of the view group
 * @param delay the delay by which each child's animation must be offset
 */
LayoutAnimationController::LayoutAnimationController(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float delay)
{
    Init(animation, delay);
}

/**
 * Returns the order used to compute the delay of each child's animation.
 *
 * @return one of {@link #LayoutAnimationController_ORDER_NORMAL}, {@link #LayoutAnimationController_ORDER_REVERSE} or
 *         {@link #LayoutAnimationController_ORDER_RANDOM)
 *
 * @attr ref android.R.styleable#LayoutAnimation_animationOrder
 */
Int32 LayoutAnimationController::GetOrder()
{
    return mOrder;
}

/**
 * Sets the order used to compute the delay of each child's animation.
 *
 * @param order one of {@link #LayoutAnimationController_ORDER_NORMAL}, {@link #LayoutAnimationController_ORDER_REVERSE} or
 *        {@link #LayoutAnimationController_ORDER_RANDOM}
 *
 * @attr ref android.R.styleable#LayoutAnimation_animationOrder
 */
ECode LayoutAnimationController::SetOrder(
    /* [in] */ Int32 order)
{
    mOrder = order;
    return NOERROR;
}

/**
 * Sets the animation to be run on each child of the view group on which
 * this layout animation controller is .
 *
 * @param context the context from which the animation must be inflated
 * @param resourceID the resource identifier of the animation
 *
 * @see #SetAnimation(Animation)
 * @see #getAnimation() 
 *
 * @attr ref android.R.styleable#LayoutAnimation_animation
 */
ECode LayoutAnimationController::SetAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    AutoPtr<IAnimation> animation;
    AnimationUtils::LoadAnimation(context, resourceID, (IAnimation**)&animation);
    return SetAnimation(animation);
}

/**
 * Sets the animation to be run on each child of the view group on which
 * this layout animation controller is .
 *
 * @param animation the animation to run on each child of the view group

 * @see #SetAnimation(android.content.Context, Int32)
 * @see #getAnimation()
 *
 * @attr ref android.R.styleable#LayoutAnimation_animation
 */
ECode LayoutAnimationController::SetAnimation(
    /* [in] */ IAnimation* animation)
{
    mAnimation = animation;
    mAnimation->SetFillBefore(TRUE);

    return NOERROR;
}

/**
 * Returns the animation applied to each child of the view group on which
 * this controller is set.
 *
 * @return an {@link android.view.animation.Animation} instance
 *
 * @see #SetAnimation(android.content.Context, Int32)
 * @see #SetAnimation(Animation)
 */
AutoPtr<IAnimation> LayoutAnimationController::GetAnimation()
{
    return mAnimation;
}

/**
 * Sets the interpolator used to interpolate the delays between the
 * children.
 *
 * @param context the context from which the interpolator must be inflated
 * @param resourceID the resource identifier of the interpolator
 *
 * @see #getInterpolator()
 * @see #SetInterpolator(Interpolator)
 *
 * @attr ref android.R.styleable#LayoutAnimation_interpolator
 */
ECode LayoutAnimationController::SetInterpolator(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    AutoPtr<IInterpolator> interpolator;
    AnimationUtils::LoadInterpolator(
        context, resourceID, (IInterpolator**)&interpolator);
    return SetInterpolator(interpolator);
}

/**
 * Sets the interpolator used to interpolate the delays between the
 * children.
 *
 * @param interpolator the interpolator
 *
 * @see #getInterpolator()
 * @see #SetInterpolator(Interpolator)
 *
 * @attr ref android.R.styleable#LayoutAnimation_interpolator
 */
ECode LayoutAnimationController::SetInterpolator(
    /* [in] */ IInterpolator* interpolator)
{
    mInterpolator = interpolator;
    return NOERROR;
}

/**
 * Returns the interpolator used to interpolate the delays between the
 * children.
 *
 * @return an {@link android.view.animation.Interpolator}
 */
AutoPtr<IInterpolator> LayoutAnimationController::GetInterpolator()
{
    return mInterpolator;
}

/**
 * Returns the delay by which the children's animation are offset. The
 * delay is expressed as a fraction of the animation duration.
 *
 * @return a fraction of the animation duration
 *
 * @see #setDelay(Float)
 */
Float LayoutAnimationController::GetDelay()
{
    return mDelay;
}

/**
 * Sets the delay, as a fraction of the animation duration, by which the
 * children's animations are offset. The general formula is:
 *
 * <pre>
 * child animation delay = child index * delay * animation duration
 * </pre>
 *
 * @param delay a fraction of the animation duration
 *
 * @see #getDelay()
 */
ECode LayoutAnimationController::SetDelay(
    /* [in] */ Float delay)
{
    mDelay = delay;
    return NOERROR;
}

/**
 * Indicates whether two children's animations will overlap. Animations
 * overlap when the delay is lower than 100% (or 1.0).
 *
 * @return TRUE if animations will overlap, false otherwise
 */
Boolean LayoutAnimationController::WillOverlap()
{
    return mDelay < 1.0f;
}

/**
 * Starts the animation.
 */
ECode LayoutAnimationController::Start()
{
    mAnimation->GetDuration(&mDuration);
    mMaxDelay = Math::INT64_MIN_VALUE;
    mAnimation->SetStartTime(-1);

    return NOERROR;
}

/**
 * Returns the animation to be applied to the specified view. The returned
 * animation is delayed by an offset computed according to the information
 * provided by
 * {@link android.view.animation.LayoutAnimationController.AnimationParameters}.
 * This method is called by view groups to obtain the animation to set on
 * a specific child.
 *
 * @param view the view to animate
 * @return an animation delayed by the number of milliseconds returned by
 *         {@link #GetDelayForView(android.view.View)}
 *
 * @see #getDelay()
 * @see #setDelay(Float)
 * @see #GetDelayForView(android.view.View)
 */
AutoPtr<IAnimation> LayoutAnimationController::GetAnimationForView(
    /* [in] */ IView* view)
{
    Int64 startOffset;
    mAnimation->GetStartOffset(&startOffset);
    Int64 delay = GetDelayForView(view) + startOffset;
    mMaxDelay = Math::Max(mMaxDelay, delay);

    AutoPtr<IAnimation> animation;
    //mAnimation->Clone((IAnimation**)&animation);
    //animation->SetStartOffset(delay);
    return animation;
}

/**
 * Indicates whether the layout animation is over or not. A layout animation
 * is considered done when the animation with the longest delay is done.
 *
 * @return TRUE if all of the children's animations are over, false otherwise
 */
Boolean LayoutAnimationController::IsDone()
{
    Int64 startTime;
    mAnimation->GetStartTime(&startTime);
    return AnimationUtils::CurrentAnimationTimeMillis() >
        startTime + mMaxDelay + mDuration;
}

/**
 * Returns the amount of milliseconds by which the specified view's
 * animation must be delayed or offset. Subclasses should override this
 * method to return a suitable value.
 *
 * This implementation returns <code>child animation delay</code>
 * milliseconds where:
 *
 * <pre>
 * child animation delay = child index * delay
 * </pre>
 *
 * The index is retrieved from the
 * {@link android.view.animation.LayoutAnimationController.AnimationParameters}
 * found in the view's {@link android.view.ViewGroup.LayoutParams}.
 *
 * @param view the view for which to obtain the animation's delay
 * @return a delay in milliseconds
 *
 * @see #getAnimationForView(android.view.View)
 * @see #getDelay()
 * @see #getTransformedIndex(android.view.animation.LayoutAnimationController.AnimationParameters)
 * @see android.view.ViewGroup.LayoutParams
 */
Int64 LayoutAnimationController::GetDelayForView(
    /* [in] */ IView* view)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    view->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    AnimationParameters* params = ((CViewGroupLayoutParams*)lp.Get())->mLayoutAnimationParameters;

    if (params == NULL) {
        return 0;
    }

    Int64 duration;
    mAnimation->GetDuration(&duration);
    Float delay = mDelay * duration;
    Int64 viewDelay = (Int64)(GetTransformedIndex(params) * delay);
    Float totalDelay = delay * params->mCount;

    if (mInterpolator == NULL) {
        ASSERT_SUCCEEDED(CLinearInterpolator::New((IInterpolator**)&mInterpolator));
    }

    Float normalizedDelay = viewDelay / totalDelay;
    mInterpolator->GetInterpolation(normalizedDelay, &normalizedDelay);

    return (Int64)(normalizedDelay * totalDelay);
}

/**
 * Transforms the index stored in
 * {@link android.view.animation.LayoutAnimationController.AnimationParameters}
 * by the order returned by {@link #getOrder()}. Subclasses should override
 * this method to provide additional support for other types of ordering.
 * This method should be invoked by
 * {@link #GetDelayForView(android.view.View)} prior to any computation. 
 *
 * @param params the animation parameters containing the index
 * @return a transformed index
 */
Int32 LayoutAnimationController::GetTransformedIndex(
    /* [in] */ AnimationParameters* params)
{
    switch (GetOrder()) {
    case LayoutAnimationController_ORDER_REVERSE:
        return params->mCount - 1 - params->mIndex;
    case LayoutAnimationController_ORDER_RANDOM:
        /*if (mRandomizer == null) {
            mRandomizer = new Random();
        }
        return (Int32) (params.count * mRandomizer.nextFloat());*/
    case LayoutAnimationController_ORDER_NORMAL:
    default:
        return params->mIndex;
    }
}

ECode LayoutAnimationController::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_LayoutAnimation,
        sizeof(R_Styleable_LayoutAnimation) / sizeof(Int32))/*com.android.internal.R.styleable.LayoutAnimation*/,
        (ITypedArray**)&a);

    AutoPtr<ITypedValue> value;
    a->PeekValue(
        1/*com.android.internal.R.styleable.LayoutAnimation_delay*/,
        (ITypedValue**)&value);

    AutoPtr<Animation::Description> d = Animation::Description::ParseValue(value);
    mDelay = d->mValue;

    a->GetInt32(
        3/*com.android.internal.R.styleable.LayoutAnimation_animationOrder*/,
        LayoutAnimationController_ORDER_NORMAL, &mOrder);

    Int32 resource;
    a->GetResourceId(
        2/*com.android.internal.R.styleable.LayoutAnimation_animation*/,
        0, &resource);
    if (resource > 0) {
        SetAnimation(context, resource);
    }

    a->GetResourceId(
        0/*com.android.internal.R.styleable.LayoutAnimation_interpolator*/,
        0, &resource);
    if (resource > 0) {
        SetInterpolator(context, resource);
    }

    a->Recycle();

    return NOERROR;
}

/**
 * Creates a new layout animation controller with the specified delay
 * and the specified animation.
 *
 * @param animation the animation to use on each child of the view group
 * @param delay the delay by which each child's animation must be offset
 */
ECode LayoutAnimationController::Init(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float delay)
{
    mDelay = delay;
    SetAnimation(animation);

    return NOERROR;
}
