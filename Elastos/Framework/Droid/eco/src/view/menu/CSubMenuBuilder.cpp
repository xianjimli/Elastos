
#include "CSubMenuBuilder.h"

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
CSubMenuBuilder::CSubMenuBuilder(
        /* [in] */ IContext* context,
        /* [in] */ IMenu* parentMenu,
        /* [in] */ IMenuItem* item) {
    mParentMenu = parentMenu;
    mItem = item;
}

CSubMenuBuilder::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IMenu* parentMenu,
        /* [in] */ IMenuItem* item){
    mParentMenu = parentMenu;
    mItem = item;
}

PInterface CSubMenuBuilder::Probe(
    /* [in]  */ REIID riid) {
    if (riid == EIID_ISubMenu) {
        return (ISubMenu*)this;
    }

    return NULL;
}

UInt32 CSubMenuBuilder::AddRef() {
    return ElRefBase::AddRef();
}

UInt32 CSubMenuBuilder::Release() {
    return ElRefBase::Release();
}

ECode CSubMenuBuilder::GetInterfaceID(
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

ECode CSubMenuBuilder::SetQwertyMode(
        /* [in] */ Boolean isQwerty){
    mParentMenu->SetQwertyMode(isQwerty);
    return NOERROR;
}

ECode CSubMenuBuilder::IsQwertyMode(
    /* [out] */ Boolean* qwertyMode) {
    return mParentMenu->IsQwertyMode(qwertyMode);
}

ECode CSubMenuBuilder::SetShortcutsVisible(
    /* [in] */ Boolean shortcutsVisible) {
    return mParentMenu->SetShortcutsVisible(shortcutsVisible);
}

ECode CSubMenuBuilder::IsShortcutsVisible(
    /* [out] */ Boolean* qwertyMode) {
    return mParentMenu->IsShortcutsVisible(qwertyMode);
}

ECode CSubMenuBuilder::GetParentMenu(
    /* [out] */ IMenu** menu) {
    *menu = mParentMenu;
    return NOERROR;
}

ECode CSubMenuBuilder::GetItemEx(
    /* [out] */ IMenuItem** menuItem) {
    *menuItem = mItem;

    return NOERROR;
}

ECode CSubMenuBuilder::GetCallback(
    /* [out] */ ICallback** callback) {
    return mParentMenu->GetCallback(callback);
}

ECode CSubMenuBuilder::SetCallback(ICallback* callback) {
    mParentMenu->SetCallback(callback);

    return NOERROR;
}

ECode CSubMenuBuilder::GetRootMenu(
    /* [out] */ IMenu** menu) {
    *menu = mParentMenu;
    return NOERROR;
}

ECode CSubMenuBuilder::SetIconEx(
    /* [in] */ IDrawable* icon,
    /* [out] */ ISubMenu** submenu) {
    AutoPtr<IMenuItem> menuItem;
    mItem->SetIcon(icon, (IMenuItem**) &menuItem);

    *submenu = this;
    return NOERROR;
}

ECode CSubMenuBuilder::SetIcon(
    /* [in] */ Int32 iconRes,
    /* [out] */ ISubMenu** submenu) {
    AutoPtr<IMenuItem> menuItem;
    mItem->SetIconEx(iconRes, (IMenuItem**) &menuItem);

    *submenu = this;
    return NOERROR;
}

ECode CSubMenuBuilder::SetHeaderIconEx(
    /* [in] */ IDrawable* icon,
    /* [out] */ ISubMenu** submenu) {
    return SetHeaderIconInt(icon, (IMenu**)submenu);
}

ECode CSubMenuBuilder::SetHeaderIcon(
    /* [in] */ Int32 iconRes,
    /* [out] */ ISubMenu** submenu) {
    return SetHeaderIconIntEx(iconRes, (IMenu**) &submenu);
}

ECode CSubMenuBuilder::SetHeaderTitleEx(
    /* [in] */ ICharSequence* title,
    /* [out] */ ISubMenu** submenu) {
    String str;

    if (title != NULL) {
        title->ToString(&str);
        SetHeaderTitleInt(str, (IMenu**) &submenu);
        *submenu = this;
    }

    return NOERROR;
}

ECode CSubMenuBuilder::SetHeaderTitle(
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** submenu) {
    return SetHeaderTitleIntEx(titleRes, (IMenu**) &submenu);
}

ECode CSubMenuBuilder::SetHeaderView(
    /* [in] */ IView* view,
    /* [out] */ ISubMenu** submenu) {
    return SetHeaderViewInt(view, (IMenu**) &submenu);
}

ECode CSubMenuBuilder::Add(
    /* [in] */ const String& title,
    /* [out] */ IMenuItem** item)
{
    return mParentMenu->Add(title, item);
}

ECode CSubMenuBuilder::AddEx(
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuItem** item)
{
    return mParentMenu->AddEx(titleRes, item);
}

ECode CSubMenuBuilder::AddEx2(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ const String& title,
    /* [out] */ IMenuItem** item)
{
    return mParentMenu->AddEx2(groupId, itemId, order, title, item);
}

ECode CSubMenuBuilder::AddEx3(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuItem** item) {
    return mParentMenu->AddEx3(groupId, itemId, order, titleRes, item);
}

ECode CSubMenuBuilder::AddSubMenu(
    /* [in] */ const String& title,
    /* [out] */ ISubMenu** subMenu) {
    return mParentMenu->AddSubMenu(title, subMenu);
}

ECode CSubMenuBuilder::AddSubMenuEx(
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** subMenu) {
    return mParentMenu->AddSubMenuEx(titleRes, subMenu);
}

ECode CSubMenuBuilder::AddSubMenuEx2(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ const String& title,
    /* [out] */ ISubMenu** subMenu) {
    return mParentMenu->AddSubMenuEx2(groupId, itemId, order, title, subMenu);
}

ECode CSubMenuBuilder::AddSubMenuEx3(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** subMenu) {
    return mParentMenu->AddSubMenuEx3(groupId, itemId, order, titleRes, subMenu);
}

ECode CSubMenuBuilder::AddIntentOptions(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ IComponentName* caller,
    /* [in] */ const ArrayOf<IIntent*>& specifics,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems,
    /* [out] */ Int32* count) {
    return mParentMenu->AddIntentOptions(groupId, itemId, order, caller, specifics, intent, flags, outSpecificItems, count);
}

ECode CSubMenuBuilder::RemoveItem(
    /* [in] */ Int32 id) {
    return mParentMenu->RemoveItem(id);
}

ECode CSubMenuBuilder::RemoveGroup(
    /* [in] */ Int32 groupId) {
    return mParentMenu->RemoveGroup(groupId);
}

ECode CSubMenuBuilder::Clear() {
    return mParentMenu->Clear();
}

ECode CSubMenuBuilder::SetGroupCheckable(
    /* [in] */ Int32 group,
    /* [in] */ Boolean checkable,
    /* [in] */ Boolean exclusive) {
    return mParentMenu->SetGroupCheckable(group, checkable, exclusive);
}

ECode CSubMenuBuilder::SetGroupVisible(
    /* [in] */ Int32 group,
    /* [in] */ Boolean visible) {
    return mParentMenu->SetGroupVisible(group, visible);
}

ECode CSubMenuBuilder::SetGroupEnabled(
    /* [in] */ Int32 group,
    /* [in] */ Boolean enabled) {
    return mParentMenu->SetGroupEnabled(group, enabled);
}

ECode CSubMenuBuilder::HasVisibleItems(
    /* [out] */ Boolean* result) {
    return mParentMenu->HasVisibleItems(result);
}

ECode CSubMenuBuilder::FindItem(
    /* [in] */ Int32 id,
    /* [out] */ IMenuItem** item) {
    return mParentMenu->FindItem(id, item);
}

ECode CSubMenuBuilder::FindItemIndex(
    /* [in] */ Int32 id,
    /* [out] */ Int32* index) {
    return mParentMenu->FindItemIndex(id, index);
}

ECode CSubMenuBuilder::FindGroupIndex(
    /* [in] */ Int32 group,
    /* [out] */ Int32* index) {
    return mParentMenu->FindGroupIndex(group, index);
}

ECode CSubMenuBuilder::FindGroupIndexEx(
    /* [in] */ Int32 group,
    /* [in] */ Int32 start,
    /* [out] */ Int32* index) {
    return mParentMenu->FindGroupIndexEx(group, start, index);
}

ECode CSubMenuBuilder::Size(
    /* [out] */ Int32* size) {
    return mParentMenu->Size(size);
}

ECode CSubMenuBuilder::GetItem(
    /* [in] */ Int32 index,
    /* [out] */ IMenuItem** item) {
    return mParentMenu->GetItem(index, item);
}

ECode CSubMenuBuilder::IsShortcutKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isShortcutKey) {
    return mParentMenu->IsShortcutKey(keyCode, event, isShortcutKey);
}

