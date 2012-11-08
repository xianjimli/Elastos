
#include "ext/frameworkext.h"
#include "view/menu/CIconMenuView.h"

ECode CIconMenuView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return IconMenuView::Init(context, attrs);
}

PInterface CIconMenuView::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CIconMenuView::Probe(riid);
}

IVIEW_METHODS_IMPL(CIconMenuView, IconMenuView, IconMenuView);

IVIEWGROUP_METHODS_IMPL(CIconMenuView, IconMenuView, IconMenuView);

IDrawableCallback_METHODS_IMPL(CIconMenuView, IconMenuView, IconMenuView);

IKeyEventCallback_METHODS_IMPL(CIconMenuView, IconMenuView, IconMenuView);

IAccessibilityEventSource_METHODS_IMPL(CIconMenuView, IconMenuView, IconMenuView);

IVIEWPARENT_METHODS_IMPL(CIconMenuView, IconMenuView, IconMenuView);

IVIEWMANAGER_METHODS_IMPL(CIconMenuView, IconMenuView, IconMenuView);

ECode CIconMenuView::GetLayout(
    /* [out, callee] */ ArrayOf<Int32>** layout)
{
    VALIDATE_NOT_NULL(layout);

    return IconMenuView::GetLayout(layout);
}

ECode CIconMenuView::GetLayoutNumRows(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    *num = IconMenuView::GetLayoutNumRows();
    return NOERROR;
}

ECode CIconMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 menuType)
{
    return IconMenuView::Initialize(menu, menuType);
}

ECode CIconMenuView::UpdateChildren(
    /* [in] */ Boolean cleared)
{
    return IconMenuView::UpdateChildren(cleared);
}

ECode CIconMenuView::GetWindowAnimations(
    /* [out] */ Int32* animations)
{
    VALIDATE_NOT_NULL(animations);

    *animations = IconMenuView::GetWindowAnimations();
    return NOERROR;
}

ECode CIconMenuView::InvokeItem(
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);

    *state = IconMenuView::InvokeItem(item);
    return NOERROR;
}

ECode CIconMenuView::Run()
{
    return IconMenuView::Run();
}
