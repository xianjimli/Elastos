
#include "view/menu/MenuBuilder.h"
#include "view/menu/CSubMenuBuilder.h"
#include "view/ElKeyCharacterMap.h"
#include "widget/CMenuAdapter.h"
#include "utils/CObjectIntegerMap.h"
#include "view/menu/CMenuType.h"
#include <stdio.h>

const String MenuBuilder::LOGTAG = String("MenuBuilder");

/** The number of different menu types */
const Int32 MenuBuilder::NUM_TYPES = 3;
/** The menu type that represents the icon menu view */
const Int32 MenuBuilder::TYPE_ICON = 0;
/** The menu type that represents the expanded menu view */
const Int32 MenuBuilder::TYPE_EXPANDED = 1;

/**
 * The menu type that represents a menu dialog. Examples are context and sub
 * menus. This menu type will not have a corresponding MenuView, but it will
 * have an ItemView.
 */

const Int32 MenuBuilder::TYPE_DIALOG = 2;

const String MenuBuilder::VIEWS_TAG = String("android:views");

// Order must be the same order as the TYPE_*
const Int32 MenuBuilder::THEME_RES_FOR_TYPE[3] = {
    0x010300a6,//com.android.internal.R.style.Theme_IconMenu,
    0x010300a7,//com.android.internal.R.style.Theme_ExpandedMenu,
    0,
};

// Order must be the same order as the TYPE_*
const Int32 MenuBuilder::LAYOUT_RES_FOR_TYPE[3] = {
    0x01090029,//com.android.internal.R.layout.icon_menu_layout,
    0x01090024,//com.android.internal.R.layout.expanded_menu_layout,
    0,
};

// Order must be the same order as the TYPE_*
const Int32 MenuBuilder::ITEM_LAYOUT_RES_FOR_TYPE[3] = {
    0x01090028,//com.android.internal.R.layout.icon_menu_item_layout,
    0x0109003f,//com.android.internal.R.layout.list_menu_item_layout,
    0x0109003f//com.android.internal.R.layout.list_menu_item_layout,
};

const Int32  MenuBuilder::sCategoryToOrder[6] = {
    1, /* No category */
    4, /* CONTAINER */
    5, /* SYSTEM */
    3, /* SECONDARY */
    2, /* ALTERNATIVE */
    0, /* SELECTED_ALTERNATIVE */
};

MenuBuilder::MenuBuilder(
    /* [in] */ IContext* context) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    mMenuTypes = new AutoPtr<IMenuBuilderType>[NUM_TYPES];
    memset(mMenuTypes, 0, sizeof(IMenuBuilderType*) * NUM_TYPES);

    mContext = context;
    context->GetResources(&mResources);

    mItems = new List<AutoPtr<IMenuItem> >();

    mVisibleItems = new List<AutoPtr<IMenuItem> >();
    mIsVisibleItemsStale = TRUE;

    mPreventDispatchingItemsChanged = FALSE;
    mOptionalIconsVisible = FALSE;
    mHeaderTitle = NULL;

    AutoPtr<IConfiguration> config;
    mResources->GetConfiguration((IConfiguration**)&config);

    if (config != NULL) {
        //TODO mShortcutsVisible = config->keyboard != Configuration.KEYBOARD_NOKEYS);
        mShortcutsVisible = TRUE;
    }
}

MenuBuilder::MenuBuilder() {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
}

MenuBuilder::~MenuBuilder() {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (mItems != NULL) {
        delete mItems;
        mItems = NULL;
    }

    if (mMenuTypes != NULL) {
        // for (Int32 i = 0; i < NUM_TYPES; i++) {
        //     if (mMenuTypes[i] != NULL) {
        //         mMenuTypes[i]->Release();
        //     }
        // }

        delete[] mMenuTypes;
        mMenuTypes = NULL;
    }

    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
}

ECode MenuBuilder::SetCallback(
    /* [in] */ IMenuBuilderCallback* callback) {
    mCallback = callback;

    return NOERROR;
}

PInterface MenuBuilder::Probe(
    /* [in]  */ REIID riid) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (riid == EIID_IMenuBuilder) {
        return (IMenuBuilder*)this;
    }
    if (riid == EIID_IMenu) {
        return (IMenu*)this;
    }

    return NULL;
}

ECode MenuBuilder::GetMenuType(
    /* [in] */ Int32 menuType,
    /* [out] */ IMenuBuilderType** type) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (mMenuTypes[menuType] == NULL) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        CMenuType::New(menuType, (IMenuBuilder*)this->Probe(EIID_IMenuBuilder), type);

        mMenuTypes[menuType] = *type;
        if (*type != NULL) {
            (*type)->AddRef();
        }
    }

    return NOERROR;
}

/**
 * Gets a menu View that contains this menu's items.
 *
 * @param menuType The type of menu to get a View for (must be one of
 *            {@link #TYPE_ICON}, {@link #TYPE_EXPANDED},
 *            {@link #TYPE_DIALOG}).
 * @param parent The ViewGroup that provides a set of LayoutParams values
 *            for this menu view
 * @return A View for the menu of type <var>menuType</var>
 */
ECode MenuBuilder::GetMenuView(
    /* [in] */ Int32 menuType,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IMenuView** menuView) {
    Boolean has;
    mMenuTypes[TYPE_ICON]->HasMenuView(&has);

    AutoPtr<IMenuBuilderType> type;
    GetMenuType(TYPE_ICON, (IMenuBuilderType**)&type);

    AutoPtr<IMenuView> tempMenuView;

    if (menuType == TYPE_EXPANDED
            && (mMenuTypes[TYPE_ICON] == NULL || !has)) {
        type->GetMenuView(parent, (IMenuView**) &tempMenuView);
    }

    GetMenuType(menuType, (IMenuBuilderType**)&type);
    return type->GetMenuView(parent, menuView);
}

