
#include "widget/CAlertControllerRecycleListView.h"

IVIEW_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

IVIEWGROUP_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

IVIEWPARENT_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

IVIEWMANAGER_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

IDrawableCallback_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

IKeyEventCallback_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

IAccessibilityEventSource_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

IADAPTERVIEW_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

IABSLISTVIEW_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);

ILISTVIEW_METHODS_IMPL(CAlertControllerRecycleListView, ListView, ListView);


CAlertControllerRecycleListView::CAlertControllerRecycleListView()
	: mRecycleOnMeasure(TRUE)
{}

ECode CAlertControllerRecycleListView::constructor(
    /* [in] */ IContext* context)
{
	return ListView::Init(context);
}

ECode CAlertControllerRecycleListView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
	return ListView::Init(context, attrs);
}

ECode CAlertControllerRecycleListView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
	return ListView::Init(context, attrs, defStyle);
}

PInterface CAlertControllerRecycleListView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return _CAlertControllerRecycleListView::Probe(riid);
}

Boolean CAlertControllerRecycleListView::RecycleOnMeasure()
{
    return mRecycleOnMeasure;
}

ECode CAlertControllerRecycleListView::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    return ListView::OnTouchModeChanged(isInTouchMode);
}

ECode CAlertControllerRecycleListView::OnGlobalLayout()
{
    return ListView::OnGlobalLayout();
}

ECode CAlertControllerRecycleListView::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    return ListView::BeforeTextChanged(s, start, count, after);
}

ECode CAlertControllerRecycleListView::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    return ListView::OnTextChanged(s, start, before, count);
}

ECode CAlertControllerRecycleListView::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    return ListView::AfterTextChanged(s);
}

ECode CAlertControllerRecycleListView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    return ListView::OnFilterComplete(count);
}
