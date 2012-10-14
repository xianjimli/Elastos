
#ifndef __ANIMATIONUTILS_H__
#define __ANIMATIONUTILS_H__

#include "ext/frameworkext.h"

/**
 * Defines common utilities for working with animations.
 *
 */
class AnimationUtils
{
public:
    /**
     * Returns the current animation time in milliseconds. This time should be used when invoking
     * {@link Animation#setStartTime(long)}. Refer to {@link android.os.SystemClock} for more
     * information about the different available clocks. The clock used by this method is
     * <em>not</em> the "wall" clock (it is not {@link System#currentTimeMillis}).
     *
     * @return the current animation time in milliseconds
     *
     * @see android.os.SystemClock
     */
    static CARAPI_(Int64) CurrentAnimationTimeMillis();

    /**
     * Loads an {@link Animation} object from a resource
     *
     * @param context Application context used to access resources
     * @param id The resource id of the animation to load
     * @return The animation object reference by the specified id
     * @throws NotFoundException when the animation cannot be loaded
     */
    static CARAPI LoadAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ IAnimation** animation);

    static CARAPI LoadLayoutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ ILayoutAnimationController** controller);

    /**
     * Make an animation for objects becoming visible. Uses a slide and fade
     * effect.
     *
     * @param c Context for loading resources
     * @param fromLeft is the object to be animated coming from the left
     * @return The new animation
     */
    static CARAPI MakeInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Boolean fromLeft,
        /* [out] */ IAnimation** animation);

    /**
     * Make an animation for objects becoming invisible. Uses a slide and fade
     * effect.
     *
     * @param c Context for loading resources
     * @param toRight is the object to be animated exiting to the right
     * @return The new animation
     */
    static CARAPI MakeOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Boolean toRight,
        /* [out] */ IAnimation** animation);

    /**
     * Make an animation for objects becoming visible. Uses a slide up and fade
     * effect.
     *
     * @param c Context for loading resources
     * @return The new animation
     */
    static CARAPI MakeInChildBottomAnimation(
        /* [in] */ IContext* context,
        /* [out] */ IAnimation** animation);

    /**
     * Loads an {@link Interpolator} object from a resource
     *
     * @param context Application context used to access resources
     * @param id The resource id of the animation to load
     * @return The animation object reference by the specified id
     * @throws NotFoundException
     */
    static CARAPI LoadInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ IInterpolator** interpolator);

private:
    static CARAPI CreateAnimationFromXml(
        /* [in] */ IContext* c,
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ IAnimation** animation);

    static CARAPI CreateAnimationFromXml(
        /* [in] */ IContext* c,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAnimationSet* parent,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IAnimation** animation);

    static CARAPI CreateLayoutAnimationFromXml(
        /* [in] */ IContext* c,
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ ILayoutAnimationController** controller);

    static CARAPI CreateLayoutAnimationFromXml(
        /* [in] */ IContext* c,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ ILayoutAnimationController** controller);

    static CARAPI CreateInterpolatorFromXml(
        /* [in] */ IContext* c,
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ IInterpolator** interpolator);
};

#endif //__ANIMATIONUTILS_H__
