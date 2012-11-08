
#include "ext/frameworkext.h"
#include "view/menu/CIconMenuItemView.h"

ECode CIconMenuItemView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return IconMenuItemView::Init(context, attrs, defStyle);
}

ECode CIconMenuItemView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return IconMenuItemView::Init(context, attrs);
}

PInterface CIconMenuItemView::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CIconMenuItemView::Probe(riid);
}

IVIEW_METHODS_IMPL(CIconMenuItemView, IconMenuItemView, IconMenuItemView);

ITEXTVIEW_METHODS_IMPL(CIconMenuItemView, IconMenuItemView, IconMenuItemView);

IDrawableCallback_METHODS_IMPL(CIconMenuItemView, IconMenuItemView, IconMenuItemView);

IKeyEventCallback_METHODS_IMPL(CIconMenuItemView, IconMenuItemView, IconMenuItemView);

IAccessibilityEventSource_METHODS_IMPL(CIconMenuItemView, IconMenuItemView, IconMenuItemView);

ECode CIconMenuItemView::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = IconMenuItemView::OnPreDraw();
    return NOERROR;
}

ECode CIconMenuItemView::SetItemInvoker(
    /* [in] */ IMenuBuilderItemInvoker* itemInvoker)
{
    return IconMenuItemView::SetItemInvoker(itemInvoker);
}

//from IMenuItemView
ECode CIconMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    return IconMenuItemView::Initialize(itemData, menuType);
}

ECode CIconMenuItemView::GetItemData(
    /* [out] */ IMenuItemImpl** itemData)
{
    VALIDATE_NOT_NULL(itemData);

    AutoPtr<IMenuItemImpl> item = IconMenuItemView::GetItemData();
    *itemData = item;
    if (*itemData != NULL) (*itemData)->AddRef();
    return NOERROR;
}

ECode CIconMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return IconMenuItemView::SetTitle(title);
}

ECode CIconMenuItemView::SetEnabledEx(
    /* [in] */ Boolean enabled)
{
    return IconMenuItemView::SetEnabled(enabled);
}

ECode CIconMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return IconMenuItemView::SetCheckable(checkable);
}

ECode CIconMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    return IconMenuItemView::SetChecked(checked);
}

ECode CIconMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    return IconMenuItemView::SetShortcut(showShortcut, shortcutKey);
}

ECode CIconMenuItemView::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return IconMenuItemView::SetIcon(icon);
}

ECode CIconMenuItemView::PrefersCondensedTitle(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    *supported = IconMenuItemView::PrefersCondensedTitle();
    return NOERROR;
}

ECode CIconMenuItemView::ShowsIcon(
    /* [out] */ Boolean* show)
{
    VALIDATE_NOT_NULL(show);

    *show = IconMenuItemView::ShowsIcon();
    return NOERROR;
}
