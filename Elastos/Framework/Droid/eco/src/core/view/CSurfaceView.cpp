
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
    /* [out] */ ISurfaceHolder** holder)
{
    assert(holder != NULL);
    AutoPtr<ISurfaceHolder> _holder = SurfaceView::GetHolder();
    *holder = _holder;
    if (*holder != NULL) {
        (*holder)->AddRef();
    }

    return NOERROR;
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
    /* [out] */ Boolean* size)
{
    assert(size != NULL);
    *size = SurfaceView::IsFixedSize();

    return NOERROR;
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext* context)
{
    return SurfaceView::Init(context);
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return SurfaceView::Init(context, attrs);
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return SurfaceView::Init(context, attrs, defStyle);
}
