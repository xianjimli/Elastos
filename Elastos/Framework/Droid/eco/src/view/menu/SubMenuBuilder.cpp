
#include "SubMenuBuilder.h"

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
SubMenuBuilder::SubMenuBuilder(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItem* item): MenuBuilder(context) {
    mParentMenu = parentMenu;
    mItem = item;
}

ECode SubMenuBuilder::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItem* item){
    mParentMenu = parentMenu;
    mItem = item;

    return NOERROR;
}

PInterface SubMenuBuilder::Probe(
    /* [in]  */ REIID riid) {
    if (riid == EIID_ISubMenu) {
        return (ISubMenu*)this;
    }

    return NULL;
}

UInt32 SubMenuBuilder::AddRef() {
    return ElRefBase::AddRef();
}

UInt32 SubMenuBuilder::Release() {
    return ElRefBase::Release();
}

ECode SubMenuBuilder::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID) {
    if (pObject == (IInterface*)(ISubMenu*)this) {
        *pIID = EIID_ISubMenu;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode SubMenuBuilder::SetQwertyMode(
        /* [in] */ Boolean isQwerty){
    ((MenuBuilder*)mParentMenu.Get())->SetQwertyMode(isQwerty);
    return NOERROR;
}

ECode SubMenuBuilder::IsQwertyMode(
    /* [out] */ Boolean* qwertyMode) {
    return mParentMenu->IsQwertyMode(qwertyMode);
}

ECode SubMenuBuilder::SetShortcutsVisible(
    /* [in] */ Boolean shortcutsVisible) {
    return mParentMenu->SetShortcutsVisible(shortcutsVisible);
}

ECode SubMenuBuilder::IsShortcutsVisible(
    /* [out] */ Boolean* qwertyMode) {
    return mParentMenu->IsShortcutsVisible(qwertyMode);
}

ECode SubMenuBuilder::GetParentMenu(
    /* [out] */ IMenu** menu) {
    *menu = (IMenu*) mParentMenu->Probe(EIID_IMenu);
    return NOERROR;
}

ECode SubMenuBuilder::GetItemEx(
    /* [out] */ IMenuItem** menuItem) {
    *menuItem = mItem;

    return NOERROR;
}

ECode SubMenuBuilder::GetCallback(
    /* [out] */ IMenuBuilderCallback** callback) {
    return mParentMenu->GetCallback(callback);
}

ECode SubMenuBuilder::SetCallback(IMenuBuilderCallback* callback) {
    mParentMenu->SetCallback(callback);

    return NOERROR;
}

ECode SubMenuBuilder::GetRootMenu(
    /* [out] */ IMenuBuilder** menu) {
    *menu = mParentMenu;
    return NOERROR;
}

ECode SubMenuBuilder::SetIconEx(
    /* [in] */ IDrawable* icon,
    /* [out] */ ISubMenu** submenu) {
    AutoPtr<IMenuItem> menuItem;
    mItem->SetIcon(icon, (IMenuItem**) &menuItem);

    *submenu = (ISubMenu*)this->Probe(EIID_ISubMenu);
    return NOERROR;
}

ECode SubMenuBuilder::SetIcon(
    /* [in] */ Int32 iconRes,
    /* [out] */ ISubMenu** submenu) {
    AutoPtr<IMenuItem> menuItem;
    mItem->SetIconEx(iconRes, (IMenuItem**) &menuItem);

    *submenu = (ISubMenu*)this->Probe(EIID_ISubMenu);
    return NOERROR;
}

ECode SubMenuBuilder::SetHeaderIconEx(
    /* [in] */ IDrawable* icon,
    /* [out] */ ISubMenu** submenu) {
    return SetHeaderIconInt(icon, (IMenuBuilder**)submenu);
}

ECode SubMenuBuilder::SetHeaderIcon(
    /* [in] */ Int32 iconRes,
    /* [out] */ ISubMenu** submenu) {
    return SetHeaderIconIntEx(iconRes, (IMenuBuilder**) &submenu);
}

ECode SubMenuBuilder::SetHeaderTitleEx(
    /* [in] */ ICharSequence* title,
    /* [out] */ ISubMenu** submenu) {
    String str;

    if (title != NULL) {
        title->ToString(&str);
        AutoPtr<ICharSequence> csq;
        CStringWrapper::New(str, (ICharSequence**)&csq);

        SetHeaderTitleInt(csq, (IMenuBuilder**) &submenu);
        *submenu = (ISubMenu*)this->Probe(EIID_ISubMenu);
    }

    return NOERROR;
}

ECode SubMenuBuilder::SetHeaderTitle(
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** submenu) {
    return SetHeaderTitleIntEx(titleRes, (IMenuBuilder**) &submenu);
}

ECode SubMenuBuilder::SetHeaderView(
    /* [in] */ IView* view,
    /* [out] */ ISubMenu** submenu) {
    return SetHeaderViewInt(view, (IMenuBuilder**) &submenu);
}

ECode SubMenuBuilder::Add(
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuItem** item)
{
    return ((MenuBuilder*)mParentMenu.Get())->Add(title, item);
}

ECode SubMenuBuilder::AddEx(
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuItem** item)
{
    return ((MenuBuilder*)mParentMenu.Get())->Add(titleRes, item);
}

ECode SubMenuBuilder::AddEx2(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuItem** item)
{
    return ((MenuBuilder*)mParentMenu.Get())->Add(groupId, itemId, order, title, item);
}

ECode SubMenuBuilder::AddEx3(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuItem** item) {
    return ((MenuBuilder*)mParentMenu.Get())->Add(groupId, itemId, order, titleRes, item);
}

ECode SubMenuBuilder::AddSubMenu(
    /* [in] */ ICharSequence* title,
    /* [out] */ ISubMenu** subMenu) {
    return ((MenuBuilder*)mParentMenu.Get())->AddSubMenu(title, subMenu);
}

ECode SubMenuBuilder::AddSubMenuEx(
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** subMenu) {
    return ((MenuBuilder*)mParentMenu.Get())->AddSubMenu(titleRes, subMenu);
}

ECode SubMenuBuilder::AddSubMenuEx2(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence* title,
    /* [out] */ ISubMenu** subMenu) {
    return ((MenuBuilder*)mParentMenu.Get())->AddSubMenu(groupId, itemId, order, title, subMenu);
}

ECode SubMenuBuilder::AddSubMenuEx3(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** subMenu) {
    return ((MenuBuilder*)mParentMenu.Get())->AddSubMenu(groupId, itemId, order, titleRes, subMenu);
}

ECode SubMenuBuilder::AddIntentOptions(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ IComponentName* caller,
    /* [in] */ const ArrayOf<IIntent*>& specifics,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems,
    /* [out] */ Int32* count) {
    return ((MenuBuilder*)mParentMenu.Get())->AddIntentOptions(groupId, itemId, order, caller, specifics, intent, flags, outSpecificItems, count);
}

ECode SubMenuBuilder::RemoveItem(
    /* [in] */ Int32 id) {
    return ((MenuBuilder*)mParentMenu.Get())->RemoveItem(id);
}

ECode SubMenuBuilder::RemoveGroup(
    /* [in] */ Int32 groupId) {
    return ((MenuBuilder*)mParentMenu.Get())->RemoveGroup(groupId);
}

ECode SubMenuBuilder::Clear() {
    return ((MenuBuilder*)mParentMenu.Get())->Clear();
}

ECode SubMenuBuilder::SetGroupCheckable(
    /* [in] */ Int32 group,
    /* [in] */ Boolean checkable,
    /* [in] */ Boolean exclusive) {
    return ((MenuBuilder*)mParentMenu.Get())->SetGroupCheckable(group, checkable, exclusive);
}

ECode SubMenuBuilder::SetGroupVisible(
    /* [in] */ Int32 group,
    /* [in] */ Boolean visible) {
    return ((MenuBuilder*)mParentMenu.Get())->SetGroupVisible(group, visible);
}

ECode SubMenuBuilder::SetGroupEnabled(
    /* [in] */ Int32 group,
    /* [in] */ Boolean enabled) {
    return ((MenuBuilder*)mParentMenu.Get())->SetGroupEnabled(group, enabled);
}

ECode SubMenuBuilder::HasVisibleItems(
    /* [out] */ Boolean* result) {
    return ((MenuBuilder*)mParentMenu.Get())->HasVisibleItems(result);
}

ECode SubMenuBuilder::FindItem(
    /* [in] */ Int32 id,
    /* [out] */ IMenuItem** item) {
    return ((MenuBuilder*)mParentMenu.Get())->FindItem(id, item);
}

ECode SubMenuBuilder::FindItemIndex(
    /* [in] */ Int32 id,
    /* [out] */ Int32* index) {
    return ((MenuBuilder*)mParentMenu.Get())->FindItemIndex(id, index);
}

ECode SubMenuBuilder::FindGroupIndex(
    /* [in] */ Int32 group,
    /* [out] */ Int32* index) {
    return ((MenuBuilder*)mParentMenu.Get())->FindGroupIndex(group, index);
}

ECode SubMenuBuilder::FindGroupIndexEx(
    /* [in] */ Int32 group,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index) {
    return ((MenuBuilder*)mParentMenu.Get())->FindGroupIndex(group, start, index);
}

ECode SubMenuBuilder::Size(
    /* [out] */ Int32* size) {
    return ((MenuBuilder*)mParentMenu.Get())->Size(size);
}

ECode SubMenuBuilder::GetItem(
    /* [in] */ Int32 index,
    /* [out] */ IMenuItem** item) {
    return ((MenuBuilder*)mParentMenu.Get())->GetItem(index, item);
}

ECode SubMenuBuilder::IsShortcutKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isShortcutKey) {
    return ((MenuBuilder*)mParentMenu.Get())->IsShortcutKey(keyCode, event, isShortcutKey);
}

ECode SubMenuBuilder::PerformIdentifierAction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* isShown) {
    return ((MenuBuilder*)mParentMenu.Get())->PerformIdentifierAction(id, flags, isShown);
}