ECode MenuBuilder::GetNumIconMenuItemsShown(
    /* [out] */ Int32* state) {
    AutoPtr<IViewGroup> parent = NULL;
    Boolean has;
    mMenuTypes[TYPE_ICON]->HasMenuView(&has);

    if (!has) {
        /*
         * There isn't an icon menu view instantiated, so when we get it
         * below, it will lazily instantiate it. We should pass a proper
         * parent so it uses the layout_ attributes present in the XML
         * layout file.
         */
         mMenuTypes[TYPE_EXPANDED]->HasMenuView(&has);
        if (has) {
            AutoPtr<IView> expandedMenuView;
            mMenuTypes[TYPE_EXPANDED]->GetMenuView(NULL, (IMenuView**) &expandedMenuView);
            expandedMenuView->GetParent((IViewParent**) &parent);
        }
    }

    //TODO
    //return ((IconMenuView) GetMenuView(TYPE_ICON, parent))->GetNumActualItemsShown(state);
    return NOERROR;
}

/**
 * Clears the cached menu views. Call this if the menu views need to another
 * layout (for example, if the screen size has changed).
 */
ECode MenuBuilder::ClearMenuViews() {
    for (Int32 i = NUM_TYPES - 1; i >= 0; i--) {
        if (mMenuTypes[i] != NULL) {
            mMenuTypes[i]->SetMenuView(NULL);
        }
    }

    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    Boolean hasSub = FALSE;
    ISubMenu* submenu = NULL;
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        item->HasSubMenu(&hasSub);
        if (hasSub) {
            item->GetSubMenu(&submenu);
            if (submenu != NULL) {
                ((CSubMenuBuilder*)submenu)->ClearMenuViews();
            }
        }

        item->ClearItemViews();
    }

    return NOERROR;
}

/**
 * Adds an item to the menu.  The other add methods funnel to this.
 */
ECode MenuBuilder::AddInternal(
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuItem** menuItem) {
    printf("==== File: %s, Line: %d ====, FUNC : %s, group=[%d], id=[%d], od=[%d].\n", __FILE__, __LINE__, __FUNCTION__, group, id, categoryOrder);
    Int32 ordering = GetOrdering(categoryOrder);

    //TODO:[suro]
    //AutoPtr<CMenuItemImpl> itemImpl = NULL;
    // CMenuItemImpl::NewByFriend((IMenuBuilder*)this->Probe(EIID_IMenuBuilder), group, id, categoryOrder
    //         , ordering, title, (CMenuItemImpl**)&itemImpl);

    // if (itemImpl != NULL) {
    //     printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    //     *menuItem = (IMenuItem*)(itemImpl->Probe(EIID_IMenuItem));
    // }

    AutoPtr<IMenuItem> item = NULL;
    CMenuItemImpl::New((IMenuBuilder*)this->Probe(EIID_IMenuBuilder), group, id, categoryOrder
            , ordering, title, (IMenuItem**)&item);

    if (mCurrentMenuInfo != NULL) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        // Pass along the current menu info
        if (item != NULL) {
            item->SetMenuInfo(mCurrentMenuInfo);
        }
    }

    Int32 pos = 0;
    FindInsertIndex(mItems, ordering, &pos);
    mItems->Insert(pos, item);
    //mItems->Insert(pos, item);
    OnItemsChanged(FALSE);

    (*menuItem) = item;
    if ((*menuItem) != NULL) {
        (*menuItem)->AddRef();
    }

    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    return NOERROR;
}

ECode MenuBuilder::Add(
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuItem** item) {
    return AddInternal(0, 0, 0, title, item);
}

ECode MenuBuilder::Add(
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuItem** item) {
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);
    return AddInternal(0, 0, 0, csq, item);
}

ECode MenuBuilder::Add(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ ICharSequence* title,
        /* [out] */ IMenuItem** item) {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    return AddInternal(groupId, itemId, order, title, item);
}

ECode MenuBuilder::Add(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ Int32 titleRes,
        /* [out] */ IMenuItem** item) {
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddInternal(groupId, itemId, order, csq, item);
}

ECode MenuBuilder::AddSubMenu(
    /* [in] */ ICharSequence* title,
    /* [out] */ ISubMenu** subMenu) {
    return AddSubMenu(0, 0, 0, title, subMenu);
}

ECode MenuBuilder::AddSubMenu(
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** subMenu) {
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddSubMenu(0, 0, 0, csq, subMenu);
}

ECode MenuBuilder::AddSubMenu(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence*  title,
    /* [out] */ ISubMenu** subMenuOut) {
    AutoPtr<IMenuItem> item;
    AddInternal(groupId, itemId, order, title, (IMenuItem**) &item);
    AutoPtr<ISubMenu> subMenu;
    CSubMenuBuilder::New(mContext, (IMenuBuilder*)this->Probe(EIID_IMenuBuilder)
                , (IMenuItem*)item, (ISubMenu**) &subMenu);

    if (item != NULL) {
        item->SetSubMenu(subMenu);
    }

    *subMenuOut = subMenu;
    if (subMenu != NULL) {
        subMenu->AddRef();
    }

    return NOERROR;
}

ECode MenuBuilder::AddSubMenu(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** subMenu) {
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddSubMenu(groupId, itemId, order, csq, subMenu);
}

