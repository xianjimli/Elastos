
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
    assert(ppHolder != NULL);
    *ppHolder = SurfaceView::GetHolder();
    if (*ppHolder != NULL) {
        (*ppHolder)->AddRef();
    }

    return E_NOT_IMPLEMENTED;
}

ECode CSurfaceView::SetZOrderMediaOverlay(
    /* [in] */ Boolean isMediaOverlay)
{
    return SurfaceView::SetZOrderMediaOverlay(isMediaOverlay);
}

ECode CSurfaceView::SetZOrderOnTop(
    /* [in] */ Boolean onTop)
{
    return SurfaceView::SetZOrderOnTop(onTop);
}

ECode CSurfaceView::SetWindowType(
    /* [in] */ Int32 type)
{
    return SurfaceView::SetWindowType(type);
}

ECode CSurfaceView::IsFixedSize(
    /* [out] */ Boolean * pSize)
{
    assert(pSize != NULL);
    *pSize = SurfaceView::IsFixedSize();

    return NOERROR;
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext * pContext)
{
    View::Init(pContext);
    SurfaceView::Init();

    return NOERROR;
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs)
{
    View::Init(pContext, pAttrs);
    SurfaceView::Init();

    return NOERROR;
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    View::Init(pContext, pAttrs, defStyle);
    SurfaceView::Init();

    return NOERROR;
}

