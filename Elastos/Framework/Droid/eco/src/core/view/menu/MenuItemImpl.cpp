
#include "view/menu/MenuItemImpl.h"
#include "view/menu/MenuBuilder.h"
#include "view/menu/CIconMenuItemView.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

CString MenuItemImpl  ::TAG = "MenuItemImpl";

const Int32 MenuItemImpl::CHECKABLE;
const Int32 MenuItemImpl::CHECKED;
const Int32 MenuItemImpl::EXCLUSIVE;
const Int32 MenuItemImpl::HIDDEN;
const Int32 MenuItemImpl::ENABLED;

/** Used for the icon resource ID if this item does not have an icon */
const Int32 MenuItemImpl::NO_ICON;

String MenuItemImpl::sPrependShortcutLabel;
String MenuItemImpl::sEnterShortcutLabel;
String MenuItemImpl::sDeleteShortcutLabel;
String MenuItemImpl::sSpaceShortcutLabel;

/**
 * @hide
 */
MenuItemImpl::MenuItemImpl (
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ Int32 ordering,
    /* [in] */ ICharSequence* title)
    : mId(id)
    , mGroup(group)
    , mCategoryOrder(categoryOrder)
    , mOrdering(ordering)
    , mTitle(title)
    , mShortcutNumericChar(0)
    , mShortcutAlphabeticChar(0)
    , mIconResId(NO_ICON)
    , mMenu(menu)
    , mFlags(ENABLED)
{
    if (sPrependShortcutLabel.IsNull()) {
        // This is instantiated from the UI thread, so no chance of sync issues
        AutoPtr<IContext> context;
        mMenu->GetContext((IContext**) &context);
        AutoPtr<IResources> res;
        context->GetResources((IResources**)&res);
        res->GetString(0x010402ce/*com.android.internal.R.string.prepend_shortcut_label*/,
                &sPrependShortcutLabel);
        res->GetString(0x010402d0/*com.android.internal.R.string.menu_enter_shortcut_label*/,
                &sEnterShortcutLabel);
        res->GetString(0x010402d1/*com.android.internal.R.string.menu_delete_shortcut_label*/,
                &sDeleteShortcutLabel);
        res->GetString(0x010402cf/*com.android.internal.R.string.menu_space_shortcut_label*/,
                &sSpaceShortcutLabel);
    }

//    mItemViews = new WeakReference[MenuBuilder.NUM_TYPES];
    mItemViews = ArrayOf< AutoPtr<IMenuItemView> >::Alloc(MenuBuilder::NUM_TYPES);
}

MenuItemImpl::~MenuItemImpl()
{
    if (mItemViews != NULL) {
        for (Int32 i = 0; i < mItemViews->GetLength(); ++i) {
            (*mItemViews)[i] = NULL;
        }
        ArrayOf< AutoPtr<IMenuItemView> >::Free(mItemViews);
    }
}

PInterface MenuItemImpl::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IMenuItem == riid) {
        return (IMenuItem *)this;
    }
    else if (EIID_IMenuItemImpl == riid) {
        return (IMenuItemImpl *)this;
    }

    return NULL;
}

UInt32 MenuItemImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MenuItemImpl::Release()
{
    return ElRefBase::Release();
}

