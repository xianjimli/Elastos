
#include "ext/frameworkdef.h"
#include "widget/CSeekBar.h"


IVIEW_METHODS_IMPL(CSeekBar, AbsSeekBar, AbsSeekBar);

IDrawableCallback_METHODS_IMPL(CSeekBar, AbsSeekBar, AbsSeekBar);

IKeyEventCallback_METHODS_IMPL(CSeekBar, AbsSeekBar, AbsSeekBar);

IAccessibilityEventSource_METHODS_IMPL(CSeekBar, AbsSeekBar, AbsSeekBar);

ECode CSeekBar::constructor(
    /* [in] */ IContext* ctx)
{
    return constructor(ctx, NULL);
}

ECode CSeekBar::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(ctx, attrs, 0x0101007b); //com.android.internal.R.attr.seekBarStyle
}

ECode CSeekBar::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return AbsSeekBar::Init(ctx, attrs, defStyle);
}

PInterface CSeekBar::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CSeekBar::Probe(riid);
}

ECode CSeekBar::IsIndeterminate(
    /* [out] */ Boolean* isIndeterminate)
{
    VALIDATE_NOT_NULL(isIndeterminate);
    *isIndeterminate = AbsSeekBar::IsIndeterminate();

    return NOERROR;
}

ECode CSeekBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    return AbsSeekBar::SetIndeterminate(indeterminate);
}

ECode CSeekBar::GetIndeterminateDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> _drawable = AbsSeekBar::GetIndeterminateDrawable();
    *drawable = _drawable;
    if (*drawable) (*drawable)->AddRef();

    return NOERROR;
}

ECode CSeekBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable* drawable)
{
    return AbsSeekBar::SetIndeterminateDrawable(drawable);
}

ECode CSeekBar::GetProgressDrawable(
    /* [out] */ IDrawable ** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> _drawable = AbsSeekBar::GetProgressDrawable();
    *drawable = _drawable;
    if (*drawable) (*drawable)->AddRef();

    return NOERROR;
}

ECode CSeekBar::SetProgressDrawable(
    /* [in] */ IDrawable * drawable)
{
    return AbsSeekBar::SetProgressDrawable(drawable);
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
    /* [out] */ Int32* progress)
{
    VALIDATE_NOT_NULL(progress);
    *progress = AbsSeekBar::GetProgress();

    return NOERROR;
}

ECode CSeekBar::GetSecondaryProgress(
    /* [out] */ Int32* secProgress)
{
    VALIDATE_NOT_NULL(secProgress);
    *secProgress = AbsSeekBar::GetSecondaryProgress();

    return NOERROR;
}

ECode CSeekBar::GetMax(
    /* [out] */ Int32* max)
{
    VALIDATE_NOT_NULL(max);
    *max = AbsSeekBar::GetMax();

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
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 resID)
{
    return AbsSeekBar::SetInterpolator(ctx, resID);
}

ECode CSeekBar::SetInterpolatorEx(
    /* [in] */ IInterpolator* interpolator)
{
    return AbsSeekBar::SetInterpolator(interpolator);
}

ECode CSeekBar::GetInterpolator(
    /* [out] */ IInterpolator** interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    AutoPtr<IInterpolator> _interpolator = AbsSeekBar::GetInterpolator();
    *interpolator = _interpolator;
    if (*interpolator) (*interpolator)->AddRef();

    return NOERROR;
}

ECode CSeekBar::SetThumb(
    /* [in] */ IDrawable* thumb)
{
    return AbsSeekBar::SetThumb(thumb);
}

ECode CSeekBar::GetThumbOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = AbsSeekBar::GetThumbOffset();

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
    /* [out] */ Int32* increment)
{
    VALIDATE_NOT_NULL(increment);
    *increment = AbsSeekBar::GetKeyProgressIncrement();

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
