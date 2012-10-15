
#include "view/menu/CMenuItemImpl.h"
#include "view/menu/CMenuBuilder.h"
#include <stdio.h>

String CMenuItemImpl::TAG = String("CMenuItemImpl");
const int CMenuItemImpl::CHECKABLE;
const int CMenuItemImpl::CHECKED;
const int CMenuItemImpl::EXCLUSIVE;
const int CMenuItemImpl::HIDDEN;
const int CMenuItemImpl::ENABLED;

/** Used for the icon resource ID if this item does not have an icon */
const int CMenuItemImpl::NO_ICON;

String CMenuItemImpl::sPrependShortcutLabel =  String("");
String CMenuItemImpl::sEnterShortcutLabel =  String("");
String CMenuItemImpl::sDeleteShortcutLabel =  String("");
String CMenuItemImpl::sSpaceShortcutLabel =  String("");

/**
 * @hide
 */
CMenuItemImpl::CMenuItemImpl(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title):
        mId(id), mGroup(group), mCategoryOrder(categoryOrder), mOrdering(ordering)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    mItemViews = NULL;
    mMenu = menu;
    if (title != NULL) {
        title->ToString(&mTitle);
    }

    mIconResId = NO_ICON;
    mFlags = ENABLED;
}

CMenuItemImpl::CMenuItemImpl():
    mId(0), mGroup(0), mCategoryOrder(0), mOrdering(0)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
}

ECode CMenuItemImpl::constructor(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title)
{
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    mMenu = menu;
    mId = id;
    mGroup = group;
    mCategoryOrder = categoryOrder;
    mOrdering = ordering;

    if (sPrependShortcutLabel.IsEmpty()) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        // This is instantiated from the UI thread, so no chance of sync issues
        if (mMenu != NULL) {
            AutoPtr<IContext> context = NULL;
            mMenu->GetContext((IContext**) &context);
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);

            if (context == NULL) {
                printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
            }
            else {
                printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
            }

            AutoPtr<IResources> res = NULL;
            context->GetResources((IResources**)&res);
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);

            if (res != NULL) {
                res->GetString(0x010402ce, &sPrependShortcutLabel);
                res->GetString(0x010402d0, &sEnterShortcutLabel);
                res->GetString(0x010402d1, &sDeleteShortcutLabel);
                res->GetString(0x010402cf, &sSpaceShortcutLabel);
            }
        }
    }

printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);

    if (mItemViews == NULL) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        mItemViews = new AutoPtr<IMenuItemView>[CMenuBuilder::NUM_TYPES];
        memset(mItemViews, 0, sizeof(IMenuItemView*) * CMenuBuilder::NUM_TYPES);
    }
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (title != NULL) {
        title->ToString(&mTitle);
    }
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    mIconResId = NO_ICON;
    mFlags = ENABLED;
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    return NOERROR;
}

PInterface CMenuItemImpl::Probe(
    /* [in] */ REIID riid) {
    return _CMenuItemImpl::Probe(riid);
}

CMenuItemImpl::~CMenuItemImpl() {
    // if (mItemViews != NULL) {

    //     delete[] mItemViews;
    //     mItemViews = NULL;
    // }
}

/**
 * Invokes the item by calling various listeners or callbacks.
 *
 * @return TRUE if the invocation was handled, FALSE otherwise
 */