ECode MenuBuilder::AddIntentOptions(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ IComponentName* caller,
    /* [in] */ const ArrayOf<IIntent*>& specifics,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems,
    /* [out] */ Int32* count) {
//TODO
//    PackageManager pm = mContext->GetPackageManager();
//    const List<ResolveInfo> lri =
//            pm.queryIntentActivityOptions(caller, specifics, Int32ent, 0);
//    const Int32 N = lri != NULL ? lri.size() : 0;
//
//    if ((flags & FLAG_APPEND_TO_GROUP) == 0) {
//        removeGroup(group);
//    }
//
//    for (Int32 i=0; i<N; i++) {
//        const ResolveInfo ri = lri.get(i);
//        Intent rIntent = new Intent(
//            ri.specificIndex < 0 ? intent : specifics[ri.specificIndex]);
//        rIntent.setComponent(new ComponentName(
//                ri.activityInfo.applicationInfo.packageName,
//                ri.activityInfo.name));
//        const MenuItem item = add(group, id, categoryOrder, ri.loadLabel(pm))
//                .setIcon(ri.loadIcon(pm))
//                .setIntent(rIntent);
//        if (outSpecificItems != NULL && ri.specificIndex >= 0) {
//            outSpecificItems[ri.specificIndex] = item;
//        }
//    }
//
//    *count = N;
    return NOERROR;
}

ECode MenuBuilder::RemoveItem(
    /* [in] */ Int32 id) {
    Int32 index;
    FindItemIndex(id, &index);
    RemoveItemAtInt(index, TRUE);

    return NOERROR;
}

ECode MenuBuilder::RemoveGroup(
    /* [in] */ Int32 group) {
    Int32 i;
    FindGroupIndex(group, &i);

    if (i >= 0) {
        const Int32 maxRemovable = mItems->GetSize() - i;
        Int32 numRemoved = 0;
        Int32 groupid;

        Int32 index = 0;
        List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
        for (; iter != mItems->End(); ++iter) {
            if(index == i) {
                AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
                item->GetGroupId(&groupid);
            }
        }

        while ((numRemoved++ < maxRemovable) && (groupid == group)) {
            // Don't force update for each one, this method will do it at the end
            RemoveItemAtInt(i, FALSE);
        }

        // Notify menu views
        OnItemsChanged(FALSE);
    }

    return NOERROR;
}

/**
 * Remove the item at the given index and optionally forces menu views to
 * update.
 *
 * @param index The index of the item to be removed. If this index is
 *            invalid an exception is thrown.
 * @param updateChildrenOnMenuViews Whether to force update on menu views.
 *            Please make sure you eventually call this after your batch of
 *            removals.
 */
ECode MenuBuilder::RemoveItemAtInt(
    /* [in] */ Int32 index,
    /* [in] */ Boolean updateChildrenOnMenuViews) {
    if ((index < 0) || (index >= mItems->GetSize())) return NOERROR;

    mItems->Remove(index);

    if (updateChildrenOnMenuViews) OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuBuilder::RemoveItemAt(
    /* [in] */ Int32 index) {
    return RemoveItemAtInt(index, TRUE);
}

ECode MenuBuilder::ClearAll() {
    mPreventDispatchingItemsChanged = TRUE;
    Clear();
    ClearHeader();
    mPreventDispatchingItemsChanged = FALSE;
    OnItemsChanged(TRUE);

    return NOERROR;
}

ECode MenuBuilder::Clear() {
    mItems->Clear();

    OnItemsChanged(TRUE);

    return NOERROR;
}

ECode MenuBuilder::SetExclusiveItemChecked(
    /* [in] */ IMenuItem* item) {
    Int32 group;
    item->GetGroupId(&group);

    Int32 groupid;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    Boolean isECheckable;
    Boolean isCheckable;
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> curItem = (AutoPtr<IMenuItem>)(*iter);
        curItem->GetGroupId(&groupid);

        if (groupid == group) {
            curItem->IsExclusiveCheckable(&isECheckable);
            if (!isECheckable) continue;

            curItem->IsCheckable(&isCheckable);
            if (!isCheckable) continue;

            // Check the item meant to be checked, uncheck the others (that are in the group)
            curItem->SetCheckedInt(curItem.Get() == item);
        }
    }

    return NOERROR;
}

ECode MenuBuilder::SetGroupCheckable(
    /* [in] */ Int32 group,
    /* [in] */ Boolean checkable,
    /* [in] */ Boolean exclusive) {
    Int32 groupid;
    AutoPtr<IMenuItem> tempItem;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> curItem = (AutoPtr<IMenuItem>)(*iter);
        curItem->GetGroupId(&groupid);

        if (groupid == group) {
            curItem->SetExclusiveCheckable(exclusive);
            curItem->SetCheckable(checkable, (IMenuItem**) &tempItem);
        }
    }

    return NOERROR;
}

ECode MenuBuilder::SetGroupVisible(
    /* [in] */ Int32 group,
    /* [in] */ Boolean visible) {
    // We handle the notification of items being changed ourselves, so we use setVisibleInt rather
    // than setVisible and at the end notify of items being changed

    Boolean changedAtLeastOneItem = FALSE;

    Int32 groupid;
    Boolean retVisiable;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> curItem = (AutoPtr<IMenuItem>)(*iter);
        curItem->GetGroupId(&groupid);

        if (groupid == group) {
            curItem->SetVisibleInt(visible, &retVisiable);
            if (retVisiable) changedAtLeastOneItem = TRUE;
        }
    }

    if (changedAtLeastOneItem) OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuBuilder::SetGroupEnabled(
    /* [in] */ Int32 group,
    /* [in] */ Boolean enabled) {
    Int32 groupid;
    AutoPtr<IMenuItem> tempItem;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> curItem = (AutoPtr<IMenuItem>)(*iter);
        curItem->GetGroupId(&groupid);

        if (groupid == group) {
            curItem->SetEnabled(enabled, (IMenuItem**) &tempItem);
        }
    }

    return NOERROR;
}

