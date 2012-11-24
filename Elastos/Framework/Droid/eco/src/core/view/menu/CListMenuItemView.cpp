
#include "view/menu/CListMenuItemView.h"

static Int32 R_Styleable_MenuView[] = {
    0x010100ae, 0x0101012c, 0x0101012d, 0x0101012e,
    0x0101012f, 0x01010130, 0x01010131
};


IVIEW_METHODS_IMPL(CListMenuItemView, _ListMenuItemView, _ListMenuItemView);

IVIEWGROUP_METHODS_IMPL(CListMenuItemView, _ListMenuItemView, _ListMenuItemView);

IVIEWPARENT_METHODS_IMPL(CListMenuItemView, _ListMenuItemView, _ListMenuItemView);

IVIEWMANAGER_METHODS_IMPL(CListMenuItemView, _ListMenuItemView, _ListMenuItemView);

IDrawableCallback_METHODS_IMPL(CListMenuItemView, _ListMenuItemView, _ListMenuItemView);

IKeyEventCallback_METHODS_IMPL(CListMenuItemView, _ListMenuItemView, _ListMenuItemView);

IAccessibilityEventSource_METHODS_IMPL(CListMenuItemView, _ListMenuItemView, _ListMenuItemView);


_ListMenuItemView::_ListMenuItemView()
{

}

_ListMenuItemView::~_ListMenuItemView()
{

}

UInt32 _ListMenuItemView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 _ListMenuItemView::Release()
{
    return ElRefBase::Release();
}

ECode _ListMenuItemView::OnFinishInflate()
{
    LinearLayout::OnFinishInflate();

    SetBackgroundDrawable(mBackground);

    AutoPtr<IView> view = FindViewById(0x01020016 /*com.android.internal.R.id.title*/);
    mTitleView = (ITextView*)view->Probe(EIID_ITextView);
    if (mTextAppearance != -1) {
        mTitleView->SetTextAppearance(mTextAppearanceContext,
                                     mTextAppearance);
    }

    view = FindViewById(0x010201ef /*com.android.internal.R.id.shortcut*/);
    mShortcutView = (ITextView*)view->Probe(EIID_ITextView);
    return NOERROR;
}


CListMenuItemView::CListMenuItemView()
{
}

ECode CListMenuItemView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(ctx, attrs, 0);
}

ECode CListMenuItemView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    LinearLayout::Init(ctx, attrs);
    AutoPtr<ITypedArray> a;
    ctx->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_MenuView, sizeof(R_Styleable_MenuView) / sizeof(Int32)),
        defStyle, 0, (ITypedArray**)&a);

    a->GetDrawable(5 /*com.android.internal.R.styleable.MenuView_itemBackground*/, (IDrawable**) &mBackground);
    a->GetResourceId(1/*com.android.internal.R.styleable.MenuView_itemTextAppearance*/, -1, &mTextAppearance);

    mTextAppearanceContext = ctx;
    a->Recycle();
    return NOERROR;
}

PInterface CListMenuItemView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return _CListMenuItemView::Probe(riid);
}

ECode CListMenuItemView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(ILinearLayout*)this) {
        *pIID = EIID_ILinearLayout;
    }
    else if (pObject == (IInterface*)(IViewParent*)this) {
        *pIID = EIID_IViewParent;
    }
    else if (pObject == (IInterface*)(IMenuItemView*)this) {
        *pIID = EIID_IMenuItemView;
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

ECode CListMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    mItemData = (MenuItemImpl*)itemData;
    mMenuType = menuType;

    Boolean visible = FALSE;
    mItemData->IsVisible(&visible);
    SetVisibility(visible ? VISIBLE : GONE);

    AutoPtr<ICharSequence> title = mItemData->GetTitleForItemView(
                (IMenuItemView*)this->Probe(EIID_IMenuItemView));

    SetTitle(title);
    Boolean isCheckable = FALSE;
    mItemData->IsCheckable(&isCheckable);
    SetCheckable(isCheckable);
    SetShortcut(mItemData->ShouldShowShortcut(),  mItemData->GetShortcut());

    AutoPtr<IDrawable> icon;
    mItemData->GetIcon((IDrawable**) &icon);
    SetIcon(icon);
    Boolean isEnabled = FALSE;
    mItemData->IsEnabled(&isEnabled);
    SetEnabled(isEnabled);

    return NOERROR;
}

ECode CListMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    //TODO :Only if the function OnFinishInflate has been invoked, the mTitleView is valid.
    if (mTitleView == NULL) {
        return NOERROR;
    }

    Int32 visibility;
    mTitleView->GetVisibility(&visibility);

    if (title != NULL) {
        //TODO TEST
        String str;
        title->ToString(&str);

        mTitleView->SetText(title);
        if (visibility != VISIBLE) {
            mTitleView->SetVisibility(VISIBLE);
        }
    } else {
        if (visibility != GONE) {
            mTitleView->SetVisibility(GONE);
        }
    }

    return NOERROR;
}

AutoPtr<IMenuItemImpl> CListMenuItemView::GetItemData()
{
    return mItemData;
}