ECode MenuItemImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IMenuItem*)this) {
        *pIID = EIID_IMenuItem;
    }
    else if (pObject == (IInterface*)(IMenuItemImpl*)this) {
        *pIID = EIID_IMenuItemImpl;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

/**
 * Invokes the item by calling various listeners or callbacks.
 *
 * @return TRUE if the invocation was handled, FALSE otherwise
 */
ECode MenuItemImpl::Invoke(
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);

    Boolean isConsumed;
    if (mClickListener != NULL &&
        (mClickListener->OnMenuItemClick(this, &isConsumed), isConsumed)) {
        *state = TRUE;
        return NOERROR;
    }


    AutoPtr<IMenuBuilderCallback> callback;
    mMenu->GetCallback((IMenuBuilderCallback**) &callback);
    if (callback != NULL) {
        AutoPtr<IMenuBuilder> menu;
        mMenu->GetRootMenu((IMenuBuilder**)&menu);
        Boolean menuState;
        callback->OnMenuItemSelected(menu, this, &menuState);
        if (menuState == TRUE) {
             *state = TRUE;
            return NOERROR;
        }
    }

    if (mItemCallback != NULL) {
        mItemCallback->Run();
        *state = TRUE;
        return NOERROR;
    }

    if (mIntent != NULL) {
//        try {
            AutoPtr<IContext> context;
            mMenu->GetContext((IContext**)&context);
            context->StartActivity(mIntent);
            *state = TRUE;
            return NOERROR;
//        } catch (ActivityNotFoundException e) {
//            Log.e(TAG, "Can't find activity to handle intent; ignoring", e);
//        }

    }

    *state = FALSE;
    return NOERROR;
}

Boolean MenuItemImpl::HasItemView(
    /* [in] */ Int32 menuType)
{
//    return mItemViews[menuType] != null && mItemViews[menuType].get() != null;
    return (*mItemViews)[menuType] != NULL;
}

ECode MenuItemImpl::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    *enabled = (mFlags & ENABLED) != 0;
    return NOERROR;
}

ECode MenuItemImpl::SetEnabled(
	/* [in] */Boolean enabled)
{
    if (enabled) {
        mFlags |= ENABLED;
    } else {
        mFlags &= ~ENABLED;
    }

    for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        // If the item view prefers a condensed title, only set this title if there
        // is no condensed title for this item
        if (HasItemView(i)) {
            (*mItemViews)[i]->SetEnabledEx(enabled);
        }
    }

    return NOERROR;
}

ECode MenuItemImpl::GetGroupId(
    /* [out] */ Int32* groupId)
{
    VALIDATE_NOT_NULL(groupId);
    *groupId = mGroup;
    return NOERROR;
}

ECode MenuItemImpl::GetItemId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode MenuItemImpl::GetOrder(
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order);
    *order = mCategoryOrder;
    return NOERROR;
}

ECode MenuItemImpl::GetOrdering(
    /* [out] */ Int32* ordering)
{
    VALIDATE_NOT_NULL(ordering);
    *ordering = mOrdering;
    return NOERROR;
}

ECode MenuItemImpl::GetIntent(
    /* [out] */ IIntent** intent)
{
    *intent = mIntent;
    if (*intent != NULL) {
        (*intent)->AddRef();
    }
    return NOERROR;
}

ECode MenuItemImpl::SetIntent(
    /* [in] */ IIntent* intent)
{
    mIntent = intent;
    return NOERROR;
}

AutoPtr<IRunnable> MenuItemImpl::GetCallback()
{
    return mItemCallback;
}

ECode MenuItemImpl::SetCallback(
    /* [in] */ IRunnable* callback)
{
    mItemCallback = callback;
    return NOERROR;
}

ECode MenuItemImpl::GetAlphabeticShortcut(
    /* [out] */ Char32* shortcut)
{
    *shortcut = mShortcutAlphabeticChar;
    return NOERROR;
}

ECode MenuItemImpl::SetAlphabeticShortcut(
    /* [in] */ Char32 alphaChar)
{
    if (mShortcutAlphabeticChar == alphaChar) {
        return NOERROR;
    }

    mShortcutAlphabeticChar = Character::ToLowerCase(alphaChar);

    RefreshShortcutOnItemViews();

    return NOERROR;
}

ECode MenuItemImpl::GetNumericShortcut(
    /* [out] */ Char32* shortcut)
{
    *shortcut = mShortcutNumericChar;
    return NOERROR;
}

ECode MenuItemImpl::SetNumericShortcut(
    /* [in] */ Char32 numericChar)
{
    if (mShortcutNumericChar == numericChar) {
        return NOERROR;
    }

    mShortcutNumericChar = numericChar;

    RefreshShortcutOnItemViews();

    return NOERROR;
}

