
#include "content/CContextThemeWrapper.h"
#include "content/CConfiguration.h"
#include "view/ElKeyCharacterMap.h"
#include "view/menu/MenuBuilderBase.h"
#include "view/menu/MenuItemImpl.h"
#include "view/menu/CIconMenuView.h"
#include "view/menu/SubMenuBuilder.h"


MenuBuilderBase::MenuType::MenuType(
    /* [in] */ Int32 menuType,
    /* [in] */ MenuBuilderBase* host)
    : mMenuType(menuType)
    , mHost(host)
{}

MenuBuilderBase::MenuType::~MenuType()
{}

PInterface MenuBuilderBase::MenuType::Probe(
/* [in] */ REIID riid)
{
    return NULL;
}

UInt32 MenuBuilderBase::MenuType::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MenuBuilderBase::MenuType::Release()
{
    return ElRefBase::Release();
}

ECode MenuBuilderBase::MenuType::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

AutoPtr<ILayoutInflater> MenuBuilderBase::MenuType::GetInflater()
{
    // Create an inflater that uses the given theme for the Views it inflates
    if (mInflater == NULL) {
        AutoPtr<IContextThemeWrapper> wrappedContext;
        CContextThemeWrapper::New(mHost->mContext, MenuBuilderBase::THEME_RES_FOR_TYPE[mMenuType],
                (IContextThemeWrapper**)&wrappedContext);
        AutoPtr<IInterface> service;
        wrappedContext->GetSystemService(Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&service);
        mInflater = ILayoutInflater::Probe(service);
    }

    return mInflater;
}

AutoPtr<IMenuView> MenuBuilderBase::MenuType::GetMenuView(
    /* [in] */ IViewGroup* parent)
{
    if (MenuBuilderBase::LAYOUT_RES_FOR_TYPE[mMenuType] == 0) {
        return NULL;
    }

    Mutex::Autolock lock(&mLock);

    AutoPtr<IMenuView> menuView = mMenuView;

    if (menuView == NULL) {
        AutoPtr<IView> view;
        GetInflater()->InflateEx2(MenuBuilderBase::LAYOUT_RES_FOR_TYPE[mMenuType], parent,
                    FALSE, (IView**)&view);
        menuView = IMenuView::Probe(view);
        menuView->Initialize((IMenuBuilder*)mHost->Probe(EIID_IMenuBuilder), mMenuType);

        //Cache the view
        //mMenuView = new WeakReference<MenuView>(menuView);
        mMenuView = menuView;
        if (mHost->mFrozenViewStates != NULL) {
            IView* view = IView::Probe(mMenuView);
            assert(view != NULL);
            view->RestoreHierarchyState(mHost->mFrozenViewStates);

            // Clear this menu type's frozen state, since we just restored it
            Int32 id = 0;
            view->GetId(&id);
            mHost->mFrozenViewStates->Put(id, NULL);
        }
    }

    return menuView;
}

Boolean MenuBuilderBase::MenuType::HasMenuView()
{
    return mMenuView != NULL;
}


//===================MenuAdapterBase==========================
MenuBuilderBase::MenuAdapterBase::MenuAdapterBase(
    /* [in] */ Int32 menuType,
    /* [in] */ MenuBuilderBase* host)
    : mMenuType(menuType)
    , mHost(host)
{}

Int32 MenuBuilderBase::MenuAdapterBase::GetOffset()
{
    if (mMenuType == MenuBuilderBase::TYPE_EXPANDED) {
        return mHost->GetNumIconMenuItemsShown();
    }
    else {
        return 0;
    }
}

Int32 MenuBuilderBase::MenuAdapterBase::GetCount()
{
    return mHost->GetVisibleItems()->GetSize() - GetOffset();
}

AutoPtr<IInterface> MenuBuilderBase::MenuAdapterBase::GetItem(
    /* [in] */ Int32 position)
{
    AutoPtr<IMenuItemImpl> item = (*mHost->GetVisibleItems())[position + GetOffset()];
    return (IInterface*)item.Get();
}

Int64 MenuBuilderBase::MenuAdapterBase::GetItemId(
    /* [in] */ Int32 position)
{
     // Since a menu item's ID is optional, we'll use the position as an
    // ID for the item in the AdapterView
    return position;
}

AutoPtr<IView> MenuBuilderBase::MenuAdapterBase::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IMenuItemImpl> item = IMenuItemImpl::Probe(GetItem(position));
    AutoPtr<IView> view = ((MenuItemImpl*)item.Get())->GetItemView(mMenuType, parent);
    return view;
}


//===================MenuAdapter==========================
MenuBuilderBase::MenuAdapter::MenuAdapter(
    /* [in] */ Int32 menuType,
    /* [in] */ MenuBuilderBase* host)
    : MenuAdapterBase(menuType, host)
{}

PInterface MenuBuilderBase::MenuAdapter::Probe(
/* [in] */ REIID riid)
{
    if (riid == EIID_IMenuAdapter) {
        return (IMenuAdapter*)this;
    }
    else if (riid == EIID_IBaseAdapter) {
        return (IBaseAdapter*)this;
    }
    else if (riid == EIID_IListAdapter) {
        return (IListAdapter*)this;
    }
    else if (riid == EIID_IAdapter) {
        return (IAdapter*)this;
    }

    return NULL;
}