ECode MenuBuilder::HasVisibleItems(
    /* [out] */ Boolean* result) {
    *result = FALSE;
    printf("==== File: %s, Line: %d ====, FUNC : %s, result = [%d].\n", __FILE__, __LINE__, __FUNCTION__, *result);
    Boolean visible = FALSE;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        printf("==== File: %s, Line: %d ====, FUNC : %s, result = [%d].\n", __FILE__, __LINE__, __FUNCTION__, *result);

        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        printf("==== File: %s, Line: %d ====, FUNC : %s, result = [%d].\n", __FILE__, __LINE__, __FUNCTION__, *result);

        if (item != NULL) {
            printf("==== File: %s, Line: %d ====, FUNC : %s, result = [%d].\n", __FILE__, __LINE__, __FUNCTION__, *result);
            item->IsVisible(&visible);
            printf("==== File: %s, Line: %d ====, FUNC : %s, result = [%d].\n", __FILE__, __LINE__, __FUNCTION__, *result);

            if (visible) {
                *result = TRUE;
                printf("==== File: %s, Line: %d ====, FUNC : %s, result = [%d].\n", __FILE__, __LINE__, __FUNCTION__, *result);
                return NOERROR;
            }
        }
        else {
            printf("==== File: %s, Line: %d ====, FUNC : %s, result = [%d].\n", __FILE__, __LINE__, __FUNCTION__, *result);
        }
    }

    printf("==== File: %s, Line: %d ====, FUNC : %s, result = [%d].\n", __FILE__, __LINE__, __FUNCTION__, *result);
    return NOERROR;
}

ECode MenuBuilder::FindItem(
    /* [in] */ Int32 id,
    /* [out] */ IMenuItem** itemOut) {
    Int32 groupid;
    *itemOut = NULL;
    Boolean hasSub = FALSE;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        item->GetGroupId(&groupid);
        item->HasSubMenu(&hasSub);

        if (groupid == id) {
            *itemOut = item;
            if ((*itemOut) != NULL) {
                (*itemOut)->AddRef();
            }

            return NOERROR;
        } else if (hasSub) {
            AutoPtr<ISubMenu> subMenu;
            item->GetSubMenu((ISubMenu**) &subMenu);
            AutoPtr<IMenuItem> possibleItem;
            subMenu->FindItem(id, (IMenuItem**) &possibleItem);

            if (possibleItem != NULL) {
                *itemOut = possibleItem;
                if ((*itemOut) != NULL) {
                    (*itemOut)->AddRef();
                }

                return NOERROR;
            }
        }
    }

    if ((*itemOut) != NULL) {
        (*itemOut)->AddRef();
    }

    return NOERROR;
}

ECode MenuBuilder::FindItemIndex(
    /* [in] */ Int32 id,
    /* [out] */ Int32* index) {
    *index = -1;
    Int32 groupid;
    Int32 pos = 0;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        item->GetGroupId(&groupid);

        if (groupid == id) {
            *index = pos;
            return NOERROR;
        }

        pos ++;
    }

    return NOERROR;
}

ECode MenuBuilder::FindGroupIndex(
    /* [in] */ Int32 group,
    /* [out] */ Int32* index) {
    return FindGroupIndex(group, 0, index);
}

ECode MenuBuilder::FindGroupIndex(
    /* [in] */ Int32 group,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index) {
    if (start < 0) {
        start = 0;
    }

    *index = -1;
    Int32 groupid;
    Int32 pos = start;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        item->GetGroupId(&groupid);

        if (groupid == group) {
            *index = pos;
            return NOERROR;
        }

        pos ++;
    }

    return NOERROR;
}

ECode MenuBuilder::Size(
    /* [out] */ Int32* size) {
    *size = mItems->GetSize();;
    return NOERROR;
}

/** {@inheritDoc} */
ECode MenuBuilder::GetItem(
    /* [in] */ Int32 index,
    /* [out] */ IMenuItem** retItem) {
    Int32 pos = 0;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        if(index == pos) {
            *retItem = (AutoPtr<IMenuItem>)(*iter);
            if ((*retItem) != NULL) {
                (*retItem)->AddRef();
            }

            break;
        }

        pos  ++;
    }

    return NOERROR;
}

ECode MenuBuilder::IsShortcutKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isShortcutKey) {
    AutoPtr<IMenuItem> menuItem;
    FindItemWithShortcutForKey(keyCode, event, (IMenuItem**) &menuItem);
    *isShortcutKey = menuItem != NULL;
    return NOERROR;
}

ECode MenuBuilder::SetQwertyMode(
    /* [in] */ Boolean isQwerty) {
    mQwertyMode = isQwerty;
    Boolean qwertyMode;
    IsShortcutsVisible(&qwertyMode);

    RefreshShortcuts(qwertyMode, isQwerty);

    return NOERROR;
}

/**
 * Returns the ordering across all items. This will grab the category from
 * the upper bits, find out how to order the category with respect to other
 * categories, and combine it with the lower bits.
 *
 * @param categoryOrder The category order for a particular item (if it has
 *            not been or/add with a category, the default category is
 *            assumed).
 * @return An ordering Integer that can be used to order this item across
 *         all the items (even from other categories).
 */
Int32 MenuBuilder::GetOrdering(
    /* [in] */ Int32 categoryOrder)
{
    const Int32 index = (categoryOrder & MENU_CATEGORY_MASK) >> MENU_CATEGORY_SHIFT;

    //if (index < 0 || index >= sCategoryToOrder.length) {
    if (index < 0 || index >= 6) {
        //TODO throw new IllegalArgumentException("order does not contain a valid category.");
    }

    return (sCategoryToOrder[index] << MENU_CATEGORY_SHIFT) | (categoryOrder & MENU_USER_MASK);
}