ECode MenuItemImpl::SetShortcut(
    /* [in] */ Char32 numericChar,
    /* [in] */ Char32 alphaChar)
{
    mShortcutNumericChar = numericChar;
    mShortcutAlphabeticChar = Character::ToLowerCase(alphaChar);

    RefreshShortcutOnItemViews();

    return NOERROR;
}

/**
 * @return The active shortcut (based on QWERTY-mode of the menu).
 */
Char32 MenuItemImpl::GetShortcut()
{
    Boolean isQwertyMode =
            ((MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase))->IsQwertyMode();
    return (isQwertyMode ? mShortcutAlphabeticChar : mShortcutNumericChar);
}

/**
 * @return The label to show for the shortcut. This includes the chording
 *         key (for example 'Menu+a'). Also, any non-human readable
 *         characters should be human readable (for example 'Menu+enter').
 */
String MenuItemImpl::GetShortcutLabel()
{
    Char32 shortcut = GetShortcut();
    if (shortcut == 0) {
        return String("");
    }

    StringBuffer sb(sPrependShortcutLabel);
    switch (shortcut) {
        case '\n':
            sb += sEnterShortcutLabel;
            break;

        case '\b':
            sb += sDeleteShortcutLabel;
            break;

        case ' ':
            sb += sSpaceShortcutLabel;
            break;

        default:
            sb += shortcut;
            break;
    }

    return String((const char*)sb);
}

/**
 * @return Whether this menu item should be showing shortcuts (depends on
 *         whether the menu should show shortcuts and whether this item has
 *         a shortcut defined)
 */
Boolean MenuItemImpl::ShouldShowShortcut()
{
    // Show shortcuts if the menu is supposed to show shortcuts AND this item has a shortcut
    Boolean visiable;
    mMenu->IsShortcutsVisible(&visiable);
    return visiable && (GetShortcut() != 0);
}

/**
 * Refreshes the shortcut shown on the ItemViews.  This method retrieves current
 * shortcut state (mode and shown) from the menu that contains this item.
 */
void MenuItemImpl::RefreshShortcutOnItemViews()
{
    Boolean isVisiable;
    mMenu->IsShortcutsVisible(&isVisiable);
    Boolean isQwertyMode =
            ((MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase))->IsQwertyMode();
    RefreshShortcutOnItemViews(isVisiable, isQwertyMode);
}

/**
 * Refreshes the shortcut shown on the ItemViews. This is usually called by
 * the {@link MenuBuilder} when it is refreshing the shortcuts on all item
 * views, so it passes arguments rather than each item calling a method on the menu to get
 * the same values.
 *
 * @param menuShortcutShown The menu's shortcut shown mode. In addition,
 *            this method will ensure this item has a shortcut before it
 *            displays the shortcut.
 * @param isQwertyMode Whether the shortcut mode is qwerty mode
 */
void MenuItemImpl::RefreshShortcutOnItemViews(
    /* [in] */ Boolean menuShortcutShown,
    /* [in] */ Boolean isQwertyMode)
{
    const Char32 shortcutKey = (isQwertyMode) ? mShortcutAlphabeticChar : mShortcutNumericChar;

    // Show shortcuts if the menu is supposed to show shortcuts AND this item has a shortcut
    const Boolean showShortcut = menuShortcutShown && (shortcutKey != 0);

    for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        if (HasItemView(i)) {
            (*mItemViews)[i]->SetShortcut(showShortcut, shortcutKey);
        }
    }
}

ECode MenuItemImpl::GetSubMenu(
    /* [out] */ ISubMenu** subMenu)
{
    VALIDATE_NOT_NULL(subMenu);

    *subMenu = ISubMenu::Probe(mSubMenu);
    if (*subMenu != NULL) {
        (*subMenu)->AddRef();
    }

    return NOERROR;
}

ECode MenuItemImpl::HasSubMenu(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);

    *has = mSubMenu != NULL;
    return NOERROR;
}

