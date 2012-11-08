
#include "graphics/CRect.h"
#include "view/menu/IconMenuItemView.h"
#include "view/CGravity.h"
#include "text/Layout.h"
#include "view/menu/CIconMenuView.h"
#include "view/menu/CIconMenuViewLayoutParams.h"


const Int32 IconMenuItemView::NO_ALPHA;
String IconMenuItemView::sPrependShortcutLabel;

IconMenuItemView::IconMenuItemView()
    : mShortcutCaptionMode(FALSE)
{
    CRect::New((IRect**)&mPositionIconAvailable);
    CRect::New((IRect**)&mPositionIconOutput);
}

static Int32 R_Styleable_MenuView[] = {
    0x010100ae, 0x0101012c, 0x0101012d, 0x0101012e,
    0x0101012f, 0x01010130, 0x01010131
};

ECode IconMenuItemView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(TextView::Init(context, attrs));

    if (sPrependShortcutLabel.IsNull()) {
        /*
         * Views should only be constructed from the UI thread, so no
         * synchronization needed
         */
        GetResources()->GetString(0x010402ce/*com.android.internal.R.string.prepend_shortcut_label*/, &sPrependShortcutLabel);
    }

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_MenuView, sizeof(R_Styleable_MenuView) / sizeof(Int32)),
        defStyle, 0, (ITypedArray**)&a);

    a->GetFloat(6/*com.android.internal.R.styleable.MenuView_itemIconDisabledAlpha*/, 0.8f, &mDisabledAlpha);
    a->GetResourceId(1/*com.android.internal.R.styleable.MenuView_itemTextAppearance*/, -1, &mTextAppearance);
    mTextAppearanceContext = context;

    a->Recycle();

    return NOERROR;
}

/**
 * Initializes with the provided title and icon
 * @param title The title of this item
 * @param icon The icon of this item
 */
void IconMenuItemView::Initialize(
    /* [in] */ ICharSequence* title,
    /* [in] */ IDrawable* icon)
{
    SetClickable(TRUE);
    SetFocusable(TRUE);

    if (mTextAppearance != -1) {
        SetTextAppearance(mTextAppearanceContext, mTextAppearance);
    }

    SetTitle(title);
    SetIcon(icon);
}

ECode IconMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    mItemData = (MenuItemImpl*)itemData;

    AutoPtr<ICharSequence> str = mItemData->GetTitleForItemView(
            (IMenuItemView*)this->Probe(EIID_IMenuItemView));
    AutoPtr<IDrawable> icon;
    itemData->GetIcon((IDrawable**) &icon);
    Initialize(str, icon);

    Boolean isVisible;
    itemData->IsVisible(&isVisible);
    SetVisibility(isVisible ? View::VISIBLE : View::GONE);
    Boolean isEnabled;
    itemData->IsEnabled(&isEnabled);
    SetEnabled(isEnabled);

    return NOERROR;
}

//@override
Boolean IconMenuItemView::PerformClick()
{
    // Let the view's click listener have top priority (the More button relies on this)
    if (TextView::PerformClick()) {
        return TRUE;
    }

    Boolean result;
    if (mItemInvoker != NULL && (mItemInvoker->InvokeItem(mItemData, &result), result)) {
        PlaySoundEffect(SoundEffectConstants_CLICK);
        return TRUE;
    }
    else {
        return FALSE;
    }
}

ECode IconMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    if (mShortcutCaptionMode) {
        /*
         * Don't set the title directly since it will replace the
         * shortcut+title being shown. Instead, re-set the shortcut caption
         * mode so the new title is shown.
         */
        SetCaptionMode(TRUE);
    }
    else if (title != NULL) {
        SetText(title);
    }

    return NOERROR;
}

void IconMenuItemView::SetCaptionMode(
    /* [in] */ Boolean shortcut)
{
    /*
     * If there is no item model, don't do any of the below (for example,
     * the 'More' item doesn't have a model)
     */
    if (mItemData == NULL) {
        return;
    }

    mShortcutCaptionMode = shortcut && mItemData->ShouldShowShortcut();

    AutoPtr<ICharSequence> text = mItemData->GetTitleForItemView(
            (IMenuItemView*)this->Probe(EIID_IMenuItemView));

    if (mShortcutCaptionMode) {
        if (mShortcutCaption.IsNull()) {
            mShortcutCaption = mItemData->GetShortcutLabel();
        }

        text = NULL;
        CStringWrapper::New(mShortcutCaption, (ICharSequence**)&text);
    }

    SetText(text);
}

