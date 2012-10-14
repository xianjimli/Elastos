
#include "widget/EdgeGlow.h"
#include "os/SystemClock.h"
#include "view/animation/CDecelerateInterpolator.h"
#include "view/animation/AnimationUtils.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const char* EdgeGlow::TAG = "EdgeGlow";
const Int32 EdgeGlow::RECEDE_TIME;
const Int32 EdgeGlow::PULL_TIME;
const Int32 EdgeGlow::PULL_DECAY_TIME;
const Float EdgeGlow:: MAX_ALPHA;
const Float EdgeGlow:: HELD_EDGE_ALPHA;
const Float EdgeGlow:: HELD_EDGE_SCALE_Y;
const Float EdgeGlow:: HELD_GLOW_ALPHA;
const Float EdgeGlow:: HELD_GLOW_SCALE_Y;
const Float EdgeGlow:: MAX_GLOW_HEIGHT;
const Float EdgeGlow:: PULL_GLOW_BEGIN;
const Float EdgeGlow:: PULL_EDGE_BEGIN;
const Int32 EdgeGlow::MIN_VELOCITY;
const Float EdgeGlow:: EPSILON;
const Int32 EdgeGlow::STATE_IDLE;
const Int32 EdgeGlow::STATE_PULL;
const Int32 EdgeGlow::STATE_ABSORB;
const Int32 EdgeGlow::STATE_RECEDE;
const Int32 EdgeGlow::STATE_PULL_DECAY;
const Int32 EdgeGlow::PULL_DISTANCE_EDGE_FACTOR;
const Int32 EdgeGlow::PULL_DISTANCE_GLOW_FACTOR;
const Float EdgeGlow:: PULL_DISTANCE_ALPHA_GLOW_FACTOR;
const Int32 EdgeGlow::VELOCITY_EDGE_FACTOR;
const Int32 EdgeGlow::VELOCITY_GLOW_FACTOR;

EdgeGlow::EdgeGlow(
    /* [in] */ IDrawable* edge,
    /* [in] */ IDrawable* glow)
    : mEdge(edge)
    , mGlow(glow)
    , mWidth(0)
    , mHeight(0)
    , mEdgeAlpha(0.0)
    , mEdgeScaleY(0.0)
    , mGlowAlpha(0.0)
    , mGlowScaleY(0.0)
    , mEdgeAlphaStart(0.0)
    , mEdgeAlphaFinish(0.0)
    , mEdgeScaleYStart(0.0)
    , mEdgeScaleYFinish(0.0)
    , mGlowAlphaStart(0.0)
    , mGlowAlphaFinish(0.0)
    , mGlowScaleYStart(0.0)
    , mGlowScaleYFinish(0.0)
    , mStartTime(0)
    , mDuration(0.0)
    , mState(STATE_IDLE)
    , mPullDistance(0.0)
{
    ASSERT_SUCCEEDED(CDecelerateInterpolator::New((IInterpolator**)&mInterpolator));
}

void EdgeGlow::SetSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWidth = width;
    mHeight = height;
}

Boolean EdgeGlow::IsFinished()
{
    return mState == STATE_IDLE;
}

void EdgeGlow::Finish()
{
    mState = STATE_IDLE;
}

/**
 * Call when the object is pulled by the user.
 *
 * @param deltaDistance Change in distance since the last call
 */