ECode CMenuItemImpl::Invoke(
    /* [out] */ Boolean* state) {
    Boolean isConsumed = FALSE;
    mClickListener->OnMenuItemClick(this, &isConsumed);
    if (mClickListener != NULL && isConsumed) {
        *state = TRUE;
        return NOERROR;
    }

    AutoPtr<IMenuBuilderCallback> callback;
    mMenu->GetCallback((IMenuBuilderCallback**) &callback);
    AutoPtr<IMenuBuilder> tempMenu;
    ((MenuBuilder*)mMenu.Get())->GetRootMenu((IMenuBuilder**) &tempMenu);

    Boolean menuState;
    callback->OnMenuItemSelected(tempMenu, (IMenuItem*)this->Probe(EIID_IMenuItem), &menuState);

    if (callback != NULL && menuState) {
        *state = TRUE;
        return NOERROR;
    }

    if (mItemCallback != NULL) {
        mItemCallback->Run();
        *state = TRUE;
        return NOERROR;
    }

    if (mIntent != NULL) {
        //TODO
        /*
        try {
            mMenu->GetContext()->StartActivity(mIntent);
            *state = TRUE;
            return NOERROR;
        } catch (ActivityNotFoundException e) {
            Log.e(TAG, "Can't find activity to handle intent; ignoring", e);
        }
        */

        AutoPtr<IContext> context;
        mMenu->GetContext((IContext**) &context);
        context->StartActivity(mIntent);

        *state = TRUE;
        return NOERROR;
    }

    *state = FALSE;
    return NOERROR;
}

Boolean CMenuItemImpl::HasItemView(int menuType) {
    return mItemViews[menuType] != NULL;
}

ECode CMenuItemImpl::IsEnabled(
    /* [out] */ Boolean* enabled) {
    *enabled = (mFlags & ENABLED) != 0;

    return NOERROR;
}

ECode CMenuItemImpl::SetEnabled(
		/* [in] */Boolean enabled,
    /* [out] */ IMenuItem** menuItem) {
    if (enabled) {
        mFlags |= ENABLED;
    } else {
        mFlags &= ~ENABLED;
    }

    for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        // If the item view prefers a condensed title, only set this title if there
        // is no condensed title for this item
        if (HasItemView(i)) {
            mItemViews[i]->SetEnabled(enabled);
        }
    }

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::GetGroupId(
    /* [out] */ Int32* groupId) {
    *groupId = mGroup;
    return NOERROR;
}

//@ViewDebug.CapturedViewProperty
ECode CMenuItemImpl::GetItemId(
    /* [out] */ Int32* id) {
    *id = mId;
    return NOERROR;
}

ECode CMenuItemImpl::GetOrder(
    /* [out] */ Int32* order) {
    *order = mCategoryOrder;
    return NOERROR;
}

ECode CMenuItemImpl::GetOrdering(
    /* [out] */ Int32* ordering) {
    *ordering = mOrdering;

    return NOERROR;
}

