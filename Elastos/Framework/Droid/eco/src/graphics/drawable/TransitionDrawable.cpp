
#include "graphics/drawable/TransitionDrawable.h"
#include "graphics/drawable/CTransitionDrawable.h"
#include "os/SystemClock.h"
#include <elastos/Math.h>


using namespace Elastos::Core;

TransitionDrawable::TransitionState::TransitionState(
    /* [in] */ TransitionState* orig,
    /* [in] */ TransitionDrawable* owner,
    /* [in] */ IResources* res)
    : LayerState(orig, owner, res)
{}

//@Override
ECode TransitionDrawable::TransitionState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CTransitionDrawable::New(
            (Handle32)this, (IResources*)NULL, (ITransitionDrawable**)drawable);
}

//@Override
ECode TransitionDrawable::TransitionState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    return CTransitionDrawable::New(
            (Handle32)this, res, (ITransitionDrawable**)drawable);
}

//@Override
ECode TransitionDrawable::TransitionState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}



const Int32 TransitionDrawable::TRANSITION_STARTING;
const Int32 TransitionDrawable::TRANSITION_RUNNING;
const Int32 TransitionDrawable::TRANSITION_NONE;

/**
 * Create a new transition drawable with the specified list of layers. At least
 * 2 layers are required for this drawable to work properly.
 */
TransitionDrawable::TransitionDrawable(
    /* [in] */ IObjectContainer* layers)
    : mTransitionState(TRANSITION_NONE)
    , mReverse(FALSE)
    , mAlpha(0)
    , mCrossFade(FALSE)
{
    ASSERT_SUCCEEDED(Init(new TransitionState(NULL, NULL, NULL), layers));
}

/**
 * Create a new transition drawable with no layer. To work correctly, at least 2
 * layers must be added to this drawable.
 *
 * @see #TransitionDrawable(Drawable[])
 */
TransitionDrawable::TransitionDrawable()
    : mTransitionState(TRANSITION_NONE)
    , mReverse(FALSE)
    , mAlpha(0)
    , mCrossFade(FALSE)
{
//    this(new TransitionState(null, null, null), (Resources)null);
}

TransitionDrawable::TransitionDrawable(
    /* [in] */ TransitionState* state,
    /* [in] */ IResources* res)
    : LayerDrawable(state, res)
    , mTransitionState(TRANSITION_NONE)
    , mReverse(FALSE)
    , mAlpha(0)
    , mCrossFade(FALSE)
{}

TransitionDrawable::TransitionDrawable(
    /* [in] */ TransitionState* state,
    /* [in] */ IObjectContainer* layers)
    : LayerDrawable(layers, state)
    , mTransitionState(TRANSITION_NONE)
    , mReverse(FALSE)
    , mAlpha(0)
    , mCrossFade(FALSE)
{}

//@Override
AutoPtr<LayerDrawable::LayerState> TransitionDrawable::CreateConstantState(
    /* [in] */ LayerState* state,
    /* [in] */ IResources* res)
{
    return new TransitionState((TransitionState*)state, this, res);
}

/**
 * Begin the second layer on top of the first layer.
 *
 * @param durationMillis The length of the transition in milliseconds
 */
ECode TransitionDrawable::StartTransition(
    /* [in] */ Int32 durationMillis)
{
    mFrom = 0;
    mTo = 255;
    mAlpha = 0;
    mDuration = mOriginalDuration = durationMillis;
    mReverse = FALSE;
    mTransitionState = TRANSITION_STARTING;
    InvalidateSelf();
    return NOERROR;
}

/**
 * Show only the first layer.
 */
ECode TransitionDrawable::ResetTransition()
{
    mAlpha = 0;
    mTransitionState = TRANSITION_NONE;
    InvalidateSelf();
    return NOERROR;
}

/**
 * Reverses the transition, picking up where the transition currently is.
 * If the transition is not currently running, this will start the transition
 * with the specified duration. If the transition is already running, the last
 * known duration will be used.
 *
 * @param duration The duration to use if no transition is running.
 */