/**
 * @return whether the menu shortcuts are in qwerty mode or not
 */
ECode MenuBuilder::IsQwertyMode(
    /* [out] */ Boolean* qwertyMode) {
    *qwertyMode = mQwertyMode;
    return NOERROR;
}

/**
 * Refreshes the shortcut labels on each of the displayed items.  Passes the arguments
 * so submenus don't need to call their parent menu for the same values.
 */
ECode MenuBuilder::RefreshShortcuts(
    /* [in] */ Boolean shortcutsVisible,
    /* [in] */ Boolean qwertyMode) {
    AutoPtr<ISubMenu> submenu;
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    Boolean hasSub = FALSE;
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        item->HasSubMenu(&hasSub);
        if (hasSub) {
            item->GetSubMenu((ISubMenu**) &submenu);
            if (submenu != NULL) {
                ((CSubMenuBuilder*)submenu.Get())->RefreshShortcuts(shortcutsVisible, qwertyMode);
            }
        }

        item->RefreshShortcutOnItemViews2(shortcutsVisible, qwertyMode);
    }

    return NOERROR;
}

/**
 * Sets whether the shortcuts should be visible on menus.  Devices without hardware
 * key input will never make shortcuts visible even if this method is passed 'TRUE'.
 *
 * @param shortcutsVisible Whether shortcuts should be visible (if TRUE and a
 *            menu item does not have a shortcut defined, that item will
 *            still NOT show a shortcut)
 */
ECode MenuBuilder::SetShortcutsVisible(
    /* [in] */ Boolean shortcutsVisible) {
    if (mShortcutsVisible == shortcutsVisible) return NOERROR;

//TODO
//    mShortcutsVisible =
//        (mResources->GetConfiguration().keyboard != Configuration.KEYBOARD_NOKEYS)
//        && shortcutsVisible;

    Boolean qwertyMode;
    IsQwertyMode(&qwertyMode);
    return RefreshShortcuts(mShortcutsVisible, qwertyMode);
}

ECode MenuBuilder::FindInsertIndex(
    /* [in] */ List<AutoPtr<IMenuItem> >* items,
    /* [in] */ Int32 ordering,
    /* [in] */ Int32* index) {
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    Int32 temp = -1;
    *index = 0;
    Int32 pos = items->GetSize();
printf("==== File: %s, Line: %d ====, FUNC : %s, pos==[%d].\n", __FILE__, __LINE__, __FUNCTION__, pos);
    List<AutoPtr<IMenuItem> >::ReverseIterator iter = items->RBegin();
    for (; iter != items->REnd(); ++iter) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        item->GetOrdering(&temp);
        if (temp <= ordering) {
            *index = pos + 1;
            return NOERROR;
        }

        pos --;
    }
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    return NOERROR;
}

ECode MenuBuilder::PerformShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* retHandled) {
    AutoPtr<IMenuItem> item;
    FindItemWithShortcutForKey(keyCode, event, (IMenuItem**) &item);

    Boolean handled = FALSE;

    if (item != NULL) {
        PerformItemAction(item, flags, &handled);
    }

    if ((flags & MENU_FLAG_ALWAYS_PERFORM_CLOSE) != 0) {
        CloseEx(TRUE);
    }

    *retHandled = handled;
    return NOERROR;
}

/*
 * This function will return all the menu and sub-menu items that can
 * be directly (the shortcut directly corresponds) and indirectly
 * (the ALT-enabled char corresponds to the shortcut) associated
 * with the keyCode.
 */
ECode MenuBuilder::FindItemsWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ IObjectContainer** list) {
    Boolean qwerty = FALSE;
    IsQwertyMode(&qwerty);

    Int32 metaState = 0;
    event->GetMetaState(&metaState);

    ElKeyCharacterMap::KeyData* possibleChars = new ElKeyCharacterMap::KeyData();
    // Get the chars associated with the keyCode (i.e using any chording combo)
    Boolean isKeyCodeMapped;
    event->GetKeyData((Handle32)possibleChars, &isKeyCodeMapped);
    // The delete key is not mapped to '\b' so we treat it specially
    if (!isKeyCodeMapped && (keyCode != KeyEvent_KEYCODE_DEL)) {
        return NULL;
    }

    AutoPtr<IObjectContainer> items;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&items));

    // Look for an item whose shortcut is this key.
    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    Boolean hasSub;
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        item->HasSubMenu(&hasSub);
        if (hasSub) {
            AutoPtr<IObjectContainer> subMenuItems;
            AutoPtr<ISubMenu> sMenu;
            item->GetSubMenu((ISubMenu**) &sMenu);

            if (sMenu != NULL) {
                //FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&subMenuItems));
                ((CSubMenuBuilder*)sMenu.Get())->FindItemsWithShortcutForKey(keyCode, event, (IObjectContainer**) &subMenuItems);
            }

            if (subMenuItems != NULL) {
                AutoPtr<IObjectEnumerator> itemIt;
                subMenuItems->GetObjectEnumerator((IObjectEnumerator**)&itemIt);
                Boolean hasNext;
                while(itemIt->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IMenuItem> subItem;
                    itemIt->Current((IInterface**)&subItem);
                    items->Add(subItem);
                }
            }
        }

        Char8 shortcutChar;
        if (qwerty) {
            item->GetAlphabeticShortcut(&shortcutChar);
        }
        else {
            item->GetNumericShortcut(&shortcutChar);
        }

        Boolean isEnabled;
        item->IsEnabled(&isEnabled);

        if (((metaState & (KeyEvent_META_SHIFT_ON | KeyEvent_META_SYM_ON)) == 0) &&
              (shortcutChar != 0) &&
              (shortcutChar == possibleChars->mMeta[0]
                  || shortcutChar == possibleChars->mMeta[2]
                  || (qwerty && shortcutChar == '\b' &&
                      keyCode == KeyEvent_KEYCODE_DEL)) && isEnabled) {
            items->Add(item);
        }
    }

    *list = items;
    if ((*list) != NULL) {
        (*list)->AddRef();
    }

    return NOERROR;
}