void MenuItemImpl::SetSubMenu(
    /* [in] */ ISubMenuBuilder* subMenu)
{
    if ((mMenu != NULL) && (ISubMenuBuilder::Probe(mMenu) != NULL)) {
        // throw new UnsupportedOperationException(
        // "Attempt to add a sub-menu to a sub-menu.");
        assert(0);
    }

    mSubMenu = subMenu;

    AutoPtr<ICharSequence> csq;
    GetTitle((ICharSequence**)&csq);
    ISubMenu::Probe(subMenu)->SetHeaderTitleEx(csq);
}

ECode MenuItemImpl::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    *title = mTitle;
    if (*title != NULL) (*title)->AddRef();

    return NOERROR;
}

/**
 * Gets the title for a particular {@link ItemView}
 *
 * @param itemView The ItemView that is receiving the title
 * @return Either the title or condensed title based on what the ItemView
 *         prefers
 */
AutoPtr<ICharSequence> MenuItemImpl::GetTitleForItemView(
    /* [in] */ IMenuItemView* itemView)
{
    AutoPtr<ICharSequence> title;
    Boolean supported;
    if ((itemView != NULL) && (itemView->PrefersCondensedTitle(&supported), supported)) {
        GetTitleCondensed((ICharSequence**)&title);
        return title;
    }
    else {
        GetTitle((ICharSequence**)&title);
        return title;
    }
}

ECode MenuItemImpl::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;

    for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        // If the item view prefers a condensed title, only set this title if there
        // is no condensed title for this item
        if (!HasItemView(i)) {
            continue;
        }

        IMenuItemView* itemView = (*mItemViews)[i];
        Boolean supported;
        itemView->PrefersCondensedTitle(&supported);
        if (!supported || mTitleCondensed == NULL) {
            itemView->SetTitle(title);
        }
    }

    if (mSubMenu != NULL) {
        ISubMenu::Probe(mSubMenu)->SetHeaderTitleEx(title);
    }

    return NOERROR;
}

ECode MenuItemImpl::SetTitleEx(
    /* [in] */ Int32 title)
{
    String str;

    AutoPtr<IContext> context;
    mMenu->GetContext((IContext**)&context);
    context->GetString(title, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);
    return SetTitle(csq);
}

ECode MenuItemImpl::GetTitleCondensed(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);

    *title = mTitleCondensed != NULL ? mTitleCondensed : mTitle;
    if (*title != NULL) (*title)->AddRef();

    return NOERROR;
}

ECode MenuItemImpl::SetTitleCondensed(
    /* [in] */ ICharSequence* title)
{
    mTitleCondensed = title;

    // Could use getTitle() in the loop below, but just cache what it would do here
    if (title == NULL) {
        title = mTitle;
    }

    for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        // Refresh those item views that prefer a condensed title
        Boolean supported;
        if (HasItemView(i) && ((*mItemViews)[i]->PrefersCondensedTitle(&supported), supported)) {
            (*mItemViews)[i]->SetTitle(title);
        }
    }

    return NOERROR;
}

ECode MenuItemImpl::GetIcon(
    /* [out] */ IDrawable** retDrawable)
{
    VALIDATE_NOT_NULL(retDrawable);

    if (mIconDrawable != NULL) {
        *retDrawable = mIconDrawable;
        (*retDrawable)->AddRef();
        return NOERROR;
    }

    if (mIconResId != NO_ICON) {
        AutoPtr<IDrawable> drawable;
        ((MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase))->GetResources()->GetDrawable(
                mIconResId, (IDrawable**)&drawable);

        *retDrawable = drawable;
        if (*retDrawable != NULL) {
            (*retDrawable)->AddRef();
        }
        return NOERROR;
    }

    *retDrawable = NULL;
    return NOERROR;
}

ECode MenuItemImpl::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mIconResId = NO_ICON;
    mIconDrawable = icon;
    SetIconOnViews(icon);

    return NOERROR;
}

