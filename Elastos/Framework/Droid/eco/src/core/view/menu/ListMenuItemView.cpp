
#include "view/menu/ListMenuItemView.h"

ListMenuItemView::ListMenuItemView()
{}

ListMenuItemView::ListMenuItemView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : LinearLayout(context, attrs)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ECode ListMenuItemView::OnFinishInflate()
{
    LinearLayout::OnFinishInflate();

    SetBackgroundDrawable(mBackground);

    AutoPtr<IView> view = FindViewById(0x01020016/*com.android.internal.R.id.title*/);
    mTitleView = ITextView::Probe(view);
    if (mTextAppearance != -1) {
        mTitleView->SetTextAppearance(mTextAppearanceContext, mTextAppearance);
    }

    view = FindViewById(0x010201ef/*com.android.internal.R.id.shortcut*/);
    mShortcutView = ITextView::Probe(view);
    return NOERROR;
}

ECode ListMenuItemView::Initialize(
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

ECode ListMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    //TODO :Only if the function OnFinishInflate has been invoked, the mTitleView is valid.
    if (mTitleView == NULL) {
        return NOERROR;
    }

    if (title != NULL) {
        mTitleView->SetText(title);

        Int32 visibility;
        mTitleView->GetVisibility(&visibility);
        if (visibility != VISIBLE) {
            mTitleView->SetVisibility(VISIBLE);
        }
    }
    else {
        Int32 visibility;
        mTitleView->GetVisibility(&visibility);
        if (visibility != GONE) {
            mTitleView->SetVisibility(GONE);
        }
    }

    return NOERROR;
}

AutoPtr<IMenuItemImpl> ListMenuItemView::GetItemData()
{
    return (IMenuItemImpl*)mItemData.Get();
}

ECode ListMenuItemView::SetCheckable(
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
    }
    else {
        compoundButton = mCheckBox;
        otherCompoundButton = mRadioButton;
    }

    if (checkable) {
        mItemData->IsChecked(&check);
        ICheckable::Probe(compoundButton)->SetChecked(check);

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
    }
    else {
        mCheckBox->SetVisibility(GONE);
        mRadioButton->SetVisibility(GONE);
    }

    return NOERROR;
}

ECode ListMenuItemView::SetChecked(
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
    }
    else {
        if (mCheckBox == NULL) {
            InsertCheckBox();
        }
        compoundButton = mCheckBox;
    }

    ICheckable::Probe(compoundButton)->SetChecked(checked);

    return NOERROR;
}

ECode ListMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    Int32 newVisibility = (showShortcut && mItemData->ShouldShowShortcut())
            ? VISIBLE : GONE;

    if (newVisibility == VISIBLE) {
        String label = mItemData->GetShortcutLabel();
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(label, (ICharSequence**)&cs);
        mShortcutView->SetText(cs);
    }

    Int32 visibility;
    mShortcutView->GetVisibility(&visibility);
    if (visibility != newVisibility) {
        mShortcutView->SetVisibility(newVisibility);
    }

    return NOERROR;
}

ECode ListMenuItemView::SetIcon(
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
    }
    else {
        mIconView->SetVisibility(GONE);
    }

    return NOERROR;
}

void ListMenuItemView::InsertIconView()
{
    AutoPtr<ILayoutInflater> inflater;
    mItemData->GetLayoutInflater(mMenuType, (ILayoutInflater**)&inflater);
    inflater->InflateEx2(0x0109003e/*com.android.internal.R.layout.list_menu_item_icon*/,
            (IViewGroup*)this->Probe(EIID_IViewGroup), FALSE, (IView**)&mIconView);
    AddView(mIconView, 0);
}

void ListMenuItemView::InsertRadioButton()
{
    AutoPtr<ILayoutInflater> inflater;
    mItemData->GetLayoutInflater(mMenuType, (ILayoutInflater**)&inflater);
    inflater->InflateEx2(0x01090040/*com.android.internal.R.layout.list_menu_item_radio*/,
            (IViewGroup*)this->Probe(EIID_IViewGroup), FALSE, (IView**)&mRadioButton);
    AddView(mRadioButton);
}

void ListMenuItemView::InsertCheckBox()
{
    AutoPtr<ILayoutInflater> inflater;
    mItemData->GetLayoutInflater(mMenuType, (ILayoutInflater**)&inflater);
    inflater->InflateEx2(0x0109003d/*com.android.internal.R.layout.list_menu_item_checkbox*/,
            (IViewGroup*)this->Probe(EIID_IViewGroup), FALSE, (IView**)&mCheckBox);
    AddView(mCheckBox);
}

Boolean ListMenuItemView::PrefersCondensedTitle()
{
    return FALSE;
}

Boolean ListMenuItemView::ShowsIcon()
{
    return FALSE;
}

ECode ListMenuItemView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(LinearLayout::Init(context, attrs));

    return InitFromAttributes(context, attrs, defStyle);
}

static Int32 R_Styleable_MenuView[] = {
    0x010100ae, 0x0101012c, 0x0101012d, 0x0101012e,
    0x0101012f, 0x01010130, 0x01010131
};

ECode ListMenuItemView::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_MenuView/* com.android.internal.R.styleable.MenuView */,
        sizeof(R_Styleable_MenuView) / sizeof(Int32)),
        defStyle, 0, (ITypedArray**)&a);

    a->GetDrawable(5/*com.android.internal.R.styleable.MenuView_itemBackground*/, (IDrawable**)&mBackground);
    a->GetResourceId(1/*com.android.internal.R.styleable.MenuView_itemTextAppearance*/, -1, &mTextAppearance);
    mTextAppearanceContext = context;

    a->Recycle();
    return NOERROR;
}
