
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
{

}

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

ECode CAlertControllerRecycleListView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IListView*)this) {
        *pIID = EIID_IListView;
    }
    else if (pObject == (IInterface*)(ITextWatcher*)this) {
        *pIID = EIID_ITextWatcher;
    }
    else if (pObject == (IInterface*)(IOnGlobalLayoutListener*)this) {
        *pIID = EIID_IOnGlobalLayoutListener;
    }
    else if (pObject == (IInterface*)(IOnTouchModeChangeListener*)this) {
        *pIID = EIID_IOnTouchModeChangeListener;
    }
    else if (pObject == (IInterface*)(IFilterListener*)this) {
        *pIID = EIID_IFilterListener;
    }
    else if (pObject == (IInterface*)(IViewParent*)this) {
        *pIID = EIID_IViewParent;
    }
    else if (pObject == (IInterface*)(IViewManager*)this) {
        *pIID = EIID_IViewManager;
    }
    else if (pObject == (IInterface*)(IDrawableCallback*)this) {
        *pIID = EIID_IDrawableCallback;
    }
    else if (pObject == (IInterface*)(IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
    }
    else if (pObject == (IInterface*)(IAccessibilityEventSource*)this) {
        *pIID = EIID_IAccessibilityEventSource;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
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