/*
 * We want to return the menu item associated with the key, but if there is no
 * ambiguity (i.e. there is only one menu item corresponding to the key) we want
 * to return it even if it's not an exact match; this allow the user to
 * _not_ use the ALT key for example, making the use of shortcuts slightly more
 * user-friendly. An example is on the G1, '!' and '1' are on the same key, and
 * in Gmail, Menu+1 will trigger Menu+! (the actual shortcut).
 *
 * On the other hand, if two (or more) shortcuts corresponds to the same key,
 * we have to only return the exact match.
 */
ECode MenuBuilder::FindItemWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ IMenuItem** menuItem) {
    // Get all items that can be associated directly or indirectly with the keyCode
    AutoPtr<IObjectContainer> items;
    FindItemsWithShortcutForKey(keyCode, event, (IObjectContainer**) &items);

    *menuItem = NULL;
    if (items == NULL) {
        return NOERROR;
    }

    Int32 metaState;
    event->GetMetaState(&metaState);
    ElKeyCharacterMap::KeyData* possibleChars = new ElKeyCharacterMap::KeyData();

    // Get the chars associated with the keyCode (i.e using any chording combo)
    Boolean isKeyCodeMapped;
    event->GetKeyData((Handle32)possibleChars, &isKeyCodeMapped);

    AutoPtr<IObjectEnumerator> itemIt;
    Boolean hasNext;
    items->GetObjectEnumerator((IObjectEnumerator**)&itemIt);
    itemIt->MoveNext(&hasNext);
    // If we have only one element, we can safely returns it
    if (!hasNext) {
        itemIt->Current((IInterface**)&menuItem);
        return NOERROR;
    }

    Boolean qwerty;
    IsQwertyMode(&qwerty);
    items->GetObjectEnumerator((IObjectEnumerator**)&itemIt);

    // If we found more than one item associated with the key,
    // we have to return the exact match
    while(itemIt->MoveNext(&hasNext), hasNext) {
        AutoPtr<IMenuItem> item;
        itemIt->Current((IInterface**)&item);

        if (item != NULL) {
            Char8 shortcutChar;
            if (qwerty) {
                item->GetAlphabeticShortcut(&shortcutChar);
            }
            else {
                item->GetNumericShortcut(&shortcutChar);
            }

            if ((shortcutChar == possibleChars->mMeta[0] &&
                    (metaState & KeyEvent_META_ALT_ON) == 0)
                || (shortcutChar == possibleChars->mMeta[2] &&
                    (metaState & KeyEvent_META_ALT_ON) != 0)
                || (qwerty && shortcutChar == '\b' &&
                    keyCode == KeyEvent_KEYCODE_DEL)) {

                *menuItem = item;
                return NOERROR;
            }
        }
    }

    return NOERROR;
}

ECode MenuBuilder::PerformIdentifierAction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* state) {
    // Look for an item whose identifier is the id.
    AutoPtr<IMenuItem> menuItem;
    FindItem(id, (IMenuItem**) &menuItem);
    return PerformItemAction((IMenuItem*)menuItem, flags, state);
}

ECode MenuBuilder::PerformItemAction(
    /* [in] */ IMenuItem* item,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* state) {
    AutoPtr<IMenuItem> itemImpl = item;

    Boolean enabled;
    itemImpl->IsEnabled(&enabled);

    *state = FALSE;
    if (itemImpl == NULL || !enabled) {
        return NOERROR;
    }

    Boolean invoked;
    itemImpl->Invoke(&invoked);

    Boolean has;
    item->HasSubMenu(&has);
    if (has) {
        CloseEx(FALSE);

        if (mCallback != NULL) {
            // Return TRUE if the sub menu was invoked or the item was invoked previously
            Boolean selected;
            AutoPtr<ISubMenu> submenu;
            item->GetSubMenu((ISubMenu**) &submenu);
            mCallback->OnSubMenuSelected(submenu, &selected);
            invoked = selected || invoked;
        }
    } else {
        if ((flags & MENU_FLAG_PERFORM_NO_CLOSE) == 0) {
            CloseEx(TRUE);
        }
    }

    *state = invoked;

    return NOERROR;
}

/**
 * Closes the visible menu.
 *
 * @param allMenusAreClosing Whether the menus are completely closing (TRUE),
 *            or whether there is another menu coming in this menu's place
 *            (FALSE). For example, if the menu is closing because a
 *            sub menu is about to be shown, <var>allMenusAreClosing</var>
 *            is FALSE.
 */
ECode MenuBuilder::CloseEx(
    /* [in] */ Boolean allMenusAreClosing) {
    AutoPtr<IMenuBuilderCallback> callback;
    GetCallback((IMenuBuilderCallback**) &callback);

    if (callback != NULL) {
        callback->OnCloseMenu((IMenuBuilder*)this->Probe(EIID_IMenuBuilder), allMenusAreClosing);
    }

    return NOERROR;
}

/**
 * Called when an item is added or removed.
 *
 * @param cleared Whether the items were cleared or just changed.
 */
