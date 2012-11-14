
#ifndef __CABSSEEKBAR_H__
#define __CABSSEEKBAR_H__

#include "_CAbsSeekBar.h"
#include "widget/AbsSeekBar.h"
#include "view/ViewMacro.h"

CarClass(CAbsSeekBar), public AbsSeekBar
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI IsIndeterminate(
        /* [out] */ Boolean * pIndeterminate);

    CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    CARAPI GetIndeterminateDrawable(
        /* [out] */ IDrawable ** ppD);

    CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable * pD);

    CARAPI GetProgressDrawable(
        /* [out] */ IDrawable ** ppD);

    CARAPI SetProgressDrawable(
        /* [in] */ IDrawable * pD);

    CARAPI SetProgress(
        /* [in] */ Int32 progress);

    CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    CARAPI GetProgress(
        /* [out] */ Int32 * pProgress);

    CARAPI GetSecondaryProgress(
        /* [out] */ Int32 * pSecondaryProgress);

    CARAPI GetMax(
        /* [out] */ Int32 * pMax);

    CARAPI SetMax(
        /* [in] */ Int32 max);

    CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    CARAPI SetInterpolator(
        /* [in] */ IContext * pCtx,
        /* [in] */ Int32 resID);

    CARAPI SetInterpolatorEx(
        /* [in] */ IInterpolator * pInterpolator);

    CARAPI GetInterpolator(
        /* [out] */ IInterpolator ** ppInterpolator);

    CARAPI SetThumb(
        /* [in] */ IDrawable * pThumb);

    CARAPI GetThumbOffset(
        /* [out] */ Int32 * pOffset);

    CARAPI SetThumbOffset(
        /* [in] */ Int32 thumbOffset);

    CARAPI SetKeyProgressIncrement(
        /* [in] */ Int32 increment);

    CARAPI GetKeyProgressIncrement(
        /* [out] */ Int32 * pIncrement);

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(Mutex*) GetSelfLock();

private:
    // TODO: Add your private member variables here.
};

#endif // __CABSSEEKBAR_H__