ECode CListMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    if (!checkable && mRadioButton == NULL && mCheckBox == NULL) {
        return NOERROR;
    }

    if (mRadioButton == NULL) {
        InsertRadioButton();
    }
    if (mCheckBox == NULL) {
        InsertCheckBox();
    }

    // Depending on whether its exclusive check or not, the checkbox or
    // radio button will be the one in use (and the other will be otherCompoundButton)
    AutoPtr<ICompoundButton> compoundButton;
    AutoPtr<ICompoundButton> otherCompoundButton;

    Boolean check = FALSE;
    mItemData->IsExclusiveCheckable(&check);
    if (check) {
        compoundButton = mRadioButton;
        otherCompoundButton = mCheckBox;
    } else {
        compoundButton = mCheckBox;
        otherCompoundButton = mRadioButton;
    }

    if (checkable) {
        mItemData->IsChecked(&check);
        ((ICheckable*)compoundButton->Probe(EIID_ICheckable))->SetChecked(check);

        Int32 newVisibility = checkable ? VISIBLE : GONE;

        Int32 tmpVisibility;
        compoundButton->GetVisibility(&tmpVisibility);
        if (tmpVisibility != newVisibility) {
            compoundButton->SetVisibility(newVisibility);
        }

        // Make sure the other compound button isn't visible
        otherCompoundButton->GetVisibility(&tmpVisibility);
        if (tmpVisibility != GONE) {
            otherCompoundButton->SetVisibility(GONE);
        }
    } else {
        mCheckBox->SetVisibility(GONE);
        mRadioButton->SetVisibility(GONE);
    }

    return NOERROR;
}

ECode CListMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    AutoPtr<ICompoundButton> compoundButton;

    Boolean isExclusiveCheckable = FALSE;
    mItemData->IsExclusiveCheckable(&isExclusiveCheckable);
    if (isExclusiveCheckable) {
        if (mRadioButton == NULL) {
            InsertRadioButton();
        }

        compoundButton = mRadioButton;
    } else {
        if (mCheckBox == NULL) {
            InsertCheckBox();
        }

        compoundButton = mCheckBox;
    }

    ((ICheckable*)compoundButton->Probe(EIID_ICheckable))->SetChecked(checked);

    return NOERROR;
}

ECode CListMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    Int32 newVisibility = (showShortcut && mItemData->ShouldShowShortcut())
            ? VISIBLE : GONE;

    if (newVisibility == VISIBLE) {
        String txt = mItemData->GetShortcutLabel();
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(txt, (ICharSequence**)&cs);

        if (mShortcutView != NULL) {
            mShortcutView->SetText(cs);
        }
    }

    if (mShortcutView != NULL) {
        Int32 visibility;
        mShortcutView->GetVisibility(&visibility);
        if (visibility != newVisibility) {
            mShortcutView->SetVisibility(newVisibility);
        }
    }

    return NOERROR;
}

ECode CListMenuItemView::SetIcon(
    /* [in] */ IDrawable* icon)
{
    Boolean show = FALSE;
    mItemData->ShouldShowIcon(mMenuType, &show);
    if (!show) {
        return NOERROR;
    }

    if (mIconView == NULL && icon == NULL) {
        return NOERROR;
    }

    if (mIconView == NULL) {
        InsertIconView();
    }

    if (icon != NULL) {
        mIconView->SetImageDrawable(icon);

        Int32 visibility;
        mIconView->GetVisibility(&visibility);
        if (visibility != VISIBLE) {
            mIconView->SetVisibility(VISIBLE);
        }
    } else {
        mIconView->SetVisibility(GONE);
    }

    return NOERROR;
}

void CListMenuItemView::InsertIconView()
{
    AutoPtr<ILayoutInflater> inflater = NULL;
    mItemData->GetLayoutInflater(mMenuType, (ILayoutInflater**) &inflater);
    inflater->InflateEx2(0x0109003e /*com.android.internal.R.layout.list_menu_item_icon*/,
            this, FALSE, (IView**) &mIconView);

    AddViewEx(mIconView, 0);
}

void CListMenuItemView::InsertRadioButton()
{
    AutoPtr<ILayoutInflater> inflater = NULL;
    mItemData->GetLayoutInflater(mMenuType, (ILayoutInflater**) &inflater);
    inflater->InflateEx2(0x01090040 /*com.android.internal.R.layout.list_menu_item_radio*/,
            this, FALSE, (IView**) &mRadioButton);

    AddView(mRadioButton);
}

void CListMenuItemView::InsertCheckBox()
{
    AutoPtr<ILayoutInflater> inflater = NULL;
    mItemData->GetLayoutInflater(mMenuType, (ILayoutInflater**) &inflater);

    inflater->InflateEx2(0x0109003d /*com.android.internal.R.layout.list_menu_item_checkbox*/,
            this, FALSE, (IView**) &mCheckBox);

    AddView(mCheckBox);
}

ECode CListMenuItemView::PrefersCondensedTitle(
    /* [out] */ Boolean* perfer)
{
    *perfer = FALSE;
    return NOERROR;
}

ECode CListMenuItemView::ShowsIcon(
    /* [out] */ Boolean* show)
{
    *show = FALSE;
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

ECode CListMenuItemView::GetItemData(
    /* [out] */ IMenuItemImpl** itemData)
{
    VALIDATE_NOT_NULL(itemData);

    *itemData = mItemData;
    if (*itemData != NULL) (*itemData)->AddRef();
    return NOERROR;
}

ECode CListMenuItemView::SetEnabledEx(
    /* [in] */ Boolean enabled)
{
    LinearLayout::SetEnabled(enabled);
    return NOERROR;
}
