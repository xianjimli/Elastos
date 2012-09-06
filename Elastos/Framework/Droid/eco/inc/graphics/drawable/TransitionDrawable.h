
#ifndef __TRANSITIONDRAWABLE_H__
#define __TRANSITIONDRAWABLE_H__


#include "ext/frameworkext.h"
#include "graphics/drawable/LayerDrawable.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>


class TransitionDrawable : public LayerDrawable
{
public:
    class TransitionState : public LayerState
    {
    public:
        TransitionState(
            /* [in] */ TransitionState* orig,
            /* [in] */ TransitionDrawable* owner,
            /* [in] */ IResources* res);

        //@Override
        CARAPI NewDrawable(
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI NewDrawableEx(
            /* [in] */ IResources* res,
            /* [out] */ IDrawable** drawable);

        //@Override
        CARAPI GetChangingConfigurations(
            /* [out] */ Int32* config);
    };

public:
    /**
     * Create a new transition drawable with the specified list of layers. At least
     * 2 layers are required for this drawable to work properly.
     */
    TransitionDrawable(
        /* [in] */ IObjectContainer* layers);

    /**
     * Create a new transition drawable with no layer. To work correctly, at least 2
     * layers must be added to this drawable.
     *
     * @see #TransitionDrawable(Drawable[])
     */
    TransitionDrawable();

    /**
     * Begin the second layer on top of the first layer.
     *
     * @param durationMillis The length of the transition in milliseconds
     */
    CARAPI StartTransition(
        /* [in] */ Int32 durationMillis);

    /**
     * Show only the first layer.
     */
    CARAPI ResetTransition();

    /**
     * Reverses the transition, picking up where the transition currently is.
     * If the transition is not currently running, this will start the transition
     * with the specified duration. If the transition is already running, the last
     * known duration will be used.
     *
     * @param duration The duration to use if no transition is running.
     */
    CARAPI ReverseTransition(
        /* [in] */ Int32 duration);

    //@Override
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    /**
     * Enables or disables the cross fade of the drawables. When cross fade
     * is disabled, the first drawable is always drawn opaque. With cross
     * fade enabled, the first drawable is drawn with the opposite alpha of
     * the second drawable. Cross fade is disabled by default.
     *
     * @param enabled True to enable cross fading, false otherwise.
     */
    CARAPI SetCrossFadeEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Indicates whether the cross fade is enabled for this transition.
     *
     * @return True if cross fading is enabled, false otherwise.
     */
    CARAPI_(Boolean) IsCrossFadeEnabled();

protected:
    //@Override
    CARAPI_(AutoPtr<LayerState>) CreateConstantState(
        /* [in] */ LayerState* state,
        /* [in] */ IResources* res);

    CARAPI Init(
        /* [in] */ IObjectContainer* layers);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ TransitionState* state,
        /* [in] */ IResources* res);

    CARAPI Init(
        /* [in] */ TransitionState* state,
        /* [in] */ IObjectContainer* layers);

private:
    TransitionDrawable(
        /* [in] */ TransitionState* state,
        /* [in] */ IResources* res);

    TransitionDrawable(
        /* [in] */ TransitionState* state,
        /* [in] */ IObjectContainer* layers);

private:
    /**
     * A transition is about to start.
     */
    static const Int32 TRANSITION_STARTING = 0;

    /**
     * The transition has started and the animation is in progress
     */
    static const Int32 TRANSITION_RUNNING = 1;

    /**
     * No transition will be applied
     */
    static const Int32 TRANSITION_NONE = 2;

    /**
     * The current state of the transition. One of {@link #TRANSITION_STARTING},
     * {@link #TRANSITION_RUNNING} and {@link #TRANSITION_NONE}
     */
    Int32 mTransitionState;

    Boolean mReverse;
    Int64 mStartTimeMillis;
    Int32 mFrom;
    Int32 mTo;
    Int32 mDuration;
    Int32 mOriginalDuration;
    Int32 mAlpha;
    Boolean mCrossFade;
};

#endif //__TRANSITIONDRAWABLE_H__
