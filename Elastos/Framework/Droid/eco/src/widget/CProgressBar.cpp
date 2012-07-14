
#include "widget/CProgressBar.h"

IVIEW_METHODS_IMPL(CProgressBar, ProgressBar, ProgressBar);

PInterface CProgressBar::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CProgressBar::Probe(riid);
}

ECode CProgressBar::IsIndeterminate(
    /* [out] */ Boolean * pIndeterminate)
{
    if (!pIndeterminate) {
        return E_INVALID_ARGUMENT;
    }

    *pIndeterminate = ProgressBar::IsIndeterminate();

    return NOERROR;
}

ECode CProgressBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    return ProgressBar::SetIndeterminate(indeterminate);
}

ECode CProgressBar::GetIndeterminateDrawable(
    /* [out] */ IDrawable ** ppD)
{
    if (!ppD) {
        return E_INVALID_ARGUMENT;
    }

    *ppD = ProgressBar::GetIndeterminateDrawable();
    
    return NOERROR;
}

ECode CProgressBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable * pD)
{
    return ProgressBar::SetIndeterminateDrawable(pD);
}

ECode CProgressBar::GetProgressDrawable(
    /* [out] */ IDrawable ** ppD)
{
    if (!ppD) {
        return E_INVALID_ARGUMENT;
    }

    *ppD = ProgressBar::GetProgressDrawable();

    return NOERROR;
}

ECode CProgressBar::SetProgressDrawable(
    /* [in] */ IDrawable * pD)
{
    return ProgressBar::SetProgressDrawable(pD);
}

ECode CProgressBar::SetProgress(
    /* [in] */ Int32 progress)
{
    return ProgressBar::SetProgress(progress);
}

ECode CProgressBar::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    return ProgressBar::SetSecondaryProgress(secondaryProgress);
}

ECode CProgressBar::GetProgress(
    /* [out] */ Int32 * pProgress)
{
    if (!pProgress) {
        return E_INVALID_ARGUMENT;
    }

    *pProgress = ProgressBar::GetProgress();

    return NOERROR;
}

ECode CProgressBar::GetSecondaryProgress(
    /* [out] */ Int32 * pSecondaryProgress)
{
    if (!pSecondaryProgress) {
        return E_INVALID_ARGUMENT;
    }

    *pSecondaryProgress = ProgressBar::GetSecondaryProgress();

    return NOERROR;
}

ECode CProgressBar::GetMax(
    /* [out] */ Int32 * pMax)
{
    if (!pMax) {
        return E_INVALID_ARGUMENT;
    }

    *pMax = ProgressBar::GetMax();

    return NOERROR;
}

ECode CProgressBar::SetMax(
    /* [in] */ Int32 max)
{
    return ProgressBar::SetMax(max);
}

ECode CProgressBar::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    return ProgressBar::IncrementProgressBy(diff);
}

ECode CProgressBar::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    return ProgressBar::IncrementSecondaryProgressBy(diff);
}

ECode CProgressBar::SetInterpolator(
    /* [in] */ IContext * pCtx,
    /* [in] */ Int32 resID)
{
    return ProgressBar::SetInterpolator(pCtx, resID);
}

ECode CProgressBar::SetInterpolatorEx(
    /* [in] */ IInterpolator * pInterpolator)
{
    return ProgressBar::SetInterpolator(pInterpolator);
}

ECode CProgressBar::GetInterpolator(
    /* [out] */ IInterpolator ** ppInterpolator)
{
    if (ppInterpolator) {
        return E_INVALID_ARGUMENT;
    }

    *ppInterpolator = ProgressBar::GetInterpolator();

    return NOERROR;
}

ECode CProgressBar::constructor(
    /* [in] */ IContext * pCtx)
{
    ProgressBar::Init(pCtx);

    return NOERROR;
}

ECode CProgressBar::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    ProgressBar::Init(pCtx, pAttrs);

    return NOERROR;
}

ECode CProgressBar::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    ProgressBar::Init(pCtx, pAttrs, defStyle);

    return NOERROR;
}