void EdgeGlow::OnPull(
    /* [in] */ Float deltaDistance)
{
    Int64 now = AnimationUtils::CurrentAnimationTimeMillis();
    if (mState == STATE_PULL_DECAY && now - mStartTime < mDuration) {
        return;
    }

    if (mState != STATE_PULL) {
        mGlowScaleY = PULL_GLOW_BEGIN;
    }

    mState = STATE_PULL;

    mStartTime = now;
    mDuration = PULL_TIME;

    mPullDistance += deltaDistance;
    Float distance = Math::Abs(mPullDistance);

    mEdgeAlpha = mEdgeAlphaStart = Math::Max(PULL_EDGE_BEGIN, Math::Min(distance, MAX_ALPHA));
    mEdgeScaleY = mEdgeScaleYStart = Math::Max(
        HELD_EDGE_SCALE_Y, Math::Min(distance * PULL_DISTANCE_EDGE_FACTOR, 1.f));

    mGlowAlpha = mGlowAlphaStart = Math::Min(MAX_ALPHA,
        mGlowAlpha + (Math::Abs(deltaDistance) * PULL_DISTANCE_ALPHA_GLOW_FACTOR));

    Float glowChange = Math::Abs(deltaDistance);
    if (deltaDistance > 0 && mPullDistance < 0) {
        glowChange = -glowChange;
    }

    if (mPullDistance == 0) {
        mGlowScaleY = 0;
    }

    // Do not allow glow to get larger than MAX_GLOW_HEIGHT.
    mGlowScaleY = mGlowScaleYStart = Math::Min(MAX_GLOW_HEIGHT, Math::Max(
        0.f, mGlowScaleY + glowChange * PULL_DISTANCE_GLOW_FACTOR));

    mEdgeAlphaFinish = mEdgeAlpha;
    mEdgeScaleYFinish = mEdgeScaleY;
    mGlowAlphaFinish = mGlowAlpha;
    mGlowScaleYFinish = mGlowScaleY;
}

/**
 * Call when the object is released after being pulled.
 */
void EdgeGlow::OnRelease()
{
    mPullDistance = 0;

    if (mState != STATE_PULL && mState != STATE_PULL_DECAY) {
        return;
    }

    mState = STATE_RECEDE;
    mEdgeAlphaStart = mEdgeAlpha;
    mEdgeScaleYStart = mEdgeScaleY;
    mGlowAlphaStart = mGlowAlpha;
    mGlowScaleYStart = mGlowScaleY;

    mEdgeAlphaFinish = 0.f;
    mEdgeScaleYFinish = 0.f;
    mGlowAlphaFinish = 0.f;
    mGlowScaleYFinish = 0.f;

    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mDuration = RECEDE_TIME;
}

/**
 * Call when the effect absorbs an impact at the given velocity.
 *
 * @param velocity Velocity at impact in pixels per second.
 */
void EdgeGlow::OnAbsorb(
    /* [in] */ Int32 velocity)
{
    mState = STATE_ABSORB;
    velocity = Math::Max(MIN_VELOCITY, Math::Abs(velocity));

    mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
    mDuration = 0.1f + (velocity * 0.03f);

    // The edge should always be at least partially visible, regardless
    // of velocity.
    mEdgeAlphaStart = 0.f;
    mEdgeScaleY = mEdgeScaleYStart = 0.f;
    // The glow depends more on the velocity, and therefore starts out
    // nearly invisible.
    mGlowAlphaStart = 0.5f;
    mGlowScaleYStart = 0.f;

    // Factor the velocity by 8. Testing on device shows this works best to
    // reflect the strength of the user's scrolling.
    mEdgeAlphaFinish = Math::Max(0, Math::Min(velocity * VELOCITY_EDGE_FACTOR, 1));
    // Edge should never get larger than the size of its asset.
    mEdgeScaleYFinish = Math::Max(
            HELD_EDGE_SCALE_Y, Math::Min(Float(velocity * VELOCITY_EDGE_FACTOR), 1.f));

    // Growth for the size of the glow should be quadratic to properly
    // respond
    // to a user's scrolling speed. The faster the scrolling speed, the more
    // intense the effect should be for both the size and the saturation.
    mGlowScaleYFinish = Math::Min(0.025f + (velocity * (velocity / 100) * 0.00015f), 1.75f);
    // Alpha should change for the glow as well as size.
    mGlowAlphaFinish = Math::Max(
            mGlowAlphaStart, Math::Min(velocity * VELOCITY_GLOW_FACTOR * .00001f, MAX_ALPHA));
}


/**
 * Draw into the provided canvas. Assumes that the canvas has been rotated
 * accordingly and the size has been set. The effect will be drawn the full
 * width of X=0 to X=width, emitting from Y=0 and extending to some factor <
 * 1.f of height.
 *
 * @param canvas Canvas to draw into
 * @return true if drawing should continue beyond this frame to continue the
 *         animation
 */
