
#include "view/CSurfaceView.h"

IVIEW_METHODS_IMPL(CSurfaceView, SurfaceView, SurfaceView);

IDrawableCallback_METHODS_IMPL(CSurfaceView, SurfaceView, SurfaceView);

IKeyEventCallback_METHODS_IMPL(CSurfaceView, SurfaceView, SurfaceView);

IAccessibilityEventSource_METHODS_IMPL(CSurfaceView, SurfaceView, SurfaceView);

PInterface CSurfaceView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CSurfaceView::Probe(riid);
}

ECode CSurfaceView::GetHolder(
    /* [out] */ ISurfaceHolder ** ppHolder)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceView::SetZOrderMediaOverlay(
    /* [in] */ Boolean isMediaOverlay)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceView::SetZOrderOnTop(
    /* [in] */ Boolean onTop)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceView::SetWindowType(
    /* [in] */ Int32 type)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceView::IsFixedSize(
    /* [out] */ Boolean * pSize)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

