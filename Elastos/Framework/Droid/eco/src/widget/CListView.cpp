
#include "widget/CListView.h"

IVIEW_METHODS_IMPL(CListView, ListView, ListView);

IVIEWGROUP_METHODS_IMPL(CListView, ListView, ListView);

IVIEWPARENT_METHODS_IMPL(CListView, ListView, ListView);

IVIEWMANAGER_METHODS_IMPL(CListView, ListView, ListView);

IDrawableCallback_METHODS_IMPL(CListView, ListView, ListView);

IKeyEventCallback_METHODS_IMPL(CListView, ListView, ListView);

IAccessibilityEventSource_METHODS_IMPL(CListView, ListView, ListView);

IADAPTERVIEW_METHODS_IMPL(CListView, ListView, ListView);

IABSLISTVIEW_METHODS_IMPL(CListView, ListView, ListView);

ILISTVIEW_METHODS_IMPL(CListView, ListView, ListView);

PInterface CListView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CListView::Probe(riid);
}

ECode CListView::constructor(
    /* [in] */ IContext* ctx)
{
    return ListView::Init(ctx);
}

ECode CListView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return ListView::Init(ctx, attrs);
}

ECode CListView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ListView::Init(ctx, attrs, defStyle);
}

ECode CListView::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    return ListView::OnTouchModeChanged(isInTouchMode);
}

ECode CListView::OnGlobalLayout()
{
    return ListView::OnGlobalLayout();
}

ECode CListView::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    return ListView::BeforeTextChanged(s, start, count, after);
}

ECode CListView::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    return ListView::OnTextChanged(s, start, before, count);
}

//ECode CListView::AfterTextChanged(
//    /* [in] */ IEditable* s);

ECode CListView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    return ListView::OnFilterComplete(count);
}