ECode SubMenuBuilder::GetResources(
    /* [out] */ IResources** resource) {
    return mParentMenu->GetResources(resource);
}

ECode SubMenuBuilder::GetContext(
    /* [out] */ IContext** context) {
    return mParentMenu->GetContext(context);
}

ECode SubMenuBuilder::PerformShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* handled) {
    return ((MenuBuilder*)mParentMenu.Get())->PerformShortcut(keyCode, event, flags, handled);
}

ECode SubMenuBuilder::FindItemsWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ IObjectContainer** list) {
    return mParentMenu->FindItemsWithShortcutForKey(keyCode, event, list);
}

ECode SubMenuBuilder::FindItemWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ IMenuItem** menuItem) {
    return mParentMenu->FindItemWithShortcutForKey(keyCode, event, menuItem);
}

ECode SubMenuBuilder::PerformItemAction(
    /* [in] */ IMenuItem* item,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* state) {
    return mParentMenu->PerformItemAction(item, flags, state);
}

ECode SubMenuBuilder::Close() {
    return ((MenuBuilder*)mParentMenu.Get())->Close();
}

ECode SubMenuBuilder::SetExclusiveItemChecked(
/* [in] */ IMenuItem* item) {
    return mParentMenu->SetExclusiveItemChecked(item);
}

