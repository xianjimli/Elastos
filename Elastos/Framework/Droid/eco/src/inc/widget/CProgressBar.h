
#ifndef __CPROGRESSBAR_H__
#define __CPROGRESSBAR_H__

#include "_CProgressBar.h"
#include "widget/ProgressBar.h"
#include "view/ViewMacro.h"

CarClass(CProgressBar), public ProgressBar
{
public:
    IVIEW_METHODS_DECL();

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

    CARAPI constructor(
        /* [in] */ IContext * pCtx);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs);

    CARAPI constructor(
        /* [in] */ IContext * pCtx,
        /* [in] */ IAttributeSet * pAttrs,
        /* [in] */ Int32 defStyle);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPROGRESSBAR_H__