UInt32 MenuBuilderBase::MenuAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MenuBuilderBase::MenuAdapter::Release()
{
    return ElRefBase::Release();
}

ECode MenuBuilderBase::MenuAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pObject == (IInterface*)(IMenuAdapter*)this) {
        *pIID = EIID_IMenuAdapter;
    }
    else if (pObject == (IInterface*)(IBaseAdapter*)this) {
        *pIID = EIID_IBaseAdapter;
    }
    else if (pObject == (IInterface*)(IListAdapter*)this) {
        *pIID = EIID_IListAdapter;
    }
    else if (pObject == (IInterface*)(IAdapter*)this) {
        *pIID = EIID_IAdapter;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::GetOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    *offset = MenuAdapterBase::GetOffset();
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = MenuAdapterBase::GetCount();
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);

    AutoPtr<IInterface> obj = MenuAdapterBase::GetItem(position);
    *item = obj.Get();
    if (*item != NULL) (*item)->AddRef();

    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);

    *id = MenuAdapterBase::GetItemId(position);
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    AutoPtr<IView> _view = MenuAdapterBase::GetView(position, convertView, parent);
    *view = _view.Get();
    if (*view != NULL) (*view)->AddRef();
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* dso)
{
    return MenuAdapterBase::RegisterDataSetObserver(dso);
}

ECode MenuBuilderBase::MenuAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* dso)
{
    return MenuAdapterBase::UnregisterDataSetObserver(dso);
}

ECode MenuBuilderBase::MenuAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds);

    *hasStableIds = MenuAdapterBase::HasStableIds();
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::GetItemViewType(
    /* [in] */ Elastos::Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);

    *viewType = MenuAdapterBase::GetItemViewType(position);
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = MenuAdapterBase::GetViewTypeCount();
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);

    *isEmpty = MenuAdapterBase::IsEmpty();
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    *enabled = MenuAdapterBase::AreAllItemsEnabled();
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    *enabled = MenuAdapterBase::IsEnabled(position);
    return NOERROR;
}

ECode MenuBuilderBase::MenuAdapter::NotifyDataSetChanged()
{
    return MenuAdapterBase::NotifyDataSetChanged();
}

ECode MenuBuilderBase::MenuAdapter::NotifyDataSetInvalidated()
{
    return MenuAdapterBase::NotifyDataSetInvalidated();
}



extern "C" const InterfaceID EIID_MenuBuilderBase =
        { 0x2dbd4c5b, 0xd8cc, 0x489e, { 0xb3, 0xe3, 0xae, 0xeb, 0x1c, 0x5b, 0x99, 0x42 } };

const CString MenuBuilderBase::LOGTAG = "MenuBuilderBase";

/** The number of different menu types */
const Int32 MenuBuilderBase::NUM_TYPES;
/** The menu type that represents the icon menu view */
const Int32 MenuBuilderBase::TYPE_ICON;
/** The menu type that represents the expanded menu view */
const Int32 MenuBuilderBase::TYPE_EXPANDED;

/**
 * The menu type that represents a menu dialog. Examples are context and sub
 * menus. This menu type will not have a corresponding MenuView, but it will
 * have an ItemView.
 */

const Int32 MenuBuilderBase::TYPE_DIALOG;

const CString MenuBuilderBase::VIEWS_TAG = "android:views";

// Order must be the same order as the TYPE_*
const Int32 MenuBuilderBase::THEME_RES_FOR_TYPE[3] = {
    0x010300a6,//com.android.internal.R.style.Theme_IconMenu,
    0x010300a7,//com.android.internal.R.style.Theme_ExpandedMenu,
    0,
};

// Order must be the same order as the TYPE_*
const Int32 MenuBuilderBase::LAYOUT_RES_FOR_TYPE[3] = {
    0x01090029,//com.android.internal.R.layout.icon_menu_layout,
    0x01090024,//com.android.internal.R.layout.expanded_menu_layout,
    0,
};

// Order must be the same order as the TYPE_*
const Int32 MenuBuilderBase::ITEM_LAYOUT_RES_FOR_TYPE[3] = {
    0x01090028,//com.android.internal.R.layout.icon_menu_item_layout,
    0x0109003f,//com.android.internal.R.layout.list_menu_item_layout,
    0x0109003f//com.android.internal.R.layout.list_menu_item_layout,
};

const Int32  MenuBuilderBase::sCategoryToOrder[6] = {
    1, /* No category */
    4, /* CONTAINER */
    5, /* SYSTEM */
    3, /* SECONDARY */
    2, /* ALTERNATIVE */
    0, /* SELECTED_ALTERNATIVE */
};