ECode MenuItemImpl::SetIconEx(
    /* [in] */Int32 iconResId)
{
    mIconDrawable = NULL;
    mIconResId = iconResId;

    // If we have a view, we need to push the Drawable to them
    if (HaveAnyOpenedIconCapableItemViews()) {
        AutoPtr<IDrawable> drawable;
        if (iconResId != NO_ICON) {
            ((MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase))->GetResources()->GetDrawable(
                    iconResId, (IDrawable**)&drawable);
        }

        SetIconOnViews(drawable);
    }

    return NOERROR;
}

void MenuItemImpl::SetIconOnViews(
    /* [in] */ IDrawable* icon)
{
    for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        // Refresh those item views that are able to display an icon
        Boolean show;
        if (HasItemView(i) && ((*mItemViews)[i]->ShowsIcon(&show), show)) {
            (*mItemViews)[i]->SetIcon(icon);
        }
    }
}

Boolean MenuItemImpl::HaveAnyOpenedIconCapableItemViews()
{
    for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        Boolean show;
        if (HasItemView(i) && ((*mItemViews)[i]->ShowsIcon(&show), show)) {
            return TRUE;
        }
    }

    return FALSE;
}

ECode MenuItemImpl::IsCheckable(
    /* [out] */ Boolean* checkable)
{
    VALIDATE_NOT_NULL(checkable);

    *checkable = (mFlags & CHECKABLE) == CHECKABLE;

    return NOERROR;
}

ECode MenuItemImpl::SetCheckable(
    /* [in] */ Boolean checkable)
{
    const Int32 oldFlags = mFlags;
    mFlags = (mFlags & ~CHECKABLE) | (checkable ? CHECKABLE : 0);
    if (oldFlags != mFlags) {
        for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
            if (HasItemView(i)) {
                (*mItemViews)[i]->SetCheckable(checkable);
            }
        }
    }

    return NOERROR;
}

ECode MenuItemImpl::SetExclusiveCheckable(
    /* [in] */ Boolean exclusive)
{
    mFlags = (mFlags & ~EXCLUSIVE) | (exclusive ? EXCLUSIVE : 0);
    return NOERROR;
}

ECode MenuItemImpl::IsExclusiveCheckable(
    /* [out] */ Boolean* checkable)
{
    VALIDATE_NOT_NULL(checkable);

    *checkable = (mFlags & EXCLUSIVE) != 0;
    return NOERROR;
}

ECode MenuItemImpl::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);

    *checked = (mFlags & CHECKED) == CHECKED;
    return NOERROR;
}

ECode MenuItemImpl::SetChecked(
        /* [in] */ Boolean checked)
{
    if ((mFlags & EXCLUSIVE) != 0) {
        // Call the method on the Menu since it knows about the others in this
        // exclusive checkable group
        ((MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase))->SetExclusiveItemChecked(this);
    }
    else {
        SetCheckedInt(checked);
    }

    return NOERROR;
}

void MenuItemImpl::SetCheckedInt(
    /* [in] */ Boolean checked)
{
    const Int32 oldFlags = mFlags;
    mFlags = (mFlags & ~CHECKED) | (checked ? CHECKED : 0);
    if (oldFlags != mFlags) {
        for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
            if (HasItemView(i)) {
                (*mItemViews)[i]->SetChecked(checked);
            }
        }
    }
}

ECode MenuItemImpl::IsVisible(
    /* [out] */ Boolean* visiable)
{
    VALIDATE_NOT_NULL(visiable);

    *visiable = (mFlags & HIDDEN) == 0;
    return NOERROR;
}

/**
 * Changes the visibility of the item. This method DOES NOT notify the
 * parent menu of a change in this item, so this should only be called from
 * methods that will eventually trigger this change.  If unsure, use {@link #setVisible(Boolean)}
 * instead.
 *
 * @param shown Whether to show (TRUE) or hide (FALSE).
 * @return Whether the item's shown state was changed
 */
Boolean MenuItemImpl::SetVisibleInt(
    /* [in] */ Boolean shown)
{
    const Int32 oldFlags = mFlags;
    mFlags = (mFlags & ~HIDDEN) | (shown ? 0 : HIDDEN);
    return oldFlags != mFlags;
}

