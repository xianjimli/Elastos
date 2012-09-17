
#ifndef __SUBMENUBUILDER_H__
#define __SUBMENUBUILDER_H__

#include "_CSubMenuBuilder.h"
#include "CMenuBuilder.h"
#include <elastos/AutoPtr.h>

class CMenuItemImpl;

/**
 * The model for a sub menu, which is an extension of the menu.  Most methods are proxied to
 * the parent menu.
 */
CarClass(CSubMenuBuilder), public ElRefBase {
public:
    CSubMenuBuilder(){}

    CSubMenuBuilder(
        /* [in] */ IContext* context,
        /* [in] */ IMenu* parentMenu,
        /* [in] */ IMenuItem* item);

    constructor(
        /* [in] */ IContext* context,
        /* [in] */ IMenu* parentMenu,
        /* [in] */ IMenuItem* item);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI SetQwertyMode(
        /* [in] */ Boolean isQwerty);

    virtual CARAPI IsQwertyMode(
        /* [out] */ Boolean* qwertyMode);

    virtual CARAPI GetParentMenu(
        /* [out] */ IMenu** menu);

    virtual CARAPI GetItemEx(
        /* [out] */ IMenuItem** menuItem);

    virtual CARAPI SetCallback(
        /* [in] */ ICallback* callback);

    virtual CARAPI GetRootMenu(
        /* [out] */ IMenu** menu);

    virtual CARAPI SetIconEx(
        /* [in] */ IDrawable* icon,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetIcon(
        /* [in] */ Int32 iconRes,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderIconEx(
        /* [in] */ IDrawable* icon,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderIcon(
        /* [in] */ Int32 iconRes,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderTitleEx(
        /* [in] */ ICharSequence* title,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderTitle(
        /* [in] */ Int32 titleRes,
        /* [out] */ ISubMenu** submenu);

    virtual CARAPI SetHeaderView(
        /* [in] */ IView* view,
        /* [out] */ ISubMenu** submenu);


   virtual CARAPI Add(
        /* [in] */ const String& title,
        /* [out] */ IMenuItem** item);

    virtual CARAPI AddEx(
        /* [in] */ Int32 titleRes,
        /* [out] */ IMenuItem** item);

    virtual CARAPI AddEx2(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ const String& title,
        /* [out] */ IMenuItem** item);

    virtual CARAPI AddEx3(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ Int32 titleRes,
        /* [out] */ IMenuItem** item);

    virtual CARAPI AddSubMenu(
        /* [in] */ const String& title,
        /* [out] */ ISubMenu** subMenu);

    virtual CARAPI AddSubMenuEx(
        /* [in] */ Int32 titleRes,
        /* [out] */ ISubMenu** subMenu);

    virtual CARAPI AddSubMenuEx2(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ const String& title,
        /* [out] */ ISubMenu** subMenu);

    virtual CARAPI AddSubMenuEx3(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ Int32 titleRes,
        /* [out] */ ISubMenu** subMenu);

    virtual CARAPI AddIntentOptions(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ IComponentName* caller,
        /* [in] */ const ArrayOf<IIntent*>& specifics,
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems,
        /* [out] */ Int32* count);

   virtual CARAPI RemoveItem(
        /* [in] */ Int32 id);

    virtual CARAPI RemoveGroup(
        /* [in] */ Int32 groupId);

    virtual CARAPI Clear();

    virtual CARAPI SetGroupCheckable(
        /* [in] */ Int32 group,
        /* [in] */ Boolean checkable,
        /* [in] */ Boolean exclusive);

    virtual CARAPI SetGroupVisible(
        /* [in] */ Int32 group,
        /* [in] */ Boolean visible);

    virtual CARAPI SetGroupEnabled(
        /* [in] */ Int32 group,
        /* [in] */ Boolean enabled);

    virtual CARAPI HasVisibleItems(
        /* [out] */ Boolean* result);

    virtual CARAPI FindItem(
        /* [in] */ Int32 id,
        /* [out] */ IMenuItem** item);

    virtual CARAPI FindItemIndex(
        /* [in] */ Int32 id,
        /* [out] */ Int32* index);

    virtual CARAPI FindGroupIndex(
        /* [in] */ Int32 group,
        /* [out] */ Int32* index);

    virtual CARAPI FindGroupIndexEx(
        /* [in] */ Int32 group,
        /* [in] */ Int32 start,
        /* [out] */ Int32* index);

    virtual CARAPI Size(
        /* [out] */ Int32* size);

    virtual CARAPI GetItem(
        /* [in] */ Int32 index,
        /* [out] */ IMenuItem** item);

    virtual CARAPI IsShortcutKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* isShortcutKey);

    virtual CARAPI PerformIdentifierAction(
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* isShown);

    virtual CARAPI SetShortcutsVisible(
        /* [in] */ Boolean shortcutsVisible);

    virtual CARAPI IsShortcutsVisible(
        /* [out] */ Boolean* qwertyMode);

    virtual CARAPI GetResources(
        /* [out] */ IResources** resource);

    virtual CARAPI GetCallback(
        /* [out] */ ICallback** callback);

    virtual CARAPI GetContext(
        /* [out] */ IContext** context);

    virtual CARAPI PerformShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* handled);

    virtual CARAPI FindItemsWithShortcutForKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ IObjectContainer** list);

    virtual CARAPI FindItemWithShortcutForKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ IMenuItem** menuItem);

    virtual CARAPI PerformItemAction(
        /* [in] */ IMenuItem* item,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* state);

    virtual CARAPI Close();

    virtual CARAPI SetExclusiveItemChecked(
    /* [in] */ IMenuItem* item);

    virtual CARAPI OnItemVisibleChanged(
        /* [in] */ IMenuItem* item);

    virtual CARAPI SetCurrentMenuInfo(
        /* [in] */ IContextMenuInfo* menuInfo);

    virtual CARAPI SetOptionalIconsVisible(
        /* [in] */ Boolean visible);

    virtual CARAPI GetOptionalIconsVisible(
        /* [out] */ Boolean* visible);

    virtual CARAPI SaveHierarchyState(
        /* [in] */ IBundle* outState);

    virtual CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* inState);

    virtual CARAPI GetMenuAdapter(
        /* [in] */ Int32 menuType,
        /* [out] */ IMenuAdapter** menuAdapter);

    virtual CARAPI GetMenuType(
        /* [in] */ Int32 menuType,
        /* [out] */ IMenuType** type);

    virtual CARAPI GetMenuView(
        /* [in] */ Int32 menuType,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IMenuView** menuView);

    virtual CARAPI ClearMenuViews();

    virtual CARAPI RefreshShortcuts(
        /* [in] */ Boolean shortcutsVisible,
        /* [in] */ Boolean qwertyMode);

    virtual CARAPI SetHeaderTitleInt(
        /* [in] */ const String& title,
        /* [out] */ IMenu** menu);

    virtual CARAPI SetHeaderTitleIntEx(
        /* [in] */ Int32 titleRes,
        /* [out] */ IMenu** menu);

    virtual CARAPI SetHeaderViewInt(
        /* [in] */ IView* view,
        /* [out] */ IMenu** menu);

    virtual CARAPI GetVisibleItems(
        /* [out] */ IObjectContainer** list);

    virtual CARAPI GetNumIconMenuItemsShown(
        /* [out] */ Int32* state);

    virtual CARAPI SetHeaderInternal(
        /* [in] */ Int32 titleRes,
        /* [in] */ const String& title,
        /* [in] */ Int32 iconRes,
        /* [in] */ IDrawable* icon,
        /* [in] */ IView* view);

    virtual CARAPI SetHeaderIconInt(
        /* [in] */ IDrawable* icon,
        /* [out] */ IMenu** menu);

    virtual CARAPI SetHeaderIconIntEx(
        /* [in] */ Int32 iconRes,
        /* [out] */ IMenu** menu);

private:
    AutoPtr<IMenu> mParentMenu;
    AutoPtr<IMenuItem> mItem;
};

#endif  //__SUBMENUBUILDER_H__