ECode MenuBuilder::OnItemsChanged(Boolean cleared) {
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (!mPreventDispatchingItemsChanged) {
        if (mIsVisibleItemsStale == FALSE) mIsVisibleItemsStale = TRUE;

        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        AutoPtr<IMenuBuilderType>* menuTypes = mMenuTypes;
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        if (menuTypes != NULL) {
            for (Int32 i = 0; i < NUM_TYPES; i++) {
                Boolean has;
                printf("==== File: %s, Line: %d ====, FUNC : %s, menuTypes[%d] = [0x%08x].\n", __FILE__, __LINE__, __FUNCTION__, i, menuTypes[i].Get());
                if (menuTypes[i] != NULL) {
                    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
                    menuTypes[i]->HasMenuView(&has);
                    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
                    if ((menuTypes[i] != NULL) && has) {
                        AutoPtr<IMenuView> menuView;
                        menuTypes[i]->GetMenuViewEx((IMenuView**) &menuView);

                        menuView->UpdateChildren(cleared);
                    }
                }
                else {
                    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
                }
            }
        }
        else {
            printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        }
    }

printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    return NOERROR;
}

/**
 * Called by {@link CMenuItemImpl*} when its visible flag is changed.
 * @param item The item that has gone through a visibility change.
 */
ECode MenuBuilder::OnItemVisibleChanged(IMenuItem* item) {
    // Notify of items being changed
    return OnItemsChanged(FALSE);
}

ECode MenuBuilder::GetVisibleItems(
    /* [out] */ IObjectContainer** list) {
    if (!mIsVisibleItemsStale) {
        return FillData(mVisibleItems, list);
    }

    // Refresh the visible items
    mVisibleItems->Clear();

    AutoPtr<IMenuItem> item;
    Boolean visible;

    List<AutoPtr<IMenuItem> >::Iterator iter = mItems->Begin();
    for (; iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);

        item->IsVisible(&visible);
        if (visible) {
            mVisibleItems->PushBack(item);
        }
    }

    mIsVisibleItemsStale = FALSE;

    return FillData(mVisibleItems, list);
}

ECode MenuBuilder::ClearHeader() {
    mHeaderIcon = NULL;
    mHeaderTitle = NULL;
    mHeaderView = NULL;

    return OnItemsChanged(FALSE);
}

ECode MenuBuilder::SetHeaderInternal(
        /* [in] */ Int32 titleRes,
        /* [in] */ ICharSequence* title,
        /* [in] */ Int32 iconRes,
        /* [in] */ IDrawable* icon,
        /* [in] */ IView* view) {
    AutoPtr<IResources> r;
    GetResources((IResources**) &r);

    if (view != NULL) {
        mHeaderView = view;

        // If using a custom view, then the title and icon aren't used
        mHeaderTitle = NULL;
        mHeaderIcon = NULL;
    } else {
        if (titleRes > 0) {
            String str;
            r->GetString(titleRes, &str);
            CStringWrapper::New(str, (ICharSequence**)&mHeaderTitle);
        } else if (title != NULL) {
            mHeaderTitle = title;
        }

        if (iconRes > 0) {
            r->GetDrawable(iconRes, &mHeaderIcon);
        } else if (icon != NULL) {
            mHeaderIcon = icon;
        }

        // If using the title or icon, then a custom view isn't used
        mHeaderView = NULL;
    }

    // Notify of change
    return OnItemsChanged(FALSE);
}

/**
 * Sets the header's title. This replaces the header view. Called by the
 * builder-style methods of subclasses.
 *
 * @param title The new title.
 * @return This MenuBuilder so additional setters can be called.
 */
ECode MenuBuilder::SetHeaderTitleInt(
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuBuilder** menu) {
    SetHeaderInternal(0, title, 0, NULL, NULL);
    *menu = (IMenuBuilder*)this->Probe(EIID_IMenuBuilder);
    return NOERROR;
}

/**
 * Sets the header's title. This replaces the header view. Called by the
 * builder-style methods of subclasses.
 *
 * @param titleRes The new title (as a resource ID).
 * @return This MenuBuilder so additional setters can be called.
 */
ECode MenuBuilder::SetHeaderTitleInt(
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuBuilder** menu) {
    SetHeaderInternal(titleRes, NULL, 0, NULL, NULL);
    *menu = (IMenuBuilder*)this->Probe(EIID_IMenuBuilder);
    return NOERROR;
}

/**
 * Sets the header's icon. This replaces the header view. Called by the
 * builder-style methods of subclasses.
 *
 * @param icon The new icon.
 * @return This MenuBuilder so additional setters can be called.
 */
ECode MenuBuilder::SetHeaderIconInt(
    /* [in] */ IDrawable* icon,
    /* [out] */ IMenuBuilder** menu) {
    SetHeaderInternal(0, NULL, 0, icon, NULL);
    *menu = (IMenuBuilder*)this->Probe(EIID_IMenuBuilder);
    return NOERROR;
}

/**
 * Sets the header's icon. This replaces the header view. Called by the
 * builder-style methods of subclasses.
 *
 * @param iconRes The new icon (as a resource ID).
 * @return This MenuBuilder so additional setters can be called.
 */
ECode MenuBuilder::SetHeaderIconInt(
    /* [in] */ Int32 iconRes,
    /* [out] */ IMenuBuilder** menu) {
    SetHeaderInternal(0, NULL, iconRes, NULL, NULL);
    *menu = (IMenuBuilder*)this->Probe(EIID_IMenuBuilder);
    return NOERROR;
}

/**
 * Sets the header's view. This replaces the title and icon. Called by the
 * builder-style methods of subclasses.
 *
 * @param view The new view.
 * @return This MenuBuilder so additional setters can be called.
 */