MenuBuilderBase::MenuBuilderBase(
    /* [in] */ IContext* context)
    : mQwertyMode(FALSE)
    , mShortcutsVisible(FALSE)
    , mIsVisibleItemsStale(TRUE)
    , mPreventDispatchingItemsChanged(FALSE)
    , mOptionalIconsVisible(FALSE)
{
    mMenuTypes = ArrayOf< AutoPtr<MenuType> >::Alloc(NUM_TYPES);

    mContext = context;
    context->GetResources((IResources**)&mResources);

    AutoPtr<IConfiguration> config;
    mResources->GetConfiguration((IConfiguration**)&config);

    mShortcutsVisible = ((CConfiguration*)config.Get())->mKeyboard != Configuration_KEYBOARD_NOKEYS;
}

MenuBuilderBase::~MenuBuilderBase()
{
    if (mMenuTypes != NULL) {
        for (Int32 i = 0; i < mMenuTypes->GetLength(); ++i) {
            (*mMenuTypes)[i] = NULL;
        }
        ArrayOf< AutoPtr<MenuType> >::Free(mMenuTypes);
    }
}

ECode MenuBuilderBase::SetCallback(
    /* [in] */ IMenuBuilderCallback* callback)
{
    mCallback = callback;
    return NOERROR;
}

AutoPtr<MenuBuilderBase::MenuType> MenuBuilderBase::GetMenuType(
    /* [in] */ Int32 menuType)
{
    if ((*mMenuTypes)[menuType] == NULL) {
        (*mMenuTypes)[menuType] = new MenuType(menuType, this);
    }

    return (*mMenuTypes)[menuType];
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
AutoPtr<IView> MenuBuilderBase::GetMenuView(
    /* [in] */ Int32 menuType,
    /* [in] */ IViewGroup* parent)
{
    // The expanded menu depends on the number if items shown in the icon menu (which
    // is adjustable as setters/XML attributes on IconMenuView [imagine a larger LCD
    // wanting to show more icons]). If, for example, the activity goes through
    // an orientation change while the expanded menu is open, the icon menu's view
    // won't have an instance anymore; so here we make sure we have an icon menu view (matching
    // the same parent so the layout parameters from the XML are used). This
    // will create the icon menu view and cache it (if it doesn't already exist).
    if (menuType == TYPE_EXPANDED && ((*mMenuTypes)[TYPE_ICON] == NULL ||
            !(*mMenuTypes)[TYPE_ICON]->HasMenuView())) {
        GetMenuType(TYPE_ICON)->GetMenuView(parent);
    }

    return IView::Probe(GetMenuType(menuType)->GetMenuView(parent));
}

Int32 MenuBuilderBase::GetNumIconMenuItemsShown()
{
    AutoPtr<IViewGroup> parent;

    if (!(*mMenuTypes)[TYPE_ICON]->HasMenuView()) {
        /*
         * There isn't an icon menu view instantiated, so when we get it
         * below, it will lazily instantiate it. We should pass a proper
         * parent so it uses the layout_ attributes present in the XML
         * layout file.
         */
        if ((*mMenuTypes)[TYPE_EXPANDED]->HasMenuView()) {
            AutoPtr<IView> expandedMenuView = IView::Probe((*mMenuTypes)[TYPE_EXPANDED]->GetMenuView(NULL));
            AutoPtr<IViewParent> p;
            expandedMenuView->GetParent((IViewParent**)&p);
            parent = IViewGroup::Probe(p);
        }
    }

    AutoPtr<IIconMenuView> view = IIconMenuView::Probe(GetMenuView(TYPE_ICON, parent));
    return ((CIconMenuView*)view.Get())->GetNumActualItemsShown();
}

/**
 * Clears the cached menu views. Call this if the menu views need to another
 * layout (for example, if the screen size has changed).
 */
ECode MenuBuilderBase::ClearMenuViews()
{
    for (Int32 i = NUM_TYPES - 1; i >= 0; i--) {
        if ((*mMenuTypes)[i] != NULL) {
            (*mMenuTypes)[i]->mMenuView = NULL;
        }
    }

    List<AutoPtr<IMenuItemImpl> >::ReverseIterator riter;
    for (riter = mItems.RBegin(); riter != mItems.REnd(); ++riter) {
        IMenuItemImpl* item = *riter;
        Boolean hasSub;
        item->HasSubMenu(&hasSub);
        if (hasSub) {
            AutoPtr<ISubMenu> subMenu;
            item->GetSubMenu((ISubMenu**)&subMenu);
            ISubMenuBuilder::Probe(subMenu)->ClearMenuViews();
        }
        ((MenuItemImpl*)item)->ClearItemViews();
    }

    return NOERROR;
}

/**
 * Adds an item to the menu.  The other add methods funnel to this.
 */
AutoPtr<IMenuItem> MenuBuilderBase::AddInternal(
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ ICharSequence* title)
{
    Int32 ordering = GetOrdering(categoryOrder);

    AutoPtr<MenuItemImpl> item = new MenuItemImpl(
            (IMenuBuilder*)this->Probe(EIID_IMenuBuilder), group,
            id, categoryOrder, ordering, title);

    if (mCurrentMenuInfo != NULL) {
        // Pass along the current menu info
        item->SetMenuInfo(mCurrentMenuInfo);
    }

    mItems.Insert(FindInsertIndex(mItems, ordering), item);
    OnItemsChanged(FALSE);

    return (IMenuItem*)item->Probe(EIID_IMenuItem);
}

AutoPtr<IMenuItem> MenuBuilderBase::Add(
    /* [in] */ ICharSequence* title)
{
    return AddInternal(0, 0, 0, title);
}

AutoPtr<IMenuItem> MenuBuilderBase::Add(
    /* [in] */ Int32 titleRes)
{
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);
    return AddInternal(0, 0, 0, csq);
}