ECode SubMenuBuilder::OnItemVisibleChanged(
    /* [in] */ IMenuItem* item) {
    return mParentMenu->OnItemVisibleChanged(item);
}

ECode SubMenuBuilder::SetCurrentMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo) {
    return mParentMenu->SetCurrentMenuInfo(menuInfo);
}

ECode SubMenuBuilder::SetOptionalIconsVisible(
    /* [in] */ Boolean visible) {
    return mParentMenu->SetOptionalIconsVisible(visible);
}

ECode SubMenuBuilder::GetOptionalIconsVisible(
    /* [out] */ Boolean* visible) {
    return mParentMenu->GetOptionalIconsVisible(visible);
}

ECode SubMenuBuilder::SaveHierarchyState(
    /* [in] */ IBundle* outState) {
    return mParentMenu->SaveHierarchyState(outState);
}

ECode SubMenuBuilder::RestoreHierarchyState(
    /* [in] */ IBundle* inState) {
    return mParentMenu->RestoreHierarchyState(inState);
}

ECode SubMenuBuilder::GetMenuAdapter(
    /* [in] */ Int32 menuType,
    /* [out] */ IMenuAdapter** menuAdapter) {
    return mParentMenu->GetMenuAdapter(menuType, menuAdapter);
}

ECode SubMenuBuilder::GetMenuType(
    /* [in] */ Int32 menuType,
    /* [out] */ IMenuBuilderType** type) {
    return mParentMenu->GetMenuType(menuType, type);
}

ECode SubMenuBuilder::GetMenuView(
    /* [in] */ Int32 menuType,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IMenuView** menuView) {
    return mParentMenu->GetMenuView(menuType, parent, menuView);
}

ECode SubMenuBuilder::ClearMenuViews() {
    return mParentMenu->ClearMenuViews();
}

ECode SubMenuBuilder::RefreshShortcuts(
    /* [in] */ Boolean shortcutsVisible,
    /* [in] */ Boolean qwertyMode) {
    return mParentMenu->RefreshShortcuts(shortcutsVisible, qwertyMode);
}

ECode SubMenuBuilder::SetHeaderTitleInt(
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuBuilder** menu) {
    return mParentMenu->SetHeaderTitleInt(title, menu);
}

ECode SubMenuBuilder::SetHeaderTitleIntEx(
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuBuilder** menu) {
    return mParentMenu->SetHeaderTitleIntEx(titleRes, menu);
}

ECode SubMenuBuilder::SetHeaderViewInt(
    /* [in] */ IView* view,
    /* [out] */ IMenuBuilder** menu) {
    return mParentMenu->SetHeaderViewInt(view, menu);
}

ECode SubMenuBuilder::GetVisibleItems(
    /* [out] */ IObjectContainer** list) {
    return mParentMenu->GetVisibleItems(list);
}

ECode SubMenuBuilder::GetNumIconMenuItemsShown(
    /* [out] */ Int32* state) {
    return mParentMenu->GetNumIconMenuItemsShown(state);
}

ECode SubMenuBuilder::SetHeaderInternal(
    /* [in] */ Int32 titleRes,
    /* [in] */ ICharSequence* title,
    /* [in] */ Int32 iconRes,
    /* [in] */ IDrawable* icon,
    /* [in] */ IView* view) {
    return mParentMenu->SetHeaderInternal(titleRes, title, iconRes, icon, view);
}

ECode SubMenuBuilder::SetHeaderIconInt(
    /* [in] */ IDrawable* icon,
    /* [out] */ IMenuBuilder** menu) {
    return mParentMenu->SetHeaderIconInt(icon, menu);
}

ECode SubMenuBuilder::SetHeaderIconIntEx(
    /* [in] */ Int32 iconRes,
    /* [out] */ IMenuBuilder** menu) {
    return mParentMenu->SetHeaderIconIntEx(iconRes, menu);
}