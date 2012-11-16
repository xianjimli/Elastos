
#include "ext/frameworkdef.h"
#include "widget/CAnalogClock.h"

IVIEW_METHODS_IMPL(CAnalogClock, AnalogClock, AnalogClock);

IDrawableCallback_METHODS_IMPL(CAnalogClock, AnalogClock, AnalogClock);

IKeyEventCallback_METHODS_IMPL(CAnalogClock, AnalogClock, AnalogClock);

IAccessibilityEventSource_METHODS_IMPL(CAnalogClock, AnalogClock, AnalogClock);

PInterface CAnalogClock::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CAnalogClock::Probe(riid);
}

ECode CAnalogClock::constructor(
    /* [in] */ IContext * pCtx)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAnalogClock::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAnalogClock::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