AutoPtr<IMenuItem> MenuBuilderBase::Add(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence* title)
{
    return AddInternal(groupId, itemId, order, title);
}

AutoPtr<IMenuItem> MenuBuilderBase::Add(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes)
{
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddInternal(groupId, itemId, order, csq);
}

AutoPtr<ISubMenu> MenuBuilderBase::AddSubMenu(
    /* [in] */ ICharSequence* title)
{
    return AddSubMenu(0, 0, 0, title);
}

AutoPtr<ISubMenu> MenuBuilderBase::AddSubMenu(
    /* [in] */ Int32 titleRes)
{
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddSubMenu(0, 0, 0, csq);
}

AutoPtr<ISubMenu> MenuBuilderBase::AddSubMenu(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence*  title)
{
    AutoPtr<IMenuItemImpl> item = IMenuItemImpl::Probe(AddInternal(groupId, itemId, order, title));
    AutoPtr<ISubMenuBuilder> subMenu = new SubMenuBuilder(mContext,
            (IMenuBuilder*)this->Probe(EIID_IMenuBuilder), item);
    ((MenuItemImpl*)item.Get())->SetSubMenu(subMenu);

    return ISubMenu::Probe(subMenu);
}

AutoPtr<ISubMenu> MenuBuilderBase::AddSubMenu(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes)
{
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddSubMenu(groupId, itemId, order, csq);
}

Int32 MenuBuilderBase::AddIntentOptions(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ IComponentName* caller,
    /* [in] */ const ArrayOf<IIntent*>& specifics,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems)
{
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
    return -1;
}

ECode MenuBuilderBase::RemoveItem(
    /* [in] */ Int32 id)
{
    RemoveItemAtInt(FindItemIndex(id), TRUE);
    return NOERROR;
}

ECode MenuBuilderBase::RemoveGroup(
    /* [in] */ Int32 group)
{
    const Int32 i = FindGroupIndex(group);

    if (i >= 0) {
        const Int32 maxRemovable = mItems.GetSize() - i;
        Int32 numRemoved = 0, groupid;
        while ((numRemoved++ < maxRemovable) && (mItems[i]->GetGroupId(&groupid), groupid == group)) {
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
void MenuBuilderBase::RemoveItemAtInt(
    /* [in] */ Int32 index,
    /* [in] */ Boolean updateChildrenOnMenuViews)
{
    if ((index < 0) || (index >= (Int32)mItems.GetSize())) return;

    mItems.Remove(index);

    if (updateChildrenOnMenuViews) OnItemsChanged(FALSE);
}

ECode MenuBuilderBase::RemoveItemAt(
    /* [in] */ Int32 index)
{
    RemoveItemAtInt(index, TRUE);
    return NOERROR;
}

ECode MenuBuilderBase::ClearAll()
{
    mPreventDispatchingItemsChanged = TRUE;
    Clear();
    ClearHeader();
    mPreventDispatchingItemsChanged = FALSE;
    OnItemsChanged(TRUE);

    return NOERROR;
}

ECode MenuBuilderBase::Clear()
{
    mItems.Clear();

    OnItemsChanged(TRUE);

    return NOERROR;
}

void MenuBuilderBase::SetExclusiveItemChecked(
    /* [in] */ IMenuItem* item)
{
    Int32 group;
    item->GetGroupId(&group);

    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(); iter != mItems.End(); ++iter) {
        IMenuItemImpl* curItem = *iter;
        Int32 groupid;
        curItem->GetGroupId(&groupid);
        if (groupid == group) {
            Boolean isCheckable;
            curItem->IsExclusiveCheckable(&isCheckable);
            if (!isCheckable) continue;
            curItem->IsCheckable(&isCheckable);
            if (!isCheckable) continue;

            // Check the item meant to be checked, uncheck the others (that are in the group)
            ((MenuItemImpl*)curItem)->SetCheckedInt(curItem == item);
        }
    }
}

ECode MenuBuilderBase::SetGroupCheckable(
    /* [in] */ Int32 group,
    /* [in] */ Boolean checkable,
    /* [in] */ Boolean exclusive)
{
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(); iter != mItems.End(); ++iter) {
        IMenuItemImpl* item = *iter;
        Int32 groupid;
        item->GetGroupId(&groupid);
        if (groupid == group) {
            item->SetExclusiveCheckable(exclusive);
            item->SetCheckable(checkable);
        }
    }

    return NOERROR;
}

ECode MenuBuilderBase::SetGroupVisible(
    /* [in] */ Int32 group,
    /* [in] */ Boolean visible)
{
    // We handle the notification of items being changed ourselves, so we use setVisibleInt rather
    // than setVisible and at the end notify of items being changed

    Boolean changedAtLeastOneItem = FALSE;

    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(); iter != mItems.End(); ++iter) {
        IMenuItemImpl* item = *iter;
        Int32 groupid;
        item->GetGroupId(&groupid);
        if (groupid == group) {
            if (((MenuItemImpl*)item)->SetVisibleInt(visible)) changedAtLeastOneItem = TRUE;
        }
    }

    if (changedAtLeastOneItem) OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuBuilderBase::SetGroupEnabled(
    /* [in] */ Int32 group,
    /* [in] */ Boolean enabled)
{
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(); iter != mItems.End(); ++iter) {
        IMenuItemImpl* item = *iter;
        Int32 groupid;
        item->GetGroupId(&groupid);
        if (groupid == group) {
            item->SetEnabled(enabled);
        }
    }

    return NOERROR;
}