ECode MenuItemImpl::SetVisible(
    /* [in] */ Boolean shown)
{

    // Try to set the shown state to the given state. If the shown state was changed
    // (i.e. the previous state isn't the same as given state), notify the parent menu that
    // the shown state has changed for this item
    if (SetVisibleInt(shown)) {
        ((MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase))->OnItemVisibleChanged(this);
    }

    return NOERROR;
}

ECode MenuItemImpl::SetOnMenuItemClickListener(
        /* [in] */ IOnMenuItemClickListener* clickListener)
{
    mClickListener = clickListener;
    return NOERROR;
}

AutoPtr<IView> MenuItemImpl::GetItemView(
    /* [in] */ Int32 menuType,
    /* [in] */ IViewGroup* parent)
{
    if (!HasItemView(menuType)) {
        (*mItemViews)[menuType] = CreateItemView(menuType, parent);
    }

    return IView::Probe((*mItemViews)[menuType]);
}

/**
 * Create and initializes a menu item view that implements {@link MenuView.ItemView}.
 * @param menuType The type of menu to get a View for (must be one of
 *            {@link MenuBuilder#TYPE_ICON}, {@link MenuBuilder#TYPE_EXPANDED},
 *            {@link MenuBuilder#TYPE_SUB}, {@link MenuBuilder#TYPE_CONTEXT}).
 * @return The inflated {@link MenuView.ItemView} that is ready for use
 */
AutoPtr<IMenuItemView> MenuItemImpl::CreateItemView(
    /* [in] */ Int32 menuType,
    /* [in] */ IViewGroup* parent)
{
    // Create the MenuView
    AutoPtr<ILayoutInflater> layoutInflater;
    GetLayoutInflater(menuType, (ILayoutInflater**)&layoutInflater);

    AutoPtr<IView> view;
    ASSERT_SUCCEEDED(layoutInflater->InflateEx2(
            MenuBuilder::ITEM_LAYOUT_RES_FOR_TYPE[menuType],
            parent, FALSE, (IView**)&view));

    AutoPtr<IMenuItemView> itemView = IMenuItemView::Probe(view);
    itemView->Initialize(this, menuType);

    return itemView;
}

void MenuItemImpl::ClearItemViews()
{
    for (Int32 i = MenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        (*mItemViews)[i] = NULL;
    }
}

//@Override
ECode MenuItemImpl::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    return mTitle->ToString(string);
}

void MenuItemImpl::SetMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo)
{
    mMenuInfo = menuInfo;
}

ECode MenuItemImpl::GetMenuInfo(
    /* [out] */ IContextMenuInfo** menuInfo)
{
    VALIDATE_NOT_NULL(menuInfo);

    *menuInfo = mMenuInfo;
    if (*menuInfo != NULL) {
        (*menuInfo)->AddRef();
    }

    return NOERROR;
}

/**
 * Returns a LayoutInflater that is themed for the given menu type.
 *
 * @param menuType The type of menu.
 * @return A LayoutInflater.
 */
ECode MenuItemImpl::GetLayoutInflater(
    /* [in] */ Int32 menuType,
    /* [out] */ ILayoutInflater** layoutInflater)
{
    VALIDATE_NOT_NULL(layoutInflater);

    AutoPtr<ILayoutInflater> inflater = ((MenuBuilderBase*)mMenu->Probe(
            EIID_MenuBuilderBase))->GetMenuType(menuType)->GetInflater();
    *layoutInflater = inflater;
    if (*layoutInflater != NULL) (*layoutInflater)->AddRef();

    return NOERROR;
}

/**
 * @return Whether the given menu type should show icons for menu items.
 */
ECode MenuItemImpl::ShouldShowIcon(
    /* [in] */ Int32 menuType,
    /* [out] */ Boolean* show)
{
    VALIDATE_NOT_NULL(show);

    *show = menuType == MenuBuilder::TYPE_ICON ||
            ((MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase))->GetOptionalIconsVisible();
    return NOERROR;
}