ECode CMenuItemImpl::GetIntent(
    /* [out] */ IIntent** intent) {
    *intent = mIntent;
    if (*intent != NULL) {
        (*intent)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetIntent(
    /* [in] */ IIntent* intent,
    /* [out] */ IMenuItem** menuItem) {
    mIntent = intent;
    *menuItem = this;
    return NOERROR;
}

ECode CMenuItemImpl::GetCallback(
    /* [out] */ IRunnable** runnable) {
    *runnable = mItemCallback;
    if (*runnable != NULL) {
        (*runnable)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetCallback(
    /* [in] */ IRunnable* callback,
    /* [out] */ IMenuItem** menuItem) {
    mItemCallback = callback;

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::GetAlphabeticShortcut(
    /* [out] */ Char8* shortcut) {
    *shortcut = mShortcutAlphabeticChar;
    return NOERROR;
}

ECode CMenuItemImpl::SetAlphabeticShortcut(
    /* [in] */ Char8 alphaChar,
    /* [out] */ IMenuItem** menuItem) {
    if (mShortcutAlphabeticChar == alphaChar) {
        *menuItem = this;
        return NOERROR;
    }

    //TODO
    //mShortcutAlphabeticChar = String::ToLowerCase(alphaChar);
    mShortcutAlphabeticChar = alphaChar;

    RefreshShortcutOnItemViews();

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::GetNumericShortcut(
    /* [out] */ Char8* shortcut) {
    *shortcut = mShortcutNumericChar;
    return NOERROR;
}

ECode CMenuItemImpl::SetNumericShortcut(
    /* [in] */ Char8 numericChar,
    /* [out] */ IMenuItem** menuItem) {
    if (mShortcutNumericChar == numericChar) {
        *menuItem = this;
        return NOERROR;
    }

    mShortcutNumericChar = numericChar;

    RefreshShortcutOnItemViews();

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetShortcut(
    /* [in] */ Char8 numericChar,
    /* [in] */ Char8 alphaChar,
    /* [out] */ IMenuItem** menuItem) {
    mShortcutNumericChar = numericChar;

    //TODO
    //mShortcutAlphabeticChar = Character.toLowerCase(alphaChar);
    mShortcutAlphabeticChar = alphaChar;

    RefreshShortcutOnItemViews();

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

/**
 * @return The active shortcut (based on QWERTY-mode of the menu).
 */
ECode CMenuItemImpl::GetShortcut(
    /* [out] */ Char8* shortcut) {
    Boolean isQwerty = FALSE;
    mMenu->IsQwertyMode(&isQwerty) ;

    *shortcut = mShortcutNumericChar;
    if (isQwerty) {
        *shortcut = mShortcutAlphabeticChar;
    }

    return NOERROR;
}

/**
 * @return The label to show for the shortcut. This includes the chording
 *         key (for example 'Menu+a'). Also, any non-human readable
 *         characters should be human readable (for example 'Menu+enter').
 */
ECode CMenuItemImpl::GetShortcutLabel(
    /* [out] */String* label) {

    Char8 shortcut;
    GetShortcut(&shortcut);
    if (shortcut == 0) {
        *label = "";
        return NOERROR;
    }

    //StringBuilder sb = new StringBuilder(sPrependShortcutLabel);
    StringBuf_<255> sb;
    switch (shortcut) {
        case '\n':
            sb.Append(sEnterShortcutLabel);
            break;

        case '\b':
            sb.Append(sDeleteShortcutLabel);
            break;

        case ' ':
            sb.Append(sSpaceShortcutLabel);
            break;

        default:
            sb.Append(shortcut);
            break;
    }

    *label = sb.GetPayload();
    return NOERROR;
}

/**
 * @return Whether this menu item should be showing shortcuts (depends on
 *         whether the menu should show shortcuts and whether this item has
 *         a shortcut defined)
 */
ECode CMenuItemImpl::ShouldShowShortcut(
    /* [out] */ Boolean* shortcut) {
    // Show shortcuts if the menu is supposed to show shortcuts AND this item has a shortcut
    Char8 temp;
    GetShortcut(&temp);

    Boolean visiable;
    mMenu->IsShortcutsVisible(&visiable);

    *shortcut = visiable && (temp != 0);
    return NOERROR;
}

/**
 * Refreshes the shortcut shown on the ItemViews.  This method retrieves current
 * shortcut state (mode and shown) from the menu that contains this item.
 */
ECode CMenuItemImpl::RefreshShortcutOnItemViews() {
    Boolean isQwertyMode = FALSE;
    mMenu->IsQwertyMode(&isQwertyMode);

    Boolean visiable;
    mMenu->IsShortcutsVisible(&visiable);
    RefreshShortcutOnItemViews2(visiable, isQwertyMode);

    return NOERROR;
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
ECode CMenuItemImpl::RefreshShortcutOnItemViews2(Boolean menuShortcutShown, Boolean isQwertyMode) {
    const char shortcutKey = (isQwertyMode) ? mShortcutAlphabeticChar : mShortcutNumericChar;

    // Show shortcuts if the menu is supposed to show shortcuts AND this item has a shortcut
    const Boolean showShortcut = menuShortcutShown && (shortcutKey != 0);

    for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        if (HasItemView(i)) {
            mItemViews[i]->SetShortcut(showShortcut, shortcutKey);
        }
    }

    return NOERROR;
}

ECode CMenuItemImpl::GetSubMenu(
    /* [out] */ ISubMenu** builder) {
    *builder = mSubMenu;
    if (*builder != NULL) {
        (*builder)->AddRef();
    }

    return NOERROR;
}

//ECode CMenuItemImpl::GetSubMenu(
//    /* [out] */ ISubMenu** builder) {
//    *builder = mSubMenu;

//    return NOERROR;
//}

ECode CMenuItemImpl::HasSubMenu(
    /* [out] */ Boolean* has) {
    *has = mSubMenu != NULL;
    return NOERROR;
}

ECode CMenuItemImpl::SetSubMenu(
    /* [in] */ ISubMenu* subMenu) {
    //TODO
    /*
    if ((mMenu != NULL) && (mMenu instanceof SubMenu)) {
        throw new UnsupportedOperationException(
        "Attempt to add a sub-menu to a sub-menu.");
    }
    */

    mSubMenu = subMenu;

    String title;
    GetTitle(&title);
    ISubMenu* tmp;
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(title, (ICharSequence**)&csq);
    subMenu->SetHeaderTitleEx(csq, &tmp);

    return NOERROR;
}

//@ViewDebug.CapturedViewProperty
ECode CMenuItemImpl::GetTitle(
    /* [out] */ String* title) {
    *title = mTitle;
    return NOERROR;
}

/**
 * Gets the title for a particular {@link ItemView}
 *
 * @param itemView The ItemView that is receiving the title
 * @return Either the title or condensed title based on what the ItemView
 *         prefers
 */
ECode CMenuItemImpl::GetTitleForItemView(
    /* [in] */ IMenuItemView* itemView,
    /* [out] */ String* title) {
    Boolean supported = FALSE;
    itemView->PrefersCondensedTitle(&supported);
    if ((itemView != NULL) && supported) {
        return GetTitleCondensed(title);
    }

    return GetTitle(title);
}

ECode CMenuItemImpl::SetTitle(
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuItem** menuItem) {
    if (title != NULL) {
        title->ToString(&mTitle);
    }

    for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        // If the item view prefers a condensed title, only set this title if there
        // is no condensed title for this item
        if (!HasItemView(i)) {
            continue;
        }

        AutoPtr<IMenuItemView> itemView = mItemViews[i];
        Boolean supported = FALSE;
        itemView->PrefersCondensedTitle(&supported);

        if (!supported || mTitleCondensed == NULL) {
            //TODO
            //itemView->SetTitle(title);
        }
    }

    if (mSubMenu != NULL) {
        ISubMenu* subMenu;
        mSubMenu->SetHeaderTitleEx(title, &subMenu);
    }

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetTitleEx(
    /* [in] */ Int32 title,
    /* [out] */ IMenuItem** menuItem) {
    String str;

    AutoPtr<IContext> context;
    mMenu->GetContext((IContext**) &context);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);

    res->GetString(title, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);
    return SetTitle(csq, menuItem);
}

ECode CMenuItemImpl::GetTitleCondensed(
    /* [out] */ String* title) {
    *title = mTitle;
    if (mTitleCondensed != NULL) {
        *title = mTitleCondensed;
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetTitleCondensed(
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuItem** menuItem) {
    if (title == NULL) {
        return NOERROR;
    }

    title->ToString(&mTitleCondensed);

    // Could use getTitle() in the loop below, but just cache what it would do here
    if (title == NULL) {
        title->ToString(&mTitle);
    }

    for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        // Refresh those item views that prefer a condensed title
        Boolean supported = FALSE;
        mItemViews[i]->PrefersCondensedTitle(&supported);

        if (HasItemView(i) && supported) {
            //TODO
            //mItemViews[i]->SetTitle(title);
        }
    }

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::GetIcon(
    /* [out] */ IDrawable** retDrawable) {

    if (mIconDrawable != NULL) {
        *retDrawable = mIconDrawable;
        return NOERROR;
    }

    if (mIconResId != NO_ICON) {
        AutoPtr<IContext> context;
        mMenu->GetContext((IContext**) &context);
        AutoPtr<IResources> res;
        context->GetResources((IResources**)&res);

        AutoPtr<IDrawable> drawable;
        res->GetDrawable(mIconResId, (IDrawable**)&drawable);

        *retDrawable = drawable;
        if (*retDrawable != NULL) {
            (*retDrawable)->AddRef();
        }

        return NOERROR;
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetIcon(
	  /* [in] */ IDrawable* icon,
    /* [out] */ IMenuItem** menuItem) {
    mIconResId = NO_ICON;
    mIconDrawable = icon;
    SetIconOnViews(icon);
    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetIconEx(
	  /* [in] */Int32 iconResId,
    /* [out] */ IMenuItem** menuItem) {
    mIconDrawable = NULL;
    mIconResId = iconResId;

    // If we have a view, we need to push the Drawable to them
    if (HaveAnyOpenedIconCapableItemViews()) {
        AutoPtr<IContext> context;
        mMenu->GetContext((IContext**) &context);

        AutoPtr<IResources> res;
        context->GetResources((IResources**)&res);
        AutoPtr<IDrawable> drawable;
        res->GetDrawable(iconResId, (IDrawable**)&drawable);

        AutoPtr<IDrawable> tempDrawable = iconResId != NO_ICON ? drawable : NULL;
        SetIconOnViews(tempDrawable);
    }

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetIconOnViews(IDrawable* icon) {
    for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        // Refresh those item views that are able to display an icon
        Boolean show = FALSE;
        mItemViews[i]->ShowsIcon(&show);
        if (HasItemView(i) && show) {
            mItemViews[i]->SetIcon(icon);
        }
    }

    return NOERROR;
}

Boolean CMenuItemImpl::HaveAnyOpenedIconCapableItemViews() {
    Boolean show = FALSE;

    for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        mItemViews[i]->ShowsIcon(&show);

        if (HasItemView(i) && show) {
            return TRUE;
        }
    }

    return FALSE;
}

ECode CMenuItemImpl::IsCheckable(
    /* [out] */ Boolean* checkable) {
    *checkable = (mFlags & CHECKABLE) == CHECKABLE;

    return NOERROR;
}

ECode CMenuItemImpl::SetCheckable(
    /* [in] */ Boolean checkable,
    /* [out] */ IMenuItem** menuItem) {
    const int oldFlags = mFlags;
    mFlags = (mFlags & ~CHECKABLE) | (checkable ? CHECKABLE : 0);
    if (oldFlags != mFlags) {
        for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
            if (HasItemView(i)) {
                mItemViews[i]->SetCheckable(checkable);
            }
        }
    }

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetExclusiveCheckable(Boolean exclusive)
{
    mFlags = (mFlags&~EXCLUSIVE) | (exclusive ? EXCLUSIVE : 0);
    return NOERROR;
}

ECode CMenuItemImpl::IsExclusiveCheckable(
    /* [out] */ Boolean* checkable) {
    *checkable = (mFlags & EXCLUSIVE) != 0;

    return NOERROR;
}

ECode CMenuItemImpl::IsChecked(
    /* [out] */ Boolean* checked) {
    *checked = (mFlags & CHECKED) == CHECKED;

    return NOERROR;
}

ECode CMenuItemImpl::SetChecked(
        /* [in] */ Boolean checked,
        /* [out] */ IMenuItem** menuItem) {
    if ((mFlags & EXCLUSIVE) != 0) {
        // Call the method on the Menu since it knows about the others in this
        // exclusive checkable group
        mMenu->SetExclusiveItemChecked(this);
    } else {
        SetCheckedInt(checked);
    }

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetCheckedInt(
    /* [in] */ Boolean checked) {
    const int oldFlags = mFlags;
    mFlags = (mFlags & ~CHECKED) | (checked ? CHECKED : 0);
    if (oldFlags != mFlags) {
        for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
            if (HasItemView(i)) {
                mItemViews[i]->SetChecked(checked);
            }
        }
    }

    return NOERROR;
}

ECode CMenuItemImpl::IsVisible(
    /* [out] */ Boolean* visiable) {
    *visiable = (mFlags & HIDDEN) == 0;
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);

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
ECode CMenuItemImpl::SetVisibleInt(
    /* [in] */ Boolean shown,
    /* [out] */ Boolean* visiable) {
    const int oldFlags = mFlags;
    mFlags = (mFlags & ~HIDDEN) | (shown ? 0 : HIDDEN);
    *visiable = oldFlags != mFlags;

    return NOERROR;
}

ECode CMenuItemImpl::SetVisible(
    /* [in] */ Boolean shown,
    /* [out] */ IMenuItem** menuItem) {

    // Try to set the shown state to the given state. If the shown state was changed
    // (i.e. the previous state isn't the same as given state), notify the parent menu that
    // the shown state has changed for this item

    Boolean visiable;
    SetVisibleInt(shown, &visiable);
    if (visiable) {
        mMenu->OnItemVisibleChanged(this);
    }

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::SetOnMenuItemClickListener(
        /* [in] */ IOnMenuItemClickListener* clickListener,
        /* [out] */ IMenuItem** menuItem) {
    mClickListener = clickListener;

    *menuItem = this;
    if (*menuItem != NULL) {
        (*menuItem)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::GetItemView(
    /* [in] */ Int32 menuType,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view) {
    if (!HasItemView(menuType)) {
        CreateItemView(menuType, parent, (IMenuItemView**)(&mItemViews[menuType]));
    }

    *view = (IView*) mItemViews[menuType];
    if (*view != NULL) {
        (*view)->AddRef();
    }

    return NOERROR;
}

/**
 * Create and initializes a menu item view that implements {@link MenuView.ItemView}.
 * @param menuType The type of menu to get a View for (must be one of
 *            {@link MenuBuilder#TYPE_ICON}, {@link MenuBuilder#TYPE_EXPANDED},
 *            {@link MenuBuilder#TYPE_SUB}, {@link MenuBuilder#TYPE_CONTEXT}).
 * @return The inflated {@link MenuView.ItemView} that is ready for use
 */
ECode CMenuItemImpl::CreateItemView(
    /* [in] */ Int32 menuType,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IMenuItemView** itemView) {
    // Create the MenuView
    ILayoutInflater* layoutInflater;
    GetLayoutInflater(menuType, &layoutInflater);

    if (layoutInflater != NULL) {
        layoutInflater->InflateEx2(CMenuBuilder::ITEM_LAYOUT_RES_FOR_TYPE[menuType], parent, FALSE, (IView**)&itemView);
        ((IMenuItemView*)itemView)->Initialize(this, menuType);
    }

    *itemView = (IMenuItemView*)itemView;
    if (*itemView != NULL) {
        (*itemView)->AddRef();
    }

    return NOERROR;
}

ECode CMenuItemImpl::ClearItemViews() {
    //TODO
    //for (int i = mItemViews.length - 1; i >= 0; i--) {
    for (int i = CMenuBuilder::NUM_TYPES - 1; i >= 0; i--) {
        mItemViews[i] = NULL;
    }

    return NOERROR;
}

//@Override
ECode CMenuItemImpl::ToString(
    /* [out] */ String* string) {
    *string = mTitle;
    return NOERROR;
}

ECode CMenuItemImpl::SetMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo) {
    mMenuInfo = menuInfo;

    return NOERROR;
}

ECode CMenuItemImpl::GetMenuInfo(
    /* [out] */ IContextMenuInfo** menuInfo) {
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
ECode CMenuItemImpl::GetLayoutInflater(
    /* [in] */ Int32 menuType,
    /* [out] */ ILayoutInflater** layoutInflater) {
    AutoPtr<IMenuBuilderType> menuTypePtr;
    mMenu->GetMenuType(menuType, (IMenuBuilderType**) &menuTypePtr);
    if (menuTypePtr != NULL) {
        menuTypePtr->GetInflater(layoutInflater);
    }

    return NOERROR;
}

/**
 * @return Whether the given menu type should show icons for menu items.
 */
ECode CMenuItemImpl::ShouldShowIcon(
    /* [in] */ Int32 menuType,
    /* [out] */ Boolean* show) {
    Boolean visiable;
    mMenu->GetOptionalIconsVisible(&visiable);
    *show = menuType == CMenuBuilder::TYPE_ICON || visiable;

    return NOERROR;
}

ECode CMenuItemImpl::OnMenuItemClick(
        /* [in] */ IMenuItem* item,
        /* [in] */ Elastos::Boolean* click) {

    return NOERROR;
}
