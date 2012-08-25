
#ifndef __CPROGRESSBAR_H__
#define __CPROGRESSBAR_H__

#include "_CProgressBar.h"
#include "widget/ProgressBar.h"
#include "view/ViewMacro.h"

CarClass(CProgressBar), public ProgressBar
{
public:
    IVIEW_METHODS_DECL();

    IDrawableCallback_METHODS_DECL();

    IKeyEventCallback_METHODS_DECL();

    IAccessibilityEventSource_METHODS_DECL();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI IsIndeterminate(
        /* [out] */ Boolean* indeterminate);

    CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    CARAPI GetIndeterminateDrawable(
        /* [out] */ IDrawable** d);

    CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable* d);

    CARAPI GetProgressDrawable(
        /* [out] */ IDrawable** d);

    CARAPI SetProgressDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetProgress(
        /* [in] */ Int32 progress);

    CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    CARAPI GetProgress(
        /* [out] */ Int32* progress);

    CARAPI GetSecondaryProgress(
        /* [out] */ Int32* secondaryProgress);

    CARAPI GetMax(
        /* [out] */ Int32* max);

    CARAPI SetMax(
        /* [in] */ Int32 max);

    CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    CARAPI SetInterpolator(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 resID);

    CARAPI SetInterpolatorEx(
        /* [in] */ IInterpolator* interpolator);

    CARAPI GetInterpolator(
        /* [out] */ IInterpolator** interpolator);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(Mutex*) GetSelfLock();

private:
    // TODO: Add your private member variables here.
};

#endif // __CPROGRESSBAR_H__