ECode MenuBuilder::SetHeaderViewInt(
    /* [in] */ IView* view,
    /* [out] */ IMenuBuilder** menu) {
    SetHeaderInternal(0, NULL, 0, NULL, view);
    *menu = (IMenuBuilder*)this->Probe(EIID_IMenuBuilder);
    return NOERROR;
}

ECode MenuBuilder::GetHeaderTitle(
    /* [out] */ ICharSequence** title) {
    *title = mHeaderTitle;
    return NOERROR;
}

ECode MenuBuilder::GetHeaderIcon(
    /* [out] */ IDrawable** drawable) {
    *drawable = mHeaderIcon;
    return NOERROR;
}

ECode MenuBuilder::GetHeaderView(
    /* [out] */ IView** headerView) {
    *headerView = mHeaderView;
    return NOERROR;
}

/**
 * Gets the root menu (if this is a submenu, find its root menu).
 * @return The root menu.
 */
ECode MenuBuilder::GetRootMenu(
    /* [out] */ IMenuBuilder** menu) {
    //*menu = (IMenuBuilder*)this->Probe(EIID_IMenuBuilder);
    *menu = (IMenuBuilder*)this;
    return NOERROR;
}

/**
 * Sets the current menu info that is set on all items added to this menu
 * (until this is called again with different menu info, in which case that
 * one will be added to all subsequent item additions).
 *
 * @param menuInfo The extra menu information to add.
 */
ECode MenuBuilder::SetCurrentMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo) {
    mCurrentMenuInfo = menuInfo;

    return NOERROR;
}

/**
 * Gets an adapter for providing items and their views.
 *
 * @param menuType The type of menu to get an adapter for.
 * @return A {@link MenuAdapter} for this menu with the given menu type.
 */
ECode MenuBuilder::GetMenuAdapter(
    /* [in] */ Int32 menuType,
    /* [out] */ IMenuAdapter** menuAdapter) {
    //return new MenuAdapter(menuType);

    return CMenuAdapter::New(menuType, (IMenuBuilder*)this->Probe(EIID_IMenuBuilder), menuAdapter);
}

ECode MenuBuilder::SetOptionalIconsVisible(Boolean visible) {
    mOptionalIconsVisible = visible;

    return NOERROR;
}

ECode MenuBuilder::GetOptionalIconsVisible(
    /* [out] */ Boolean* visible) {
    *visible = mOptionalIconsVisible;
    return NOERROR;
}

ECode MenuBuilder::SaveHierarchyState(
    /* [in] */ IBundle* outState) {
    //SparseArray<Parcelable> viewStates = new SparseArray<Parcelable>();

/*
    AutoPtr<IObjectIntegerMap> viewStates;
    FAIL_RETURN(CObjectIntegerMap::New((IObjectIntegerMap**) &viewStates));

    IMenuBuilderType ** menuTypes = mMenuTypes;
    for (Int32 i = NUM_TYPES - 1; i >= 0; i--) {
        if (menuTypes[i] == NULL) {
            continue;
        }

        Boolean has;
        menuTypes[i]->HasMenuView(&has);
        if (has) {
            AutoPtr<IMenuView> menuView;
            menuTypes[i]->GetMenuView(NULL, (IMenuView**) &menuView);
            menuView->SaveHierarchyState(viewStates);
        }
    }

    outState->PutSparseParcelableArray(VIEWS_TAG, viewStates);
*/

    return E_NOT_IMPLEMENTED;
}

ECode MenuBuilder::GetContext(
    /* [out] */ IContext** context) {
    *context = mContext;
    if (*context != NULL) {
        (*context)->AddRef();
    }

    return NOERROR;
}

ECode MenuBuilder::IsShortcutsVisible(
    /* [out] */ Boolean* qwertyMode) {
    *qwertyMode = mShortcutsVisible;
    return NOERROR;
}

ECode MenuBuilder::GetResources(
    /* [out] */ IResources** resource) {
    *resource = mResources;
    if (*resource != NULL) {
        (*resource)->AddRef();
    }

    return NOERROR;
}

ECode MenuBuilder::GetCallback(
    /* [out] */ IMenuBuilderCallback** callback) {
    *callback = mCallback;
    if (*callback != NULL) {
        (*callback)->AddRef();
    }

    return NOERROR;
}

ECode MenuBuilder::RestoreHierarchyState(
    /* [in] */ IBundle* inState) {
    // Save this for menu views opened later

/*
    SparseArray<Parcelable> viewStates = mFrozenViewStates = inState
            .getSparseParcelableArray(VIEWS_TAG);

    AutoPtr<IObjectIntegerMap> viewStates;

    // Thaw those menu views already open
    IMenuBuilderType** menuTypes = mMenuTypes;
    for (Int32 i = NUM_TYPES - 1; i >= 0; i--) {
        if (menuTypes[i] == NULL) {
            continue;
        }

        Boolean has;
        menuTypes[i]->HasMenuView(&has);
        if (has) {
            AutoPtr<IMenuView> menuView;
            menuTypes[i]->GetMenuView(NULL, (IMenuView**) &menuView);

            if (menuView != NULL) {
                menuView->RestoreHierarchyState(viewStates);
            }
        }
    }
*/
    return E_NOT_IMPLEMENTED;
}

ECode MenuBuilder::FillData(
        /* [in] */ List<AutoPtr<IMenuItem> >* inputData,
        /* [out] */ IObjectContainer** list) {
    if (inputData == NULL || list == NULL || *list == NULL) {
        return E_INVALID_ARGUMENT;
    }

    List<AutoPtr<IMenuItem> >::Iterator iter = inputData->Begin();
    for (; iter != inputData->End(); ++iter) {
        AutoPtr<IMenuItem> item = (AutoPtr<IMenuItem>)(*iter);
        (*list)->Add(item);
    }

    return NOERROR;
}
