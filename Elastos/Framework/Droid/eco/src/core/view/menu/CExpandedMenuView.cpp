
#include "ext/frameworkext.h"
#include "view/menu/CExpandedMenuView.h"


ECode CExpandedMenuView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ExpandedMenuView::Init(context, attrs);
}

PInterface CExpandedMenuView::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CExpandedMenuView::Probe(riid);
}

IVIEW_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

IVIEWGROUP_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

IVIEWPARENT_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

IVIEWMANAGER_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

IDrawableCallback_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

IKeyEventCallback_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

IAccessibilityEventSource_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

IADAPTERVIEW_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

IABSLISTVIEW_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

ILISTVIEW_METHODS_IMPL(CExpandedMenuView, ExpandedMenuView, ExpandedMenuView);

//from IMenuBuilderItemInvoker
ECode CExpandedMenuView::InvokeItem(
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);

    *state = ExpandedMenuView::InvokeItem(item);
    return NOERROR;
}

//from IMenuView
ECode CExpandedMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 menuType)
{
    return ExpandedMenuView::Initialize(menu, menuType);
}

ECode CExpandedMenuView::UpdateChildren(
    /* [in] */ Boolean cleared)
{
    return ExpandedMenuView::UpdateChildren(cleared);
}

ECode CExpandedMenuView::GetWindowAnimations(
    /* [out] */ Int32* animations)
{
    VALIDATE_NOT_NULL(animations);

    *animations = ExpandedMenuView::GetWindowAnimations();
    return NOERROR;
}

//from IOnItemClickListener
ECode CExpandedMenuView::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return ExpandedMenuView::OnItemClick(parent, view, position, id);
}

ECode CExpandedMenuView::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    return ExpandedMenuView::OnTouchModeChanged(isInTouchMode);
}

ECode CExpandedMenuView::OnGlobalLayout()
{
    return ExpandedMenuView::OnGlobalLayout();
}

ECode CExpandedMenuView::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    return ExpandedMenuView::BeforeTextChanged(s, start, count, after);
}

ECode CExpandedMenuView::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    return ExpandedMenuView::OnTextChanged(s, start, before, count);
}

ECode CExpandedMenuView::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    return ExpandedMenuView::AfterTextChanged(s);
}

ECode CExpandedMenuView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    return ExpandedMenuView::OnFilterComplete(count);
}

