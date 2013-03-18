
#include "ext/frameworkdef.h"
#include "widget/CSeekBar.h"

IVIEW_METHODS_IMPL(CSeekBar, AbsSeekBar, AbsSeekBar);

IDrawableCallback_METHODS_IMPL(CSeekBar, AbsSeekBar, AbsSeekBar);

IKeyEventCallback_METHODS_IMPL(CSeekBar, AbsSeekBar, AbsSeekBar);

IAccessibilityEventSource_METHODS_IMPL(CSeekBar, AbsSeekBar, AbsSeekBar);

PInterface CSeekBar::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CSeekBar::Probe(riid);
}

ECode CSeekBar::IsIndeterminate(
    /* [out] */ Boolean * pIndeterminate)
{
    VALIDATE_NOT_NULL(pIndeterminate);
    *pIndeterminate = AbsSeekBar::IsIndeterminate();

    return NOERROR;
}

ECode CSeekBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    return AbsSeekBar::SetIndeterminate(indeterminate);
}

ECode CSeekBar::GetIndeterminateDrawable(
    /* [out] */ IDrawable ** ppD)
{
    VALIDATE_NOT_NULL(ppD);
    AutoPtr<IDrawable> drawable = AbsSeekBar::GetIndeterminateDrawable();
    *ppD = drawable.Get();
    if (*ppD) (*ppD)->AddRef();

    return NOERROR;
}

ECode CSeekBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable * pD)
{
    return AbsSeekBar::SetIndeterminateDrawable(pD);
}

ECode CSeekBar::GetProgressDrawable(
    /* [out] */ IDrawable ** ppD)
{
    VALIDATE_NOT_NULL(ppD);
    AutoPtr<IDrawable> drawable = AbsSeekBar::GetProgressDrawable();
    *ppD = drawable.Get();
    if (*ppD) (*ppD)->AddRef();

    return NOERROR;
}

ECode CSeekBar::SetProgressDrawable(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return AbsSeekBar::SetProgressDrawable(pD);
}

ECode CSeekBar::SetProgress(
    /* [in] */ Int32 progress)
{
    return AbsSeekBar::SetProgress(progress);
}

ECode CSeekBar::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    return AbsSeekBar::SetSecondaryProgress(secondaryProgress);
}

ECode CSeekBar::GetProgress(
    /* [out] */ Int32 * pProgress)
{
    VALIDATE_NOT_NULL(pProgress);
    *pProgress = AbsSeekBar::GetProgress();

    return NOERROR;
}

ECode CSeekBar::GetSecondaryProgress(
    /* [out] */ Int32 * pSecondaryProgress)
{
    VALIDATE_NOT_NULL(pSecondaryProgress);
    *pSecondaryProgress = AbsSeekBar::GetSecondaryProgress();

    return NOERROR;
}

ECode CSeekBar::GetMax(
    /* [out] */ Int32 * pMax)
{
    VALIDATE_NOT_NULL(pMax);
    *pMax = AbsSeekBar::GetMax();

    return NOERROR;
}

ECode CSeekBar::SetMax(
    /* [in] */ Int32 max)
{
    return AbsSeekBar::SetMax(max);
}

ECode CSeekBar::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    return AbsSeekBar::IncrementProgressBy(diff);
}

ECode CSeekBar::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    return AbsSeekBar::IncrementSecondaryProgressBy(diff);
}

ECode CSeekBar::SetInterpolator(
    /* [in] */ IContext * pCtx,
    /* [in] */ Int32 resID)
{
    return AbsSeekBar::SetInterpolator(pCtx, resID);
}

ECode CSeekBar::SetInterpolatorEx(
    /* [in] */ IInterpolator * pInterpolator)
{
    return AbsSeekBar::SetInterpolator(pInterpolator);
}

ECode CSeekBar::GetInterpolator(
    /* [out] */ IInterpolator ** ppInterpolator)
{
    VALIDATE_NOT_NULL(ppInterpolator);
    AutoPtr<IInterpolator> interpolator = AbsSeekBar::GetInterpolator();
    *ppInterpolator = interpolator.Get();
    if (*ppInterpolator) (*ppInterpolator)->AddRef();

    return NOERROR;
}

ECode CSeekBar::SetThumb(
    /* [in] */ IDrawable * pThumb)
{
    return AbsSeekBar::SetThumb(pThumb);
}

ECode CSeekBar::GetThumbOffset(
    /* [out] */ Int32 * pOffset)
{
    VALIDATE_NOT_NULL(pOffset);
    *pOffset = AbsSeekBar::GetThumbOffset();

    return NOERROR;
}

ECode CSeekBar::SetThumbOffset(
    /* [in] */ Int32 thumbOffset)
{
    return AbsSeekBar::SetThumbOffset(thumbOffset);
}

ECode CSeekBar::SetKeyProgressIncrement(
    /* [in] */ Int32 increment)
{
    return AbsSeekBar::SetKeyProgressIncrement(increment);
}

ECode CSeekBar::GetKeyProgressIncrement(
    /* [out] */ Int32 * pIncrement)
{
    VALIDATE_NOT_NULL(pIncrement);
    *pIncrement = AbsSeekBar::GetKeyProgressIncrement();

    return NOERROR;
}

Mutex* CSeekBar::GetSelfLock()
{
    return &_m_syncLock;
}

ECode CSeekBar::SetOnSeekBarChangeListener(
    /* [in] */ IOnSeekBarChangeListener* l)
{
    mOnSeekBarChangeListener = l;

    return NOERROR;
}

ECode CSeekBar::constructor(
    /* [in] */ IContext * pCtx)
{
    return constructor(pCtx, NULL);
}

ECode CSeekBar::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    return constructor(pCtx, pAttrs, 0x0101007b); //com.android.internal.R.attr.seekBarStyle
}

ECode CSeekBar::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    return AbsSeekBar::Init(pCtx, pAttrs, defStyle);
}

void CSeekBar::OnStartTrackingTouch()
{
    if (mOnSeekBarChangeListener != NULL) {
        mOnSeekBarChangeListener->OnStartTrackingTouch(this);
    }
}

void CSeekBar::OnStopTrackingTouch()
{
    if (mOnSeekBarChangeListener != NULL) {
        mOnSeekBarChangeListener->OnStopTrackingTouch(this);
    }
}

 void CSeekBar::OnProgressRefresh(
    /* [in] */ Float scale,
    /* [in] */ Boolean fromUser)
{
    AbsSeekBar::OnProgressRefresh(scale, fromUser);

    if (mOnSeekBarChangeListener != NULL) {
        mOnSeekBarChangeListener->OnProgressChanged(
                this, AbsSeekBar::GetProgress(), fromUser);
    }
}