Boolean MenuBuilderBase::HasVisibleItems()
{
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(); iter != mItems.End(); ++iter) {
        IMenuItemImpl* item = *iter;
        Boolean visible;
        item->IsVisible(&visible);
        if (visible) {
            return TRUE;
        }
    }

    return FALSE;
}

AutoPtr<IMenuItem> MenuBuilderBase::FindItem(
    /* [in] */ Int32 id)
{
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(); iter != mItems.End(); ++iter) {
        IMenuItemImpl* item = *iter;
        Int32 itemId;
        item->GetItemId(&itemId);
        if (itemId == id) {
            return (IMenuItem*)item;
        }
        else {
            Boolean hasSub;
            item->HasSubMenu(&hasSub);
            if (hasSub) {
                AutoPtr<ISubMenu> subMenu;
                item->GetSubMenu((ISubMenu**)&subMenu);
                AutoPtr<IMenuItem> possibleItem;
                subMenu->FindItem(id, (IMenuItem**)&possibleItem);

                if (possibleItem != NULL) {
                    return possibleItem;
                }
            }
        }
    }

    return NULL;
}

Int32 MenuBuilderBase::FindItemIndex(
    /* [in] */ Int32 id)
{
    Int32 i;
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(), i = 0; iter != mItems.End(); ++iter, ++i) {
        IMenuItemImpl* item = *iter;
        Int32 groupid;
        item->GetGroupId(&groupid);
        if (groupid == id) {
            return i;
        }
    }

    return -1;
}

Int32 MenuBuilderBase::FindGroupIndex(
    /* [in] */ Int32 group)
{
    return FindGroupIndex(group, 0);
}

Int32 MenuBuilderBase::FindGroupIndex(
    /* [in] */ Int32 group,
    /* [in] */ Int32 start)
{
    if (start < 0) {
        start = 0;
    }

    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    Int32 i;
    for(iter = mItems.Begin(), i = 0; iter != mItems.End(); ++iter, ++i) {
        if (i >= start) {
            IMenuItemImpl* item = *iter;

            Int32 groupid;
            item->GetGroupId(&groupid);
            if (groupid == group) {
                return i;
            }
        }
    }

    return -1;
}

Int32 MenuBuilderBase::GetSize()
{
    return mItems.GetSize();
}

/** {@inheritDoc} */
AutoPtr<IMenuItem> MenuBuilderBase::GetItem(
    /* [in] */ Int32 index)
{
    return mItems[index];
}

Boolean MenuBuilderBase::IsShortcutKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FindItemWithShortcutForKey(keyCode, event) != NULL;
}

