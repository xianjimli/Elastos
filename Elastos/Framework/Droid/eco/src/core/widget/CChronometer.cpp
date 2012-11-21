
#include "widget/CChronometer.h"

IVIEW_METHODS_IMPL(CChronometer, Chronometer, Chronometer);

IDrawableCallback_METHODS_IMPL(CChronometer, Chronometer, Chronometer);

IKeyEventCallback_METHODS_IMPL(CChronometer, Chronometer, Chronometer);

IAccessibilityEventSource_METHODS_IMPL(CChronometer, Chronometer, Chronometer);

ITEXTVIEW_METHODS_IMPL(CChronometer, Chronometer, Chronometer);

PInterface CChronometer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CChronometer::Probe(riid);
}

ECode CChronometer::SetBase(
    /* [in] */ Int64 base)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::GetBase(
    /* [out] */ Int64 * pBase)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::SetFormat(
    /* [in] */ const String& format)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::GetFormat(
    /* [out] */ String * pFormat)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::SetOnChronometerTickListener(
    /* [in] */ IOnChronometerTickListener * pListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::GetOnChronometerTickListener(
    /* [out] */ IOnChronometerTickListener ** ppListener)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::Start()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::Stop()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::SetStarted(
    /* [in] */ Boolean started)
{

    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::OnPreDraw(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CChronometer::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

