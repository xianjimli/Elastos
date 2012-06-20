
#include "widget/CFrameLayout.h"

IVIEW_METHODS_IMPL(CFrameLayout, FrameLayout, FrameLayout);

IVIEWGROUP_METHODS_IMPL(CFrameLayout, FrameLayout, FrameLayout);

IVIEWPARENT_METHODS_IMPL(CFrameLayout, FrameLayout, FrameLayout);

IVIEWMANAGER_METHODS_IMPL(CFrameLayout, FrameLayout, FrameLayout);

IKeyEventCallback_METHODS_IMPL(CFrameLayout, FrameLayout, FrameLayout);

PInterface CFrameLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CFrameLayout::Probe(riid);
}

// IFrameLayout interface
//
ECode CFrameLayout::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    FrameLayout::SetForegroundGravity(foregroundGravity);

    return NOERROR;
}

ECode CFrameLayout::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    FrameLayout::SetForeground(drawable);

    return NOERROR;
}

ECode CFrameLayout::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = FrameLayout::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode CFrameLayout::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    FrameLayout::SetMeasureAllChildren(measureAll);

    return NOERROR;
}

ECode CFrameLayout::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();

    return NOERROR;
}

ECode CFrameLayout::constructor(
    /* [in] */ IContext* ctx)
{
    return FrameLayout::Init(ctx);
}

ECode CFrameLayout::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return FrameLayout::Init(ctx, attrs);
}

ECode CFrameLayout::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return FrameLayout::Init(ctx, attrs, defStyle);
}
