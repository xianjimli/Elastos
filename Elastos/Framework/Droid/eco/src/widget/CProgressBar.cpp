
#include "ext/frameworkdef.h"
#include "widget/CProgressBar.h"

IVIEW_METHODS_IMPL(CProgressBar, ProgressBar, ProgressBar);

IDrawableCallback_METHODS_IMPL(CProgressBar, ProgressBar, ProgressBar);

IKeyEventCallback_METHODS_IMPL(CProgressBar, ProgressBar, ProgressBar);

IAccessibilityEventSource_METHODS_IMPL(CProgressBar, ProgressBar, ProgressBar);

PInterface CProgressBar::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CProgressBar::Probe(riid);
}

ECode CProgressBar::IsIndeterminate(
    /* [out] */ Boolean* indeterminate)
{
    VALIDATE_NOT_NULL(indeterminate);

    *indeterminate = ProgressBar::IsIndeterminate();

    return NOERROR;
}

ECode CProgressBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    return ProgressBar::SetIndeterminate(indeterminate);
}

ECode CProgressBar::GetIndeterminateDrawable(
    /* [out] */ IDrawable** d)
{
    VALIDATE_NOT_NULL(d);

    *d = ProgressBar::GetIndeterminateDrawable();

    return NOERROR;
}

ECode CProgressBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable* d)
{
    return ProgressBar::SetIndeterminateDrawable(d);
}

ECode CProgressBar::GetProgressDrawable(
    /* [out] */ IDrawable** d)
{
    VALIDATE_NOT_NULL(d);

    *d = ProgressBar::GetProgressDrawable();

    return NOERROR;
}

ECode CProgressBar::SetProgressDrawable(
    /* [in] */ IDrawable* d)
{
    return ProgressBar::SetProgressDrawable(d);
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
    /* [out] */ Int32 * progress)
{
    VALIDATE_NOT_NULL(progress);

    *progress = ProgressBar::GetProgress();

    return NOERROR;
}

ECode CProgressBar::GetSecondaryProgress(
    /* [out] */ Int32 * secondaryProgress)
{
    VALIDATE_NOT_NULL(secondaryProgress);

    *secondaryProgress = ProgressBar::GetSecondaryProgress();

    return NOERROR;
}

ECode CProgressBar::GetMax(
    /* [out] */ Int32* max)
{
    VALIDATE_NOT_NULL(max);

    *max = ProgressBar::GetMax();

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
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 resID)
{
    return ProgressBar::SetInterpolator(ctx, resID);
}

ECode CProgressBar::SetInterpolatorEx(
    /* [in] */ IInterpolator* interpolator)
{
    return ProgressBar::SetInterpolator(interpolator);
}

ECode CProgressBar::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);

    *interpolator = ProgressBar::GetInterpolator();

    return NOERROR;
}

ECode CProgressBar::constructor(
    /* [in] */ IContext* ctx)
{
    ProgressBar::Init(ctx);

    return NOERROR;
}

ECode CProgressBar::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    ProgressBar::Init(ctx, attrs);

    return NOERROR;
}

ECode CProgressBar::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ProgressBar::Init(ctx, attrs, defStyle);

    return NOERROR;
}

Mutex* CProgressBar::GetSelfLock()
{
    return &_m_syncLock;
}
