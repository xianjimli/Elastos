
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

extern "C" const InterfaceID EIID_Animation;

class Animation
{
    friend class AnimationSet;

public:
    /**
     * Utility class to parse a string description of a size.
     */
    class Description : public ElRefBase
    {
    public:
        static CARAPI_(AutoPtr<Description>) ParseValue(
            /* [in] */ ITypedValue* value);

    public:
        /**
        * One of Animation.ABSOLUTE, Animation.RELATIVE_TO_SELF, or
        * Animation.RELATIVE_TO_PARENT.
        */
        Int32 mType;

        /**
        * The absolute or relative dimension for this Description.
        */
        Float mValue;
    };

public:
    Animation();

    Animation(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI Reset();

    virtual CARAPI Cancel();

    virtual CARAPI Detach();

    virtual CARAPI_(Boolean) IsInitialized();

    virtual CARAPI Initialize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 parentWidth,
        /* [in] */ Int32 parentHeight);

    virtual CARAPI SetInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resID);

    virtual CARAPI SetInterpolator(
        /* [in] */ IInterpolator* i);

    virtual CARAPI SetStartOffset(
        /* [in] */ Int64 startOffset);

    virtual CARAPI SetDuration(
        /* [in] */ Int64 durationMillis);

    virtual CARAPI RestrictDuration(
        /* [in] */ Int64 durationMillis);

    virtual CARAPI ScaleCurrentDuration(
        /* [in] */ Float scale);

    virtual CARAPI SetStartTime(
        /* [in] */ Int64 startTimeMillis);

    virtual CARAPI Start();

    virtual CARAPI StartNow();

    virtual CARAPI SetRepeatMode(
        /* [in] */ Int32 repeatMode);

    virtual CARAPI SetRepeatCount(
        /* [in] */ Int32 repeatCount);

    virtual CARAPI_(Boolean) IsFillEnabled();

    virtual CARAPI SetFillEnabled(
        /* [in] */ Boolean fillEnabled);

    virtual CARAPI SetFillBefore(
        /* [in] */ Boolean fillBefore);

    virtual CARAPI SetFillAfter(
        /* [in] */ Boolean fillAfter);

    virtual CARAPI SetZAdjustment(
        /* [in] */ Int32 zAdjustment);

    virtual CARAPI SetDetachWallpaper(
        /* [in] */ Boolean detachWallpaper);

    virtual CARAPI_(AutoPtr<IInterpolator>) GetInterpolator();

    virtual CARAPI_(Int64) GetStartTime();

    virtual CARAPI_(Int64) GetDuration();

    virtual CARAPI_(Int64) GetStartOffset();

    virtual CARAPI_(Int32) GetRepeatMode();

    virtual CARAPI_(Int32) GetRepeatCount();

    virtual CARAPI_(Boolean) GetFillBefore();

    virtual CARAPI_(Boolean) GetFillAfter();

    virtual CARAPI_(Int32) GetZAdjustment();

    virtual CARAPI_(Boolean) GetDetachWallpaper();

    virtual CARAPI_(Boolean) WillChangeTransformationMatrix();

    virtual CARAPI_(Boolean) WillChangeBounds();

    virtual CARAPI SetAnimationListener(
        /* [in] */ IAnimationListener* listener);

    virtual CARAPI_(Int64) ComputeDurationHint();

    virtual CARAPI_(Boolean) GetTransformation(
        /* [in] */ Int64 currentTime,
        /* [in] */ ITransformation* outTransformation);

    virtual CARAPI_(Boolean) HasStarted();

    virtual CARAPI_(Boolean) HasEnded();

    virtual CARAPI GetInvalidateRegion(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [in] */ IRectF* invalidate,
        /* [in] */ ITransformation* transformation);

    virtual CARAPI InitializeInvalidateRegion(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

protected:
    //protected Animation Clone();

    virtual CARAPI_(void) EnsureInterpolator();

    virtual CARAPI_(void) ApplyTransformation(
        /* [in] */ Float interpolatedTime,
        /* [in] */ ITransformation* t);

    virtual CARAPI_(Float) ResolveSize(
        /* [in] */ Int32 type,
        /* [in] */ Float value,
        /* [in] */ Int32 size,
        /* [in] */ Int32 parentSize);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

protected:
    /**
     * Set by {@link #getTransformation(Int64, Transformation)} when the animation ends.
     */
    Boolean mEnded;

    /**
     * Set by {@link #getTransformation(Int64, Transformation)} when the animation starts.
     */
    Boolean mStarted;

    /**
     * Set by {@link #getTransformation(Int64, Transformation)} when the animation repeats
     * in REVERSE mode.
     */
    Boolean mCycleFlip;

    /**
     * This value must be Set to TRUE by {@link #initialize(Int32, Int32, Int32, Int32)}. It
     * indicates the animation was successfully initialized and can be played.
     */
    Boolean mInitialized;

    /**
     * Indicates whether the animation transformation should be applied before the
     * animation starts.
     */
    Boolean mFillBefore;

    /**
     * Indicates whether the animation transformation should be applied after the
     * animation ends.
     */
    Boolean mFillAfter;

    /**
     * Indicates whether fillAfter should be taken into account.
     */
    Boolean mFillEnabled;

    /**
     * The time in milliseconds at which the animation must start;
     */
    Int64 mStartTime;

    /**
     * The delay in milliseconds after which the animation must start. When the
     * start offset is > 0, the start time of the animation is startTime + startOffset.
     */
    Int64 mStartOffset;

    /**
     * The duration of one animation cycle in milliseconds.
     */
    Int64 mDuration;

    /**
     * The number of times the animation must repeat. By default, an animation repeats
     * indefinitely.
     */
    Int32 mRepeatCount;

    /**
     * Indicates how many times the animation was repeated.
     */
    Int32 mRepeated;

    /**
     * The behavior of the animation when it repeats. The repeat mode is either
     * {@link #Animation_RESTART} or {@link #REVERSE}.
     *
     */
    Int32 mRepeatMode;

    /**
     * The interpolator used by the animation to smooth the movement.
     */
    AutoPtr<IInterpolator> mInterpolator;

    /**
     * The animation listener to be notified when the animation starts, ends or repeats.
     */
    AutoPtr<IAnimationListener> mListener;

    AutoPtr<IRectF> mPreviousRegion;
    AutoPtr<IRectF> mRegion;
    AutoPtr<ITransformation> mTransformation;
    AutoPtr<ITransformation> mPreviousTransformation;

private:
    /**
     * Desired Z order mode during animation.
     */
    Int32 mZAdjustment;

    /**
     * Don't animate the wallpaper.
     */
    Boolean mDetachWallpaper;

    Boolean mMore;
    Boolean mOneMoreTime;
};
#endif //__ANIMATION_H__