ECode MenuBuilderBase::SetQwertyMode(
    /* [in] */ Boolean isQwerty)
{
    mQwertyMode = isQwerty;

    RefreshShortcuts(IsShortcutsVisible(), isQwerty);

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
Int32 MenuBuilderBase::GetOrdering(
    /* [in] */ Int32 categoryOrder)
{
    const Int32 index = (categoryOrder & MENU_CATEGORY_MASK) >> MENU_CATEGORY_SHIFT;

    //if (index < 0 || index >= sCategoryToOrder.length) {
    if (index < 0 || index >= 6) {
        //TODO throw new IllegalArgumentException("order does not contain a valid category.");
        assert(0);
    }

    return (sCategoryToOrder[index] << MENU_CATEGORY_SHIFT) | (categoryOrder & MENU_USER_MASK);
}

/**
 * @return whether the menu shortcuts are in qwerty mode or not
 */
Boolean MenuBuilderBase::IsQwertyMode()
{
    return mQwertyMode;
}

/**
 * Refreshes the shortcut labels on each of the displayed items.  Passes the arguments
 * so submenus don't need to call their parent menu for the same values.
 */
void MenuBuilderBase::RefreshShortcuts(
    /* [in] */ Boolean shortcutsVisible,
    /* [in] */ Boolean qwertyMode)
{
    List<AutoPtr<IMenuItemImpl> >::ReverseIterator riter;
    for (riter = mItems.RBegin(); riter != mItems.REnd(); ++riter) {
        IMenuItemImpl* item = *riter;

        Boolean hasSub;
        item->HasSubMenu(&hasSub);
        if (hasSub) {
            AutoPtr<ISubMenu> subMenu;
            item->GetSubMenu((ISubMenu**)&subMenu);
            ((MenuBuilderBase*)subMenu->Probe(EIID_MenuBuilderBase))->RefreshShortcuts(shortcutsVisible, qwertyMode);
        }

        ((MenuItemImpl*)item)->RefreshShortcutOnItemViews(shortcutsVisible, qwertyMode);
    }
}

/**
 * Sets whether the shortcuts should be visible on menus.  Devices without hardware
 * key input will never make shortcuts visible even if this method is passed 'TRUE'.
 *
 * @param shortcutsVisible Whether shortcuts should be visible (if TRUE and a
 *            menu item does not have a shortcut defined, that item will
 *            still NOT show a shortcut)
 */
ECode MenuBuilderBase::SetShortcutsVisible(
    /* [in] */ Boolean shortcutsVisible)
{
    if (mShortcutsVisible == shortcutsVisible) return NOERROR;

//    mShortcutsVisible =
//        (mResources->GetConfiguration().keyboard != Configuration.KEYBOARD_NOKEYS)
//        && shortcutsVisible;

    RefreshShortcuts(mShortcutsVisible, IsQwertyMode());
    return NOERROR;
}

/**
 * @return Whether shortcuts should be visible on menus.
 */
Boolean MenuBuilderBase::IsShortcutsVisible()
{
    return mShortcutsVisible;
}

AutoPtr<IResources> MenuBuilderBase::GetResources()
{
    return mResources;
}

AutoPtr<IMenuBuilderCallback> MenuBuilderBase::GetCallback()
{
    return mCallback;
}

AutoPtr<IContext> MenuBuilderBase::GetContext()
{
    return mContext;
}

Int32 MenuBuilderBase::FindInsertIndex(
    /* [in] */ List<AutoPtr<IMenuItemImpl> >& items,
    /* [in] */ Int32 ordering)
{
    Int32 i = items.GetSize() - 1;
    List<AutoPtr<IMenuItemImpl> >::ReverseIterator riter;
    for (riter = items.RBegin(); riter != items.REnd(); ++riter, --i) {
        IMenuItemImpl* item = *riter;
        Int32 _ordering;
        item->GetOrdering(&_ordering);
        if (_ordering <= ordering) {
            return i + 1;
        }
    }

    return 0;
}

Boolean MenuBuilderBase::PerformShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags)
{
    AutoPtr<IMenuItemImpl> item = FindItemWithShortcutForKey(keyCode, event);

    Boolean handled = FALSE;

    if (item != NULL) {
        handled = PerformItemAction(item, flags);
    }

    if ((flags & MENU_FLAG_ALWAYS_PERFORM_CLOSE) != 0) {
        Close(TRUE);
    }

    return handled;
}

/*
 * This function will return all the menu and sub-menu items that can
 * be directly (the shortcut directly corresponds) and indirectly
 * (the ALT-enabled char corresponds to the shortcut) associated
 * with the keyCode.
 */
List< AutoPtr<IMenuItemImpl> >* MenuBuilderBase::FindItemsWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    const Boolean qwerty = IsQwertyMode();
    Int32 metaState;
    event->GetMetaState(&metaState);
    ElKeyCharacterMap::KeyData* possibleChars = new ElKeyCharacterMap::KeyData();
    // Get the chars associated with the keyCode (i.e using any chording combo)
    Boolean isKeyCodeMapped;
    event->GetKeyData((Handle32)possibleChars, &isKeyCodeMapped);
    // The delete key is not mapped to '\b' so we treat it specially
    if (!isKeyCodeMapped && (keyCode != KeyEvent_KEYCODE_DEL)) {
        return NULL;
    }

    List< AutoPtr<IMenuItemImpl> >* items = new List< AutoPtr<IMenuItemImpl> >;
    // Look for an item whose shortcut is this key.
    List< AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(); iter != mItems.End(); ++iter) {
        IMenuItemImpl* item = *iter;
        Boolean hasSub;
        item->HasSubMenu(&hasSub);
        if (hasSub) {
            AutoPtr<ISubMenu> subMenu;
            item->GetSubMenu((ISubMenu**)&subMenu);
            List< AutoPtr<IMenuItemImpl> >* subMenuItems =
                    ((MenuBuilderBase*)subMenu->Probe(EIID_MenuBuilderBase))->FindItemsWithShortcutForKey(keyCode, event);

            if (subMenuItems != NULL) {
                List< AutoPtr<IMenuItemImpl> >::Iterator subIter;
                for (subIter = subMenuItems->Begin(); subIter != subMenuItems->End(); ++subIter) {
                    items->PushBack(*subIter);
                }
                delete subMenuItems;
            }
        }

        Char32 shortcutChar;
        if (qwerty) {
            item->GetAlphabeticShortcut(&shortcutChar);
        }
        else {
            item->GetNumericShortcut(&shortcutChar);
        }
        Boolean isEnabled;
        if (((metaState & (KeyEvent_META_SHIFT_ON | KeyEvent_META_SYM_ON)) == 0) &&
              (shortcutChar != 0) &&
              (shortcutChar == possibleChars->mMeta[0]
                  || shortcutChar == possibleChars->mMeta[2]
                  || (qwerty && shortcutChar == '\b' &&
                      keyCode == KeyEvent_KEYCODE_DEL)) && (item->IsEnabled(&isEnabled), isEnabled)) {
            items->PushBack(item);
        }
    }

    delete possibleChars;

    return items;
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
AutoPtr<IMenuItemImpl> MenuBuilderBase::FindItemWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // Get all items that can be associated directly or indirectly with the keyCode
    List< AutoPtr<IMenuItemImpl> >* items = FindItemsWithShortcutForKey(keyCode, event);

    if (items == NULL) {
        return NULL;
    }

    Int32 metaState;
    event->GetMetaState(&metaState);
    ElKeyCharacterMap::KeyData* possibleChars = new ElKeyCharacterMap::KeyData();
    // Get the chars associated with the keyCode (i.e using any chording combo)
    Boolean isKeyCodeMapped;
    event->GetKeyData((Handle32)possibleChars, &isKeyCodeMapped);

    // If we have only one element, we can safely returns it
    if (items->GetSize() == 1) {
        delete possibleChars;
        AutoPtr<IMenuItemImpl> item = (*items)[0];
        delete items;
        return item;
    }

    const Boolean qwerty = IsQwertyMode();
    // If we found more than one item associated with the key,
    // we have to return the exact match
    List< AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = items->Begin(); iter != items->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Char32 shortcutChar;
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
            delete possibleChars;
            delete items;
            return item;
        }
    }
    delete possibleChars;
    delete items;
    return NULL;
}