ECode CSubMenuBuilder::PerformIdentifierAction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* isShown) {
    return mParentMenu->PerformIdentifierAction(id, flags, isShown);
}

ECode CSubMenuBuilder::GetResources(
    /* [out] */ IResources** resource) {
    return mParentMenu->GetResources(resource);
}

ECode CSubMenuBuilder::GetContext(
    /* [out] */ IContext** context) {
    return mParentMenu->GetContext(context);
}

ECode CSubMenuBuilder::PerformShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* handled) {
    return mParentMenu->PerformShortcut(keyCode, event, flags, handled);
}

ECode CSubMenuBuilder::FindItemsWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ IObjectContainer** list) {
    return mParentMenu->FindItemsWithShortcutForKey(keyCode, event, list);
}

ECode CSubMenuBuilder::FindItemWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ IMenuItem** menuItem) {
    return mParentMenu->FindItemWithShortcutForKey(keyCode, event, menuItem);
}

ECode CSubMenuBuilder::PerformItemAction(
    /* [in] */ IMenuItem* item,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* state) {
    return mParentMenu->PerformItemAction(item, flags, state);
}

ECode CSubMenuBuilder::Close() {
    return mParentMenu->Close();
}

ECode CSubMenuBuilder::SetExclusiveItemChecked(
/* [in] */ IMenuItem* item) {
    return mParentMenu->SetExclusiveItemChecked(item);
}

