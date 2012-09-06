
#ifndef __CANIMATIONDRAWABLE_H__
#define __CANIMATIONDRAWABLE_H__

#include "_CAnimationDrawable.h"
#include "graphics/drawable/AnimationDrawable.h"
#include "graphics/drawable/DrawableMacro.h"
#include "view/ViewMacro.h"

CarClass(CAnimationDrawable), public AnimationDrawable
{
public:
    IDRAWABLE_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    /**
     * @return The number of frames in the animation
     */
    CARAPI GetNumberOfFrames(
        /* [out] */ Int32* number);

    /**
     * @return The Drawable at the specified frame index
     */
    CARAPI GetFrame(
        /* [in] */ Int32 index,
        /* [out] */ IDrawable** drawable);

    /**
     * @return The duration in milliseconds of the frame at the
     * specified index
     */
    CARAPI GetDuration(
        /* [in] */ Int32 i,
        /* [out] */ Int32* duration);

    /**
     * @return True of the animation will play once, false otherwise
     */
    CARAPI IsOneShot(
        /* [out] */ Boolean* result);

    /**
     * Sets whether the animation should play once or repeat.
     *
     * @param oneShot Pass true if the animation should only play once
     */
    CARAPI SetOneShot(
        /* [in] */ Boolean oneShot);

    /**
     * Add a frame to the animation
     *
     * @param frame The frame to add
     * @param duration How long in milliseconds the frame should appear
     */
    CARAPI AddFrame(
        /* [in] */ IDrawable* frame,
        /* [in] */ Int32 duration);

    CARAPI Start();

    CARAPI Stop();

    CARAPI IsRunning(
        /* [out] */ Boolean* isRunning);

    CARAPI Run();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    //@Override
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

#endif //__CANIMATIONDRAWABLE_H__