Boolean MenuBuilderBase::PerformIdentifierAction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags)
{
    // Look for an item whose identifier is the id.
    return PerformItemAction(FindItem(id), flags);
}

Boolean MenuBuilderBase::PerformItemAction(
    /* [in] */ IMenuItem* item,
    /* [in] */ Int32 flags)
{
    IMenuItemImpl* itemImpl = IMenuItemImpl::Probe(item);

    Boolean enabled;
    if (itemImpl == NULL || (itemImpl->IsEnabled(&enabled), !enabled)) {
        return FALSE;
    }

    Boolean invoked;
    itemImpl->Invoke(&invoked);

    Boolean has;
    if (item->HasSubMenu(&has), has) {
        Close(FALSE);

        if (mCallback != NULL) {
            // Return TRUE if the sub menu was invoked or the item was invoked previously
            AutoPtr<ISubMenu> submenu;
            item->GetSubMenu((ISubMenu**)&submenu);
            Boolean selected;
            mCallback->OnSubMenuSelected(ISubMenuBuilder::Probe(submenu), &selected);
            invoked = selected || invoked;
        }
    }
    else {
        if ((flags & MENU_FLAG_PERFORM_NO_CLOSE) == 0) {
            Close(TRUE);
        }
    }

    return invoked;
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
ECode MenuBuilderBase::Close(
    /* [in] */ Boolean allMenusAreClosing)
{
    AutoPtr<IMenuBuilderCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->OnCloseMenu((IMenuBuilder*)this->Probe(EIID_IMenuBuilder), allMenusAreClosing);
    }
    return NOERROR;
}

/** {@inheritDoc} */
ECode MenuBuilderBase::Close()
{
    Close(TRUE);
    return NOERROR;
}

/**
 * Called when an item is added or removed.
 *
 * @param cleared Whether the items were cleared or just changed.
 */
void MenuBuilderBase::OnItemsChanged(
    /* [in] */ Boolean cleared)
{
    if (!mPreventDispatchingItemsChanged) {
        if (mIsVisibleItemsStale == FALSE) mIsVisibleItemsStale = TRUE;

        ArrayOf< AutoPtr<MenuType> >* menuTypes = mMenuTypes;
        for (Int32 i = 0; i < NUM_TYPES; i++) {
            if ((*menuTypes)[i] != NULL && ((*menuTypes)[i]->HasMenuView())) {
                IMenuView* menuView = (*menuTypes)[i]->mMenuView;
                menuView->UpdateChildren(cleared);
            }
        }
    }
}

/**
 * Called by {@link CMenuItemImpl*} when its visible flag is changed.
 * @param item The item that has gone through a visibility change.
 */
void MenuBuilderBase::OnItemVisibleChanged(
    /* [in] */ IMenuItemImpl* item)
{
    // Notify of items being changed
    OnItemsChanged(FALSE);
}

List< AutoPtr<IMenuItemImpl> >* MenuBuilderBase::GetVisibleItems()
{
    if (!mIsVisibleItemsStale) return &mVisibleItems;

    // Refresh the visible items
    mVisibleItems.Clear();

    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems.Begin(); iter != mItems.End(); ++iter) {
        IMenuItemImpl* item = *iter;
        Boolean visible;
        item->IsVisible(&visible);
        if (visible) {
            mVisibleItems.PushBack(item);
        }
    }

    mIsVisibleItemsStale = FALSE;

    return &mVisibleItems;
}

ECode MenuBuilderBase::ClearHeader()
{
    mHeaderIcon = NULL;
    mHeaderTitle = NULL;
    mHeaderView = NULL;

    OnItemsChanged(FALSE);
    return NOERROR;
}