ECode TransitionDrawable::ReverseTransition(
    /* [in] */ Int32 duration)
{
    const Int64 time = SystemClock::GetUptimeMillis();
    // Animation is over
    if (time - mStartTimeMillis > mDuration) {
        if (mAlpha == 0) {
            mFrom = 0;
            mTo = 255;
            mAlpha = 0;
            mReverse = FALSE;
        }
        else {
            mFrom = 255;
            mTo = 0;
            mAlpha = 255;
            mReverse = TRUE;
        }
        mDuration = mOriginalDuration = duration;
        mTransitionState = TRANSITION_STARTING;
        InvalidateSelf();
        return NOERROR;
    }

    mReverse = !mReverse;
    mFrom = mAlpha;
    mTo = mReverse ? 0 : 255;
    mDuration = (Int32)(mReverse ? time - mStartTimeMillis :
            mOriginalDuration - (time - mStartTimeMillis));
    mTransitionState = TRANSITION_STARTING;
    return NOERROR;
}

//@Override
ECode TransitionDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    Boolean done = TRUE;

    switch (mTransitionState) {
        case TRANSITION_STARTING:
            mStartTimeMillis = SystemClock::GetUptimeMillis();
            done = FALSE;
            mTransitionState = TRANSITION_RUNNING;
            break;

        case TRANSITION_RUNNING:
            if (mStartTimeMillis >= 0) {
                Float normalized = (Float)
                        (SystemClock::GetUptimeMillis() - mStartTimeMillis) / mDuration;
                done = normalized >= 1.0f;
                normalized = Math::Min(normalized, 1.0f);
                mAlpha = (Int32)(mFrom  + (mTo - mFrom) * normalized);
            }
            break;
    }

    const Int32 alpha = mAlpha;
    const Boolean crossFade = mCrossFade;
    ArrayOf<AutoPtr<ChildDrawable> >* array = mLayerState->mChildren;
    IDrawable* d;

    d = (*array)[0]->mDrawable;
    if (crossFade) {
        d->SetAlpha(255 - alpha);
    }
    d->Draw(canvas);
    if (crossFade) {
        d->SetAlpha(0xFF);
    }

    if (alpha > 0) {
        d = (*array)[1]->mDrawable;
        d->SetAlpha(alpha);
        d->Draw(canvas);
        d->SetAlpha(0xFF);
    }

    if (!done) {
        InvalidateSelf();
    }

    return NOERROR;
}

/**
 * Enables or disables the cross fade of the drawables. When cross fade
 * is disabled, the first drawable is always drawn opaque. With cross
 * fade enabled, the first drawable is drawn with the opposite alpha of
 * the second drawable. Cross fade is disabled by default.
 *
 * @param enabled True to enable cross fading, false otherwise.
 */
ECode TransitionDrawable::SetCrossFadeEnabled(
    /* [in] */ Boolean enabled)
{
    mCrossFade = enabled;
    return NOERROR;
}

/**
 * Indicates whether the cross fade is enabled for this transition.
 *
 * @return True if cross fading is enabled, false otherwise.
 */
Boolean TransitionDrawable::IsCrossFadeEnabled()
{
    return mCrossFade;
}

ECode TransitionDrawable::Init(
    /* [in] */ IObjectContainer* layers)
{
    return Init(new TransitionState(NULL, NULL, NULL), layers);
}

ECode TransitionDrawable::Init()
{
    return Init(new TransitionState(NULL, NULL, NULL), (IResources*)NULL);
}

ECode TransitionDrawable::Init(
    /* [in] */ TransitionState* state,
    /* [in] */ IResources* res)
{
    return LayerDrawable::Init(state, res);
}

ECode TransitionDrawable::Init(
    /* [in] */ TransitionState* state,
    /* [in] */ IObjectContainer* layers)
{
    return LayerDrawable::Init(layers, state);
}

