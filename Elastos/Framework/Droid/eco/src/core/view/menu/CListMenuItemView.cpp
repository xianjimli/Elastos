
#include "view/menu/CListMenuItemView.h"


IVIEW_METHODS_IMPL(CListMenuItemView, ListMenuItemView, ListMenuItemView);

IVIEWGROUP_METHODS_IMPL(CListMenuItemView, ListMenuItemView, ListMenuItemView);

IVIEWPARENT_METHODS_IMPL(CListMenuItemView, ListMenuItemView, ListMenuItemView);

IVIEWMANAGER_METHODS_IMPL(CListMenuItemView, ListMenuItemView, ListMenuItemView);

IDrawableCallback_METHODS_IMPL(CListMenuItemView, ListMenuItemView, ListMenuItemView);

IKeyEventCallback_METHODS_IMPL(CListMenuItemView, ListMenuItemView, ListMenuItemView);

IAccessibilityEventSource_METHODS_IMPL(CListMenuItemView, ListMenuItemView, ListMenuItemView);

ECode CListMenuItemView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ListMenuItemView::Init(ctx, attrs, defStyle);
}

PInterface CListMenuItemView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return _CListMenuItemView::Probe(riid);
}

ECode CListMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    return ListMenuItemView::Initialize(itemData, menuType);
}

ECode CListMenuItemView::GetItemData(
    /* [out] */ IMenuItemImpl** itemData)
{
    VALIDATE_NOT_NULL(itemData);

    AutoPtr<IMenuItemImpl> item = ListMenuItemView::GetItemData();
    *itemData = item.Get();
    if (*itemData != NULL) (*itemData)->AddRef();
    return NOERROR;
}

ECode CListMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return ListMenuItemView::SetTitle(title);
}

ECode CListMenuItemView::SetEnabledEx(
    /* [in] */ Boolean enabled)
{
    return ListMenuItemView::SetEnabled(enabled);
}

ECode CListMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return ListMenuItemView::SetCheckable(checkable);
}

ECode CListMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    return ListMenuItemView::SetChecked(checked);
}

ECode CListMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    return ListMenuItemView::SetShortcut(showShortcut, shortcutKey);
}

ECode CListMenuItemView::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return ListMenuItemView::SetIcon(icon);
}

ECode CListMenuItemView::PrefersCondensedTitle(
    /* [out] */ Boolean* prefer)
{
    VALIDATE_NOT_NULL(prefer);

    *prefer = ListMenuItemView::PrefersCondensedTitle();
    return NOERROR;
}

ECode CListMenuItemView::ShowsIcon(
    /* [out] */ Boolean* show)
{
    VALIDATE_NOT_NULL(show);

    *show = ListMenuItemView::ShowsIcon();
    return NOERROR;
}

ECode CListMenuItemView::IsBaselineAligned(
    /* [out] */  Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = LinearLayout::IsBaselineAligned();
    return NOERROR;
}

ECode CListMenuItemView::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return LinearLayout::SetBaselineAligned(baselineAligned);
}

ECode CListMenuItemView::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = LinearLayout::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CListMenuItemView::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return LinearLayout::SetBaselineAlignedChildIndex(index);
}

ECode CListMenuItemView::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = LinearLayout::GetWeightSum();
    return NOERROR;
}

ECode CListMenuItemView::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return LinearLayout::SetWeightSum(weightSum);
}

ECode CListMenuItemView::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return LinearLayout::SetOrientation(orientation);
}

ECode CListMenuItemView::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = LinearLayout::GetOrientation();
    return NOERROR;
}

ECode CListMenuItemView::SetGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetGravity(gravity);
}

ECode CListMenuItemView::SetHorizontalGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetHorizontalGravity(gravity);
}

ECode CListMenuItemView::SetVerticalGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetVerticalGravity(gravity);
}