void MenuBuilderBase::SetHeaderInternal(
        /* [in] */ Int32 titleRes,
        /* [in] */ ICharSequence* title,
        /* [in] */ Int32 iconRes,
        /* [in] */ IDrawable* icon,
        /* [in] */ IView* view)
{
    AutoPtr<IResources> r = GetResources();

    if (view != NULL) {
        mHeaderView = view;

        // If using a custom view, then the title and icon aren't used
        mHeaderTitle = NULL;
        mHeaderIcon = NULL;
    }
    else {
        if (titleRes > 0) {
            mHeaderTitle = NULL;
            r->GetText(titleRes, (ICharSequence**)&mHeaderTitle);
        }
        else if (title != NULL) {
            mHeaderTitle = title;
        }

        if (iconRes > 0) {
            mHeaderIcon = NULL;
            r->GetDrawable(iconRes, (IDrawable**)&mHeaderIcon);
        }
        else if (icon != NULL) {
            mHeaderIcon = icon;
        }

        // If using the title or icon, then a custom view isn't used
        mHeaderView = NULL;
    }

    // Notify of change
    OnItemsChanged(FALSE);
}

/**
 * Sets the header's title. This replaces the header view. Called by the
 * builder-style methods of subclasses.
 *
 * @param title The new title.
 * @return This MenuBuilderBase so additional setters can be called.
 */
void MenuBuilderBase::SetHeaderTitleInt(
    /* [in] */ ICharSequence* title)
{
    SetHeaderInternal(0, title, 0, NULL, NULL);
}

/**
 * Sets the header's title. This replaces the header view. Called by the
 * builder-style methods of subclasses.
 *
 * @param titleRes The new title (as a resource ID).
 * @return This MenuBuilderBase so additional setters can be called.
 */
void MenuBuilderBase::SetHeaderTitleInt(
    /* [in] */ Int32 titleRes)
{
    SetHeaderInternal(titleRes, NULL, 0, NULL, NULL);
}

/**
 * Sets the header's icon. This replaces the header view. Called by the
 * builder-style methods of subclasses.
 *
 * @param icon The new icon.
 * @return This MenuBuilderBase so additional setters can be called.
 */
void MenuBuilderBase::SetHeaderIconInt(
    /* [in] */ IDrawable* icon)
{
    SetHeaderInternal(0, NULL, 0, icon, NULL);
}

/**
 * Sets the header's icon. This replaces the header view. Called by the
 * builder-style methods of subclasses.
 *
 * @param iconRes The new icon (as a resource ID).
 * @return This MenuBuilderBase so additional setters can be called.
 */
void MenuBuilderBase::SetHeaderIconInt(
    /* [in] */ Int32 iconRes)
{
    SetHeaderInternal(0, NULL, iconRes, NULL, NULL);
}

/**
 * Sets the header's view. This replaces the title and icon. Called by the
 * builder-style methods of subclasses.
 *
 * @param view The new view.
 * @return This MenuBuilderBase so additional setters can be called.
 */
void MenuBuilderBase::SetHeaderViewInt(
    /* [in] */ IView* view)
{
    SetHeaderInternal(0, NULL, 0, NULL, view);
}

AutoPtr<ICharSequence> MenuBuilderBase::GetHeaderTitle()
{
    return mHeaderTitle;
}

AutoPtr<IDrawable> MenuBuilderBase::GetHeaderIcon()
{
    return mHeaderIcon;
}

AutoPtr<IView> MenuBuilderBase::GetHeaderView()
{
    return mHeaderView;
}

/**
 * Gets the root menu (if this is a submenu, find its root menu).
 * @return The root menu.
 */
AutoPtr<IMenuBuilder> MenuBuilderBase::GetRootMenu()
{
    return (IMenuBuilder*)this->Probe(EIID_IMenuBuilder);
}

/**
 * Sets the current menu info that is set on all items added to this menu
 * (until this is called again with different menu info, in which case that
 * one will be added to all subsequent item additions).
 *
 * @param menuInfo The extra menu information to add.
 */
ECode MenuBuilderBase::SetCurrentMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo)
{
    mCurrentMenuInfo = menuInfo;

    return NOERROR;
}

/**
 * Gets an adapter for providing items and their views.
 *
 * @param menuType The type of menu to get an adapter for.
 * @return A {@link MenuAdapter} for this menu with the given menu type.
 */
AutoPtr<IMenuAdapter> MenuBuilderBase::GetMenuAdapter(
    /* [in] */ Int32 menuType)
{
    AutoPtr<IMenuAdapter> adapter = new MenuAdapter(menuType, this);
    return adapter;
}

void MenuBuilderBase::SetOptionalIconsVisible(Boolean visible)
{
    mOptionalIconsVisible = visible;
}

Boolean MenuBuilderBase::GetOptionalIconsVisible()
{
    return mOptionalIconsVisible;
}

ECode MenuBuilderBase::SaveHierarchyState(
    /* [in] */ IBundle* outState)
{
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

ECode MenuBuilderBase::RestoreHierarchyState(
    /* [in] */ IBundle* inState)
{
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