Boolean EdgeGlow::Draw(
    /* [in] */ ICanvas* canvas)
{
    Update();

    Int32 edgeHeight, glowHeight;
    mEdge->GetIntrinsicHeight(&edgeHeight);
    mGlow->GetIntrinsicHeight(&glowHeight);

    Float distScale = (Float)mHeight / mWidth;

    mGlow->SetAlpha((Int32)(Math::Max(0.f, Math::Min(mGlowAlpha, 1.f)) * 255));
    // Width of the image should be 3 * the width of the screen.
    // Should start off screen to the left.
    mGlow->SetBounds(-mWidth, 0, mWidth * 2, (Int32) Math::Min(
            glowHeight * mGlowScaleY * distScale * 0.6f, mHeight * MAX_GLOW_HEIGHT));
    mGlow->Draw(canvas);

    mEdge->SetAlpha((Int32)(Math::Max(0.f, Math::Min(mEdgeAlpha, 1.f)) * 255));
    mEdge->SetBounds(0, 0, mWidth, (Int32)(edgeHeight * mEdgeScaleY));
    mEdge->Draw(canvas);

    return mState != STATE_IDLE;
}

void EdgeGlow::Update()
{
    Int64 time = AnimationUtils::CurrentAnimationTimeMillis();
    Float t = Math::Min((time - mStartTime) / mDuration, 1.f);

    Float interp;
    mInterpolator->GetInterpolation(t, &interp);

    mEdgeAlpha = mEdgeAlphaStart + (mEdgeAlphaFinish - mEdgeAlphaStart) * interp;
    mEdgeScaleY = mEdgeScaleYStart + (mEdgeScaleYFinish - mEdgeScaleYStart) * interp;
    mGlowAlpha = mGlowAlphaStart + (mGlowAlphaFinish - mGlowAlphaStart) * interp;
    mGlowScaleY = mGlowScaleYStart + (mGlowScaleYFinish - mGlowScaleYStart) * interp;

    if (t >= 1.f - EPSILON) {
        switch (mState) {
        case STATE_ABSORB:
            {
                mState = STATE_RECEDE;
                mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
                mDuration = RECEDE_TIME;

                mEdgeAlphaStart = mEdgeAlpha;
                mEdgeScaleYStart = mEdgeScaleY;
                mGlowAlphaStart = mGlowAlpha;
                mGlowScaleYStart = mGlowScaleY;

                // After absorb, the glow and edge should fade to nothing.
                mEdgeAlphaFinish = 0.f;
                mEdgeScaleYFinish = 0.f;
                mGlowAlphaFinish = 0.f;
                mGlowScaleYFinish = 0.f;
            }
            break;
        case STATE_PULL:
            {
                mState = STATE_PULL_DECAY;
                mStartTime = AnimationUtils::CurrentAnimationTimeMillis();
                mDuration = PULL_DECAY_TIME;

                mEdgeAlphaStart = mEdgeAlpha;
                mEdgeScaleYStart = mEdgeScaleY;
                mGlowAlphaStart = mGlowAlpha;
                mGlowScaleYStart = mGlowScaleY;

                // After pull, the glow and edge should fade to nothing.
                mEdgeAlphaFinish = 0.f;
                mEdgeScaleYFinish = 0.f;
                mGlowAlphaFinish = 0.f;
                mGlowScaleYFinish = 0.f;
            }
            break;
        case STATE_PULL_DECAY:
            {
                // When receding, we want edge to decrease more slowly
                // than the glow.
                Float factor = mGlowScaleYFinish != 0 ? 1
                    / (mGlowScaleYFinish * mGlowScaleYFinish)
                    : Math::FLOAT_MIN_VALUE;
                mEdgeScaleY = mEdgeScaleYStart +
                    (mEdgeScaleYFinish - mEdgeScaleYStart) *
                    interp * factor;
            }
            break;
        case STATE_RECEDE:
            mState = STATE_IDLE;
            break;
        default:
            break;
        }
    }
}