ECode CSubMenuBuilder::OnItemVisibleChanged(
    /* [in] */ IMenuItem* item) {
    return mParentMenu->OnItemVisibleChanged(item);
}

ECode CSubMenuBuilder::SetCurrentMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo) {
    return mParentMenu->SetCurrentMenuInfo(menuInfo);
}

ECode CSubMenuBuilder::SetOptionalIconsVisible(
    /* [in] */ Boolean visible) {
    return mParentMenu->SetOptionalIconsVisible(visible);
}

ECode CSubMenuBuilder::GetOptionalIconsVisible(
    /* [out] */ Boolean* visible) {
    return mParentMenu->GetOptionalIconsVisible(visible);
}

ECode CSubMenuBuilder::SaveHierarchyState(
    /* [in] */ IBundle* outState) {
    return mParentMenu->SaveHierarchyState(outState);
}

ECode CSubMenuBuilder::RestoreHierarchyState(
    /* [in] */ IBundle* inState) {
    return mParentMenu->RestoreHierarchyState(inState);
}

ECode CSubMenuBuilder::GetMenuAdapter(
    /* [in] */ Int32 menuType,
    /* [out] */ IMenuAdapter** menuAdapter) {
    return mParentMenu->GetMenuAdapter(menuType, menuAdapter);
}

ECode CSubMenuBuilder::GetMenuType(
    /* [in] */ Int32 menuType,
    /* [out] */ IMenuType** type) {
    return mParentMenu->GetMenuType(menuType, type);
}

ECode CSubMenuBuilder::GetMenuView(
    /* [in] */ Int32 menuType,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IMenuView** menuView) {
    return mParentMenu->GetMenuView(menuType, parent, menuView);
}

ECode CSubMenuBuilder::ClearMenuViews() {
    return mParentMenu->ClearMenuViews();
}

ECode CSubMenuBuilder::RefreshShortcuts(
    /* [in] */ Boolean shortcutsVisible,
    /* [in] */ Boolean qwertyMode) {
    return mParentMenu->RefreshShortcuts(shortcutsVisible, qwertyMode);
}

ECode CSubMenuBuilder::SetHeaderTitleInt(
    /* [in] */ const String& title,
    /* [out] */ IMenu** menu) {
    return mParentMenu->SetHeaderTitleInt(title, menu);
}

ECode CSubMenuBuilder::SetHeaderTitleIntEx(
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenu** menu) {
    return mParentMenu->SetHeaderTitleIntEx(titleRes, menu);
}

ECode CSubMenuBuilder::SetHeaderViewInt(
    /* [in] */ IView* view,
    /* [out] */ IMenu** menu) {
    return mParentMenu->SetHeaderViewInt(view, menu);
}

ECode CSubMenuBuilder::GetVisibleItems(
    /* [out] */ IObjectContainer** list) {
    return mParentMenu->GetVisibleItems(list);
}

ECode CSubMenuBuilder::GetNumIconMenuItemsShown(
    /* [out] */ Int32* state) {
    return mParentMenu->GetNumIconMenuItemsShown(state);
}

ECode CSubMenuBuilder::SetHeaderInternal(
    /* [in] */ Int32 titleRes,
    /* [in] */ const String& title,
    /* [in] */ Int32 iconRes,
    /* [in] */ IDrawable* icon,
    /* [in] */ IView* view) {
    return mParentMenu->SetHeaderInternal(titleRes, title, iconRes, icon, view);
}

ECode CSubMenuBuilder::SetHeaderIconInt(
    /* [in] */ IDrawable* icon,
    /* [out] */ IMenu** menu) {
    return mParentMenu->SetHeaderIconInt(icon, menu);
}

ECode CSubMenuBuilder::SetHeaderIconIntEx(
    /* [in] */ Int32 iconRes,
    /* [out] */ IMenu** menu) {
    return mParentMenu->SetHeaderIconIntEx(iconRes, menu);
}