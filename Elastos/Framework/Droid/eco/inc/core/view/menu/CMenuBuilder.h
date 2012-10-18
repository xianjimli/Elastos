
#ifndef __CMENUBUILDER_H__
#define __CMENUBUILDER_H__

#include "ext/frameworkext.h"
#include "_CMenuBuilder.h"
#include "widget/BaseAdapter.h"
#include "MenuBuilder.h"
#include <elastos/List.h>

using namespace Elastos;

CarClass(CMenuBuilder), public MenuBuilder
{
public:
    CMenuBuilder(
        /* [in] */ IContext* context);

    CMenuBuilder();

    virtual PInterface Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /*[in]*/ IContext* context);

    virtual CARAPI SetCallback(
        /* [in] */ IMenuBuilderCallback* callback);

    virtual CARAPI GetMenuType(
        /* [in] */ Int32 menuType,
        /* [out] */ IMenuBuilderType** type);

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
    virtual CARAPI GetMenuView(
        /* [in] */ Int32 menuType,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IMenuView** menuView);

    /**
     * Clears the cached menu views. Call this if the menu views need to another
     * layout (for example, if the screen size has changed).
     */
    virtual CARAPI ClearMenuViews();

    /**
     * Add a new item to the menu. This item displays the given title for its
     * label.
     *
     * @param title The text to display for the item.
     * @return The newly added menu item.
     */
    virtual CARAPI Add(
        /* [in] */ ICharSequence* title,
        /* [out] */ IMenuItem** item);

    /**
     * Add a new item to the menu. This item displays the given title for its
     * label.
     *
     * @param titleRes Resource identifier of title string.
     * @return The newly added menu item.
     */
    virtual CARAPI AddEx(
        /* [in] */ Int32 titleRes,
        /* [out] */ IMenuItem** item);

    /**
     * Add a new item to the menu. This item displays the given title for its
     * label.
     *
     * @param groupId The group identifier that this item should be part of.
     *        This can be used to define groups of items for batch state
     *        changes. Normally use {@link #NONE} if an item should not be in a
     *        group.
     * @param itemId Unique item ID. Use {@link #NONE} if you do not need a
     *        unique ID.
     * @param order The order for the item. Use {@link #NONE} if you do not care
     *        about the order. See {@link MenuItem#getOrder()}.
     * @param title The text to display for the item.
     * @return The newly added menu item.
     */
    virtual CARAPI AddEx2(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ ICharSequence* title,
        /* [out] */ IMenuItem** item);

    /**
     * Variation on {@link #add(int, int, int, CharSequence)} that takes a
     * string resource identifier instead of the string itself.
     *
     * @param groupId The group identifier that this item should be part of.
     *        This can also be used to define groups of items for batch state
     *        changes. Normally use {@link #NONE} if an item should not be in a
     *        group.
     * @param itemId Unique item ID. Use {@link #NONE} if you do not need a
     *        unique ID.
     * @param order The order for the item. Use {@link #NONE} if you do not care
     *        about the order. See {@link MenuItem#getOrder()}.
     * @param titleRes Resource identifier of title string.
     * @return The newly added menu item.
     */
    virtual CARAPI AddEx3(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ Int32 titleRes,
        /* [out] */ IMenuItem** item);

    /**
     * Add a new sub-menu to the menu. This item displays the given title for
     * its label. To modify other attributes on the submenu's menu item, use
     * {@link SubMenu#getItem()}.
     *
     * @param title The text to display for the item.
     * @return The newly added sub-menu
     */
    virtual CARAPI AddSubMenu(
        /* [in] */ ICharSequence* title,
        /* [out] */ ISubMenu** subMenu);

    /**
     * Add a new sub-menu to the menu. This item displays the given title for
     * its label. To modify other attributes on the submenu's menu item, use
     * {@link SubMenu#getItem()}.
     *
     * @param titleRes Resource identifier of title string.
     * @return The newly added sub-menu
     */
    virtual CARAPI AddSubMenuEx(
        /* [in] */ Int32 titleRes,
        /* [out] */ ISubMenu** subMenu);

    /**
     * Add a new sub-menu to the menu. This item displays the given
     * <var>title</var> for its label. To modify other attributes on the
     * submenu's menu item, use {@link SubMenu#getItem()}.
     *<p>
     * Note that you can only have one level of sub-menus, i.e. you cannnot add
     * a subMenu to a subMenu: An {@link UnsupportedOperationException} will be
     * thrown if you try.
     *
     * @param groupId The group identifier that this item should be part of.
     *        This can also be used to define groups of items for batch state
     *        changes. Normally use {@link #NONE} if an item should not be in a
     *        group.
     * @param itemId Unique item ID. Use {@link #NONE} if you do not need a
     *        unique ID.
     * @param order The order for the item. Use {@link #NONE} if you do not care
     *        about the order. See {@link MenuItem#getOrder()}.
     * @param title The text to display for the item.
     * @return The newly added sub-menu
     */
    virtual CARAPI AddSubMenuEx2(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ ICharSequence* title,
        /* [out] */ ISubMenu** subMenu);

    /**
     * Variation on {@link #addSubMenu(int, int, int, CharSequence)} that takes
     * a string resource identifier for the title instead of the string itself.
     *
     * @param groupId The group identifier that this item should be part of.
     *        This can also be used to define groups of items for batch state
     *        changes. Normally use {@link #NONE} if an item should not be in a group.
     * @param itemId Unique item ID. Use {@link #NONE} if you do not need a unique ID.
     * @param order The order for the item. Use {@link #NONE} if you do not care about the
     *        order. See {@link MenuItem#getOrder()}.
     * @param titleRes Resource identifier of title string.
     * @return The newly added sub-menu
     */
    virtual CARAPI AddSubMenuEx3(
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 itemId,
        /* [in] */ Int32 order,
        /* [in] */ Int32 titleRes,
        /* [out] */ ISubMenu** subMenu);


    /**
     * Add a group of menu items corresponding to actions that can be performed
     * for a particular Intent. The Intent is most often configured with a null
     * action, the data that the current activity is working with, and includes
     * either the {@link Intent#CATEGORY_ALTERNATIVE} or
     * {@link Intent#CATEGORY_SELECTED_ALTERNATIVE} to find activities that have
     * said they would like to be included as optional action. You can, however,
     * use any Intent you want.
     *
     * <p>
     * See {@link android.content.pm.PackageManager#queryIntentActivityOptions}
     * for more * details on the <var>caller</var>, <var>specifics</var>, and
     * <var>intent</var> arguments. The list returned by that function is used
     * to populate the resulting menu items.
     *
     * <p>
     * All of the menu items of possible options for the intent will be added
     * with the given group and id. You can use the group to control ordering of
     * the items in relation to other items in the menu. Normally this function
     * will automatically remove any existing items in the menu in the same
     * group and place a divider above and below the added items; this behavior
     * can be modified with the <var>flags</var> parameter. For each of the
     * generated items {@link MenuItem#setIntent} is called to associate the
     * appropriate Intent with the item; this means the activity will
     * automatically be started for you without having to do anything else.
     *
     * @param groupId The group identifier that the items should be part of.
     *        This can also be used to define groups of items for batch state
     *        changes. Normally use {@link #NONE} if the items should not be in
     *        a group.
     * @param itemId Unique item ID. Use {@link #NONE} if you do not need a
     *        unique ID.
     * @param order The order for the items. Use {@link #NONE} if you do not
     *        care about the order. See {@link MenuItem#getOrder()}.
     * @param caller The current activity component name as defined by
     *        queryIntentActivityOptions().
     * @param specifics Specific items to place first as defined by
     *        queryIntentActivityOptions().
     * @param intent Intent describing the kinds of items to populate in the
     *        list as defined by queryIntentActivityOptions().
     * @param flags Additional options controlling how the items are added.
     * @param outSpecificItems Optional array in which to place the menu items
     *        that were generated for each of the <var>specifics</var> that were
     *        requested. Entries may be null if no activity was found for that
     *        specific action.
     * @return The number of menu items that were added.
     *
     * @see #FLAG_APPEND_TO_GROUP
     * @see MenuItem#setIntent
     * @see android.content.pm.PackageManager#queryIntentActivityOptions
     */
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

    /**
     * Remove the item with the given identifier.
     *
     * @param id The item to be removed.  If there is no item with this
     *           identifier, nothing happens.
     */
    virtual CARAPI RemoveItem(
        /* [in] */ Int32 id);

    /**
     * Remove all items in the given group.
     *
     * @param groupId The group to be removed.  If there are no items in this
     *           group, nothing happens.
     */
    virtual CARAPI RemoveGroup(
        /* [in] */ Int32 groupId);

    /**
     * Remove all existing items from the menu, leaving it empty as if it had
     * just been created.
     */
    virtual CARAPI Clear();

    /**
     * Control whether a particular group of items can show a check mark.  This
     * is similar to calling {@link MenuItem#setCheckable} on all of the menu items
     * with the given group identifier, but in addition you can control whether
     * this group contains a mutually-exclusive set items.  This should be called
     * after the items of the group have been added to the menu.
     *
     * @param group The group of items to operate on.
     * @param checkable Set to true to allow a check mark, false to
     *                  disallow.  The default is false.
     * @param exclusive If set to true, only one item in this group can be
     *                  checked at a time; checking an item will automatically
     *                  uncheck all others in the group.  If set to false, each
     *                  item can be checked independently of the others.
     *
     * @see MenuItem#setCheckable
     * @see MenuItem#setChecked
     */
    virtual CARAPI SetGroupCheckable(
        /* [in] */ Int32 group,
        /* [in] */ Boolean checkable,
        /* [in] */ Boolean exclusive);

    /**
     * Show or hide all menu items that are in the given group.
     *
     * @param group The group of items to operate on.
     * @param visible If true the items are visible, else they are hidden.
     *
     * @see MenuItem#setVisible
     */
    virtual CARAPI SetGroupVisible(
        /* [in] */ Int32 group,
        /* [in] */ Boolean visible);

    /**
     * Enable or disable all menu items that are in the given group.
     *
     * @param group The group of items to operate on.
     * @param enabled If true the items will be enabled, else they will be disabled.
     *
     * @see MenuItem#setEnabled
     */
    virtual CARAPI SetGroupEnabled(
        /* [in] */ Int32 group,
        /* [in] */ Boolean enabled);

    /**
     * Return whether the menu currently has item items that are visible.
     *
     * @return True if there is one or more item visible,
     *         else false.
     */
    virtual CARAPI HasVisibleItems(
        /* [out] */ Boolean* result);

    /**
     * Return the menu item with a particular identifier.
     *
     * @param id The identifier to find.
     *
     * @return The menu item object, or null if there is no item with
     *         this identifier.
     */
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

    /**
     * Get the number of items in the menu.  Note that this will change any
     * times items are added or removed from the menu.
     *
     * @return The item count.
     */
    virtual CARAPI Size(
        /* [out] */ Int32* size);

    /**
     * Gets the menu item at the given index.
     *
     * @param index The index of the menu item to return.
     * @return The menu item.
     * @exception IndexOutOfBoundsException
     *                when {@code index < 0 || >= size()}
     */
    virtual CARAPI GetItem(
        /* [in] */ Int32 index,
        /* [out] */ IMenuItem** item);

    /**
     * Is a keypress one of the defined shortcut keys for this window.
     * @param keyCode the key code from {@link KeyEvent} to check.
     * @param event the {@link KeyEvent} to use to help check.
     */
    virtual CARAPI IsShortcutKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* isShortcutKey);

    /**
     * Execute the menu item action associated with the given menu identifier.
     *
     * @param id Identifier associated with the menu item.
     * @param flags Additional option flags or 0.
     *
     * @return If the given identifier exists and is shown, returns
     *         true; else returns false.
     *
     * @see #FLAG_PERFORM_NO_CLOSE
     */
    virtual CARAPI PerformIdentifierAction(
        /* [in] */ Int32 id,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* isShown);


    /**
     * Control whether the menu should be running in qwerty mode (alphabetic
     * shortcuts) or 12-key mode (numeric shortcuts).
     *
     * @param isQwerty If true the menu will use alphabetic shortcuts; else it
     *                 will use numeric shortcuts.
     */
    virtual CARAPI SetQwertyMode(
        /* [in] */ Boolean isQwerty);

    /**
     * @return whether the menu shortcuts are in qwerty mode or not
     */
    virtual CARAPI IsQwertyMode(
        /* [out] */ Boolean* qwertyMode);

    /**
     * Sets whether the shortcuts should be visible on menus.  Devices without hardware
     * key input will never make shortcuts visible even if this method is passed 'true'.
     *
     * @param shortcutsVisible Whether shortcuts should be visible (if true and a
     *            menu item does not have a shortcut defined, that item will
     *            still NOT show a shortcut)
     */
    virtual CARAPI SetShortcutsVisible(
        /* [in] */ Boolean shortcutsVisible);

    /**
     * @return Whether shortcuts should be visible on menus.
     */
    virtual CARAPI IsShortcutsVisible(
        /* [out] */ Boolean* qwertyMode);

    virtual CARAPI GetResources(
        /* [out] */ IResources** resource);

    virtual CARAPI GetCallback(
        /* [out] */ IMenuBuilderCallback** callback);

    virtual CARAPI GetContext(
        /* [out] */ IContext** context);

    /**
     * Execute the menu item action associated with the given shortcut
     * character.
     *
     * @param keyCode The keycode of the shortcut key.
     * @param event Key event message.
     * @param flags Additional option flags or 0.
     *
     * @return If the given shortcut exists and is shown, returns
     *         true; else returns false.
     *
     * @see #FLAG_PERFORM_NO_CLOSE
     */
    virtual CARAPI PerformShortcut(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* handled);

    /*
     * This function will return all the menu and sub-menu items that can
     * be directly (the shortcut directly corresponds) and indirectly
     * (the ALT-enabled char corresponds to the shortcut) associated
     * with the keyCode.
     */
    virtual CARAPI FindItemsWithShortcutForKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ IObjectContainer** list);

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
    virtual CARAPI FindItemWithShortcutForKey(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ IMenuItem** menuItem);

    virtual CARAPI PerformItemAction(
        /* [in] */ IMenuItem* item,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* state);

    /**
     * Closes the visible menu.
     *
     * @param allMenusAreClosing Whether the menus are completely closing (true),
     *            or whether there is another menu coming in this menu's place
     *            (false). For example, if the menu is closing because a
     *            sub menu is about to be shown, <var>allMenusAreClosing</var>
     *            is false.
     */
    virtual CARAPI CloseEx(
        /* [in] */ Boolean allMenusAreClosing);

    /** {@inheritDoc} */
    virtual CARAPI Close();

    /**
     * Called by {@link CMenuItemImpl*} when its visible flag is changed.
     * @param item The item that has gone through a visibility change.
     */
    virtual CARAPI OnItemVisibleChanged(
        /* [in] */ IMenuItem* item);

    virtual CARAPI GetVisibleItems(
        /* [out] */ IObjectContainer** list);

    virtual CARAPI ClearHeader();

    virtual CARAPI GetHeaderTitle(
        /* [out] */ ICharSequence** title);

    virtual CARAPI GetHeaderIcon(
        /* [out] */ IDrawable** drawable);

    virtual CARAPI GetHeaderView(
        /* [out] */ IView** headerView);

    /**
     * Gets the root menu (if this is a submenu, find its root menu).
     * @return The root menu.
     */
    virtual CARAPI GetRootMenu(
        /* [out] */ IMenuBuilder** menu);

    /**
     * Sets the current menu info that is set on all items added to this menu
     * (until this is called again with different menu info, in which case that
     * one will be added to all subsequent item additions).
     *
     * @param menuInfo The extra menu information to add.
     */
    virtual CARAPI SetCurrentMenuInfo(
        /* [in] */ IContextMenuInfo* menuInfo);

    /**
     * Gets an adapter for providing items and their views.
     *
     * @param menuType The type of menu to get an adapter for.
     * @return A {@link MenuAdapter} for this menu with the given menu type.
     */
    virtual CARAPI GetMenuAdapter(
        /* [in] */ Int32 menuType,
        /* [out] */ IMenuAdapter** menuAdapter);

    virtual CARAPI SetOptionalIconsVisible(
        /* [in] */ Boolean visible);

    virtual CARAPI GetOptionalIconsVisible(
        /* [out] */ Boolean* visible);

    virtual CARAPI SaveHierarchyState(
        /* [in] */ IBundle* outState);

    virtual CARAPI RestoreHierarchyState(
        /* [in] */ IBundle* inState);

    virtual CARAPI SetExclusiveItemChecked(
        /* [in] */ IMenuItem* item);

    CARAPI SetHeaderTitleInt(
        /* [in] */ ICharSequence* title,
        /* [out] */ IMenuBuilder** menu);

    /**
     * Sets the header's title. This replaces the header view. Called by the
     * builder-style methods of subclasses.
     *
     * @param titleRes The new title (as a resource ID).
     * @return This CMenuBuilder so additional setters can be called.
     */
    CARAPI SetHeaderTitleIntEx(
        /* [in] */ Int32 titleRes,
        /* [out] */ IMenuBuilder** menu);

    /**
     * Sets the header's icon. This replaces the header view. Called by the
     * builder-style methods of subclasses.
     *
     * @param icon The new icon.
     * @return This CMenuBuilder so additional setters can be called.
     */
    CARAPI SetHeaderIconInt(
        /* [in] */ IDrawable* icon,
        /* [out] */ IMenuBuilder** menu);

    /**
     * Sets the header's icon. This replaces the header view. Called by the
     * builder-style methods of subclasses.
     *
     * @param iconRes The new icon (as a resource ID).
     * @return This CMenuBuilder so additional setters can be called.
     */
    CARAPI SetHeaderIconIntEx(
        /* [in] */ Int32 iconRes,
        /* [out] */ IMenuBuilder** menu);

    /**
     * Sets the header's view. This replaces the title and icon. Called by the
     * builder-style methods of subclasses.
     *
     * @param view The new view.
     * @return This CMenuBuilder so additional setters can be called.
     */
    CARAPI SetHeaderViewInt(
        /* [in] */ IView* view,
        /* [out] */ IMenuBuilder** menu);

    CARAPI GetNumIconMenuItemsShown(
        /* [out] */ Int32* state);

    CARAPI SetHeaderInternal(
        /* [in] */ Int32 titleRes,
        /* [in] */ ICharSequence* title,
        /* [in] */ Int32 iconRes,
        /* [in] */ IDrawable* icon,
        /* [in] */ IView* view);

    /**
     * Refreshes the shortcut labels on each of the displayed items.  Passes the arguments
     * so submenus don't need to call their parent menu for the same values.
     */
    CARAPI RefreshShortcuts(
        /* [in] */ Boolean shortcutsVisible,
        /* [in] */ Boolean qwertyMode);

    CARAPI ClearAll();

};

#endif    //__CMENUBUILDER_H__