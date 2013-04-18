
#ifndef __MENUITEMIMPL_H__
#define __MENUITEMIMPL_H__


#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Vector.h>
#include <StringBuffer.h>
#include <elastos/ElRefBase.h>

/**
 * @hide
 */
class MenuItemImpl
    : public ElRefBase
    , public IMenuItemImpl
{
public:
    /**
     * Instantiates this menu item. The constructor
     * {@link #MenuItemData(MenuBuilder, int, int, int, String, int)} is
     * preferred due to lazy loading of the icon Drawable.
     *
     * @param menu
     * @param group Item ordering grouping control. The item will be added after
     *            all other items whose order is <= this number, and before any
     *            that are larger than it. This can also be used to define
     *            groups of items for batch state changes. Normally use 0.
     * @param id Unique item ID. Use 0 if you do not need a unique ID.
     * @param categoryOrder The ordering for this item.
     * @param title The text to display for the item.
     */
    MenuItemImpl(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title);

    ~MenuItemImpl();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Invokes the item by calling various listeners or callbacks.
     *
     * @return true if the invocation was handled, false otherwise
     */
    CARAPI Invoke(
        /* [out] */ Boolean* handled);

    CARAPI IsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetGroupId(
        /* [out] */ Int32* groupId);

    CARAPI GetItemId(
        /* [out] */ Int32* id);

    CARAPI GetOrder(
        /* [out] */ Int32* order);

    CARAPI GetOrdering(
        /* [out] */ Int32* ordering);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI SetIntent(
        /* [in] */ IIntent* intent);

    CARAPI_(AutoPtr<IRunnable>) GetCallback();

    CARAPI SetCallback(
        /* [in] */ IRunnable* callback);

    CARAPI GetAlphabeticShortcut(
        /* [out] */ Char32* shortcut);

    CARAPI SetAlphabeticShortcut(
        /* [in] */ Char32 alphaChar);

    CARAPI GetNumericShortcut(
        /* [out] */ Char32* shortcut);

    CARAPI SetNumericShortcut(
        /* [in] */ Char32 numericChar);

    CARAPI SetShortcut(
        /* [in] */ Char32 numericChar,
        /* [in] */ Char32 alphaChar);

    /**
     * @return The active shortcut (based on QWERTY-mode of the menu).
     */
    CARAPI_(Char32) GetShortcut();

    /**
     * @return The label to show for the shortcut. This includes the chording
     *         key (for example 'Menu+a'). Also, any non-human readable
     *         characters should be human readable (for example 'Menu+enter').
     */
    CARAPI_(String) GetShortcutLabel();

    /**
     * @return Whether this menu item should be showing shortcuts (depends on
     *         whether the menu should show shortcuts and whether this item has
     *         a shortcut defined)
     */
    CARAPI_(Boolean) ShouldShowShortcut();

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
    CARAPI_(void) RefreshShortcutOnItemViews(
        /* [in] */ Boolean menuShortcutShown,
        /* [in] */ Boolean isQwertyMode);

    CARAPI GetSubMenu(
        /* [out] */ ISubMenu** subMenu);

    CARAPI HasSubMenu(
        /* [out] */ Boolean* has);

    CARAPI_(void) SetSubMenu(
        /* [in] */ ISubMenuBuilder* subMenu);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    /**
     * Gets the title for a particular {@link ItemView}
     *
     * @param itemView The ItemView that is receiving the title
     * @return Either the title or condensed title based on what the ItemView
     *         prefers
     */
    CARAPI_(AutoPtr<ICharSequence>) GetTitleForItemView(
        /* [in] */ IMenuItemView* itemView);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetTitleEx(
        /* [in] */ Int32 title);

    CARAPI GetTitleCondensed(
        /* [out] */ ICharSequence** title);

    CARAPI SetTitleCondensed(
        /* [in] */ ICharSequence* title);

    CARAPI GetIcon(
        /* [out] */ IDrawable** drawable);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI SetIconEx(
        /* [in] */ Int32 iconResId);

    CARAPI IsCheckable(
        /* [out] */ Boolean* checkable);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetExclusiveCheckable(
        /* [in] */ Boolean exclusive);

    CARAPI IsExclusiveCheckable(
        /* [out] */ Boolean* checkable);

    CARAPI IsChecked(
        /* [out] */ Boolean* checked) ;

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI_(void) SetCheckedInt(
        /* [in] */ Boolean checked);

    CARAPI IsVisible(
        /* [out] */ Boolean* isVisible);

    /**
     * Changes the visibility of the item. This method DOES NOT notify the
     * parent menu of a change in this item, so this should only be called from
     * methods that will eventually trigger this change.  If unsure, use {@link #setVisible(Boolean)}
     * instead.
     *
     * @param shown Whether to show (true) or hide (false).
     * @return Whether the item's shown state was changed
     */
    CARAPI_(Boolean) SetVisibleInt(
        /* [in] */ Boolean show);

    CARAPI SetVisible(
        /* [in] */ Boolean shown);

    CARAPI SetOnMenuItemClickListener(
        /* [in] */ IOnMenuItemClickListener* clickListener);

    CARAPI_(AutoPtr<IView>) GetItemView(
        /* [in] */ Int32 menuType,
        /* [in] */ IViewGroup* parent);

    CARAPI_(void) ClearItemViews();

    //@Override
    CARAPI ToString(
        /* [out] */ String* string);

    CARAPI_(void) SetMenuInfo(
        /* [in] */ IContextMenuInfo* menuInfo);

    CARAPI GetMenuInfo(
        /* [out] */ IContextMenuInfo** menuInfo);

    /**
     * Returns a LayoutInflater that is themed for the given menu type.
     *
     * @param menuType The type of menu.
     * @return A LayoutInflater.
     */
    CARAPI GetLayoutInflater(
        /* [in] */ Int32 menuType,
        /* [out] */ ILayoutInflater** inflater);

    /**
     * @return Whether the given menu type should show icons for menu items.
     */
    CARAPI ShouldShowIcon(
        /* [in] */ Int32 menuType,
        /* [out] */ Boolean* shown);

private:
    CARAPI_(Boolean) HasItemView(
        /* [in] */ Int32 menuType);

    /**
     * Refreshes the shortcut shown on the ItemViews.  This method retrieves current
     * shortcut state (mode and shown) from the menu that contains this item.
     */
    CARAPI_(void) RefreshShortcutOnItemViews();

    CARAPI_(void) SetIconOnViews(
        /* [in] */ IDrawable* icon) ;

    CARAPI_(Boolean) HaveAnyOpenedIconCapableItemViews();

    /**
     * Create and initializes a menu item view that implements {@link MenuView.ItemView}.
     * @param menuType The type of menu to get a View for (must be one of
     *            {@link MenuBuilder#TYPE_ICON}, {@link MenuBuilder#TYPE_EXPANDED},
     *            {@link MenuBuilder#TYPE_SUB}, {@link MenuBuilder#TYPE_CONTEXT}).
     * @return The inflated {@link MenuView.ItemView} that is ready for use
     */
     CARAPI_(AutoPtr<IMenuItemView>) CreateItemView(
        /* [in] */ int menuType,
        /* [in] */ IViewGroup* parent);

private:
    static CString TAG;
    static const Int32 CHECKABLE      = 0x00000001;
    static const Int32 CHECKED        = 0x00000002;
    static const Int32 EXCLUSIVE      = 0x00000004;
    static const Int32 HIDDEN         = 0x00000008;
    static const Int32 ENABLED        = 0x00000010;

    /** Used for the icon resource ID if this item does not have an icon */
    static const Int32 NO_ICON = 0;

    static String sPrependShortcutLabel;
    static String sEnterShortcutLabel;
    static String sDeleteShortcutLabel;
    static String sSpaceShortcutLabel;

    Int32 mId;
    Int32 mGroup;
    Int32 mCategoryOrder;
    Int32 mOrdering;
    AutoPtr<ICharSequence> mTitle;
    AutoPtr<ICharSequence> mTitleCondensed;
    AutoPtr<IIntent> mIntent;
    Char32 mShortcutNumericChar;
    Char32 mShortcutAlphabeticChar;

    /** The icon's drawable which is only created as needed */
    AutoPtr<IDrawable> mIconDrawable;
    /**
     * The icon's resource ID which is used to get the Drawable when it is
     * needed (if the Drawable isn't already obtained--only one of the two is
     * needed).
     */
    Int32 mIconResId;

    /** The (cached) menu item views for this item */
    // WeakReference<ItemView>
    ArrayOf< AutoPtr<IMenuItemView> >* mItemViews;

    /** The menu to which this item belongs */
    AutoPtr<IMenuBuilder> mMenu;
    /** If this item should launch a sub menu, this is the sub menu to launch */
    AutoPtr<ISubMenuBuilder> mSubMenu;

    AutoPtr<IRunnable> mItemCallback;
    AutoPtr<IOnMenuItemClickListener> mClickListener;

    Int32 mFlags;

    /**
     * Current use case is for context menu: Extra information linked to the
     * View that added this item to the context menu.
     */
    AutoPtr<IContextMenuInfo> mMenuInfo;
};

#endif    //__MENUITEMIMPL_H__