ECode IconMenuItemView::SetIcon(
        /* [in] */ IDrawable* icon)
{
    mIcon = icon;

    if (icon != NULL) {
        /* Set the bounds of the icon since setCompoundDrawables needs it. */
        Int32 width, height;
        icon->GetIntrinsicWidth(&width);
        icon->GetIntrinsicHeight(&height);
        icon->SetBounds(0, 0, width, height);

        // Set the compound drawables
        SetCompoundDrawables(NULL, icon, NULL, NULL);

        // When there is an icon, make sure the text is at the bottom
        SetGravity(Gravity_BOTTOM | Gravity_CENTER_HORIZONTAL);

        /*
         * Request a layout to reposition the icon. The positioning of icon
         * depends on this TextView's line bounds, which is only available
         * after a layout.
         */
        RequestLayout();
    }
    else {
        SetCompoundDrawables(NULL, NULL, NULL, NULL);

        // When there is no icon, make sure the text is centered vertically
        SetGravity(Gravity_CENTER_VERTICAL | Gravity_CENTER_HORIZONTAL);
    }

    return NOERROR;
}

ECode IconMenuItemView::SetItemInvoker(
        /* [in] */ IMenuBuilderItemInvoker* itemInvoker)
{
    mItemInvoker = itemInvoker;

    return NOERROR;
}

AutoPtr<IMenuItemImpl> IconMenuItemView::GetItemData()
{
    return mItemData;
}

//@override
ECode IconMenuItemView::SetVisibility(
    /* [in] */ Int32 v)
{
    TextView::SetVisibility(v);

    if (mIconMenuView != NULL) {
        // On visibility change, mark the IconMenuView to refresh itself eventually
        ((CIconMenuView*)mIconMenuView.Get())->MarkStaleChildren();
    }

    return NOERROR;
}

void IconMenuItemView::SetIconMenuView(
    /* [in] */ IIconMenuView* iconMenuView)
{
    mIconMenuView = iconMenuView;;
}

//@override
ECode IconMenuItemView::DrawableStateChanged()
{
    TextView::DrawableStateChanged();

    if (mItemData != NULL && mIcon != NULL) {
        // When disabled, the not-focused state and the pressed state should
        // drop alpha on the icon
        Boolean isEnabled;
        mItemData->IsEnabled(&isEnabled);
        Boolean isInAlphaState = !isEnabled && (IsPressed() || !IsFocused());
        mIcon->SetAlpha(isInAlphaState ? (Int32)(mDisabledAlpha * NO_ALPHA) : NO_ALPHA);
    }

    return NOERROR;
}

//@override
void IconMenuItemView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    TextView::OnLayout(changed, left, top, right, bottom);

    PositionIcon();
}

void IconMenuItemView::OnTextChanged(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    TextView::OnTextChanged(text, start, before, after);
    // our layout params depend on the length of the text
    SetLayoutParams(GetTextAppropriateLayoutParams());
}

/**
 * @return layout params appropriate for this view.  If layout params already exist, it will
 *         augment them to be appropriate to the current text size.
 */
AutoPtr<IIconMenuViewLayoutParams> IconMenuItemView::GetTextAppropriateLayoutParams()
{
    AutoPtr<IIconMenuViewLayoutParams> lp;

    AutoPtr<IViewGroupLayoutParams> _lp = GetLayoutParams();
    if (_lp == NULL) {
        // Default layout parameters
        CIconMenuViewLayoutParams::New(ViewGroupLayoutParams_MATCH_PARENT,
                ViewGroupLayoutParams_MATCH_PARENT, (IIconMenuViewLayoutParams**)&lp);
    }
    else {
        lp = IIconMenuViewLayoutParams::Probe(_lp);
    }

    // Set the desired width of item
    ((CIconMenuViewLayoutParams*)lp.Get())->mDesiredWidth =
            (Int32)Layout::GetDesiredWidth(GetText(), GetPaint());

    return lp;
}

/**
 * Positions the icon vertically (horizontal centering is taken care of by
 * the TextView's gravity).
 */
void IconMenuItemView::PositionIcon()
{
    if (mIcon == NULL) {
        return;
    }

    // We reuse the output rectangle as a temp rect
    IRect* tmpRect = mPositionIconOutput;
    GetLineBounds(0, tmpRect);

    mPositionIconAvailable->Set(0, 0, GetWidth(), ((CRect*)tmpRect)->mTop);

    Int32 iconWidth, iconHeight;
    mIcon->GetIntrinsicWidth(&iconWidth);
    mIcon->GetIntrinsicHeight(&iconHeight);

    AutoPtr<IGravity> gravity;
    CGravity::AcquireSingleton((IGravity**)&gravity);
    gravity->Apply(Gravity_CENTER_VERTICAL | Gravity_LEFT, iconWidth, iconHeight, mPositionIconAvailable, mPositionIconOutput);
    mIcon->SetBoundsEx(mPositionIconOutput);
}

ECode IconMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return NOERROR;
}

ECode IconMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    return NOERROR;
}

ECode IconMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    if (mShortcutCaptionMode) {
        /*
         * Shortcut has changed and we're showing it right now, need to
         * update (clear the old one first).
         */
        mShortcutCaption = NULL;
        SetCaptionMode(TRUE);
    }

    return NOERROR;
}

Boolean IconMenuItemView::PrefersCondensedTitle()
{
    return TRUE;
}

Boolean IconMenuItemView::ShowsIcon()
{
    return TRUE;
}

