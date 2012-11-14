
#include "ext/frameworkdef.h"
#include "widget/CAbsSeekBar.h"

IVIEW_METHODS_IMPL(CAbsSeekBar, AbsSeekBar, AbsSeekBar);

IDrawableCallback_METHODS_IMPL(CAbsSeekBar, AbsSeekBar, AbsSeekBar);

IKeyEventCallback_METHODS_IMPL(CAbsSeekBar, AbsSeekBar, AbsSeekBar);

IAccessibilityEventSource_METHODS_IMPL(CAbsSeekBar, AbsSeekBar, AbsSeekBar);

PInterface CAbsSeekBar::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CAbsSeekBar::Probe(riid);
}

ECode CAbsSeekBar::IsIndeterminate(
    /* [out] */ Boolean * pIndeterminate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetIndeterminate(
    /* [in] */ Boolean indeterminate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::GetIndeterminateDrawable(
    /* [out] */ IDrawable ** ppD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetIndeterminateDrawable(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::GetProgressDrawable(
    /* [out] */ IDrawable ** ppD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetProgressDrawable(
    /* [in] */ IDrawable * pD)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetProgress(
    /* [in] */ Int32 progress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetSecondaryProgress(
    /* [in] */ Int32 secondaryProgress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::GetProgress(
    /* [out] */ Int32 * pProgress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::GetSecondaryProgress(
    /* [out] */ Int32 * pSecondaryProgress)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::GetMax(
    /* [out] */ Int32 * pMax)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetMax(
    /* [in] */ Int32 max)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::IncrementProgressBy(
    /* [in] */ Int32 diff)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::IncrementSecondaryProgressBy(
    /* [in] */ Int32 diff)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetInterpolator(
    /* [in] */ IContext * pCtx,
    /* [in] */ Int32 resID)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetInterpolatorEx(
    /* [in] */ IInterpolator * pInterpolator)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::GetInterpolator(
    /* [out] */ IInterpolator ** ppInterpolator)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetThumb(
    /* [in] */ IDrawable * pThumb)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::GetThumbOffset(
    /* [out] */ Int32 * pOffset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetThumbOffset(
    /* [in] */ Int32 thumbOffset)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::SetKeyProgressIncrement(
    /* [in] */ Int32 increment)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::GetKeyProgressIncrement(
    /* [out] */ Int32 * pIncrement)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAbsSeekBar::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

Mutex* CAbsSeekBar::GetSelfLock()
{
    return &_m_syncLock;
}

