
#ifndef __EDGEGLOW_H__
#define __EDGEGLOW_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * This class performs the glow effect used at the edges of scrollable widgets.
 * @hide
 */
class EdgeGlow
{
private:
    static const char* TAG;

    // Time it will take the effect to fully recede in ms
    static const Int32 RECEDE_TIME = 1000;

    // Time it will take before a pulled glow begins receding
    static const Int32 PULL_TIME = 167;

    // Time it will take for a pulled glow to decay to partial strength before release
    static const Int32 PULL_DECAY_TIME = 1000;

    static const Float MAX_ALPHA = 0.8f;
    static const Float HELD_EDGE_ALPHA = 0.7f;
    static const Float HELD_EDGE_SCALE_Y = 0.5f;
    static const Float HELD_GLOW_ALPHA = 0.5f;
    static const Float HELD_GLOW_SCALE_Y = 0.5f;

    static const Float MAX_GLOW_HEIGHT = 3.f;

    static const Float PULL_GLOW_BEGIN = 1.f;
    static const Float PULL_EDGE_BEGIN = 0.6f;

    // Minimum velocity that will be absorbed
    static const Int32 MIN_VELOCITY = 100;

    static const Float EPSILON = 0.001f;

    static const Int32 STATE_IDLE = 0;
    static const Int32 STATE_PULL = 1;
    static const Int32 STATE_ABSORB = 2;
    static const Int32 STATE_RECEDE = 3;
    static const Int32 STATE_PULL_DECAY = 4;

    // How much dragging should effect the height of the edge image.
    // Number determined by user testing.
    static const Int32 PULL_DISTANCE_EDGE_FACTOR = 5;

    // How much dragging should effect the height of the glow image.
    // Number determined by user testing.
    static const Int32 PULL_DISTANCE_GLOW_FACTOR = 5;
    static const Float PULL_DISTANCE_ALPHA_GLOW_FACTOR = 0.8f;

    static const Int32 VELOCITY_EDGE_FACTOR = 8;
    static const Int32 VELOCITY_GLOW_FACTOR = 16;

public:
    EdgeGlow(
        /* [in] */ IDrawable* edge,
        /* [in] */ IDrawable* glow);

    CARAPI_(void) SetSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(Boolean) IsFinished();

    CARAPI_(void) Finish();

    CARAPI_(void) OnPull(
        /* [in] */ Float deltaDistance);

    CARAPI_(void) OnRelease();

    CARAPI_(void) OnAbsorb(
        /* [in] */ Int32 velocity);

    CARAPI_(Boolean) Draw(
        /* [in] */ ICanvas* canvas);

private:
    CARAPI_(void) Update();

private:
    AutoPtr<IDrawable> mEdge;
    AutoPtr<IDrawable> mGlow;
    Int32 mWidth;
    Int32 mHeight;

    Float mEdgeAlpha;
    Float mEdgeScaleY;
    Float mGlowAlpha;
    Float mGlowScaleY;

    Float mEdgeAlphaStart;
    Float mEdgeAlphaFinish;
    Float mEdgeScaleYStart;
    Float mEdgeScaleYFinish;
    Float mGlowAlphaStart;
    Float mGlowAlphaFinish;
    Float mGlowScaleYStart;
    Float mGlowScaleYFinish;

    Int64 mStartTime;
    Float mDuration;

    AutoPtr<IInterpolator> mInterpolator;

    Int32 mState;

    Float mPullDistance;
};

#endif //__EDGEGLOW_H__
