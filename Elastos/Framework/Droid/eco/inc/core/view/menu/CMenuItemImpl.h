
#ifndef __MENUITEMIMPL_H__
#define __MENUITEMIMPL_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "_CMenuItemImpl.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/Vector.h>
#include <StringBuffer.h>
#include <elastos/ElRefBase.h>

using namespace Elastos;

/**
 * @hide
 */
CarClass(CMenuItemImpl)
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
    CMenuItemImpl(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title);

    CMenuItemImpl();

    ~CMenuItemImpl();

    virtual PInterface Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Int32 group,
        /* [in] */ Int32 id,
        /* [in] */ Int32 categoryOrder,
        /* [in] */ Int32 ordering,
        /* [in] */ ICharSequence* title);

    /**
     * Invokes the item by calling various listeners or callbacks.
     *
     * @return true if the invocation was handled, false otherwise
     */
    CARAPI Invoke(
         /* [out] */ Boolean* state);

    CARAPI IsEnabled(
        /* [out] */ Boolean* enabled);

    virtual CARAPI SetEnabled(
        /* [in] */ Boolean enabled,
        /* [out] */ IMenuItem** menuItem);

    CARAPI GetGroupId(
        /* [out] */ Int32* groupId);

    //@ViewDebug.CapturedViewProperty
    CARAPI GetItemId(
        /* [out] */ Int32* id);

    CARAPI GetOrder(
        /* [out] */ Int32* order);

    CARAPI GetOrdering(
        /* [out] */ Int32* ordering);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    virtual CARAPI SetIntent(
        /* [in] */ IIntent* intent,
        /* [out] */ IMenuItem** menuItem);

    CARAPI GetCallback(
        /* [out] */ IRunnable** runnable);

    CARAPI SetCallback(
        /* [in] */ IRunnable* callback,
        /* [out] */ IMenuItem** menuItem);

    CARAPI GetAlphabeticShortcut(
        /* [out] */ Char8* shortcut);

    virtual CARAPI SetAlphabeticShortcut(
        /* [in] */ Char8 alphaChar,
        /* [out] */ IMenuItem** menuItem);

    CARAPI GetNumericShortcut(
        /* [out] */ Char8* shortcut);

    virtual CARAPI SetNumericShortcut(
        /* [in] */ Char8 numericChar,
        /* [out] */ IMenuItem** menuItem);

    CARAPI SetShortcut(
        /* [in] */ Char8 numericChar,
        /* [in] */ Char8 alphaChar,
        /* [out] */ IMenuItem** menuItem);

    /**
     * @return The active shortcut (based on QWERTY-mode of the menu).
     */
    CARAPI GetShortcut(
        /* [out] */ Char8* shortcut);

    /**
     * @return The label to show for the shortcut. This includes the chording
     *         key (for example 'Menu+a'). Also, any non-human readable
     *         characters should be human readable (for example 'Menu+enter').
     */
    CARAPI GetShortcutLabel(
        /* [out] */String* label);

    /**
     * @return Whether this menu item should be showing shortcuts (depends on
     *         whether the menu should show shortcuts and whether this item has
     *         a shortcut defined)
     */
    CARAPI ShouldShowShortcut(
        /* [out] */ Boolean* show);

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
    CARAPI RefreshShortcutOnItemViews2(
        /* [in] */ Boolean menuShortcutShown,
        /* [in] */ Boolean isQwertyMode);

    CARAPI GetSubMenu(
        /* [out] */ ISubMenu** builder);

//    CARAPI GetSubMenu(
//       /* [out] */ ISubMenu** builder);

    CARAPI HasSubMenu(
        /* [out] */ Boolean* has);

    CARAPI SetSubMenu(
        /* [in] */ ISubMenu* subMenu);

    //@ViewDebug.CapturedViewProperty
    CARAPI GetTitle(
        /* [out] */ String * title);

    /**
     * Gets the title for a particular {@link ItemView}
     *
     * @param itemView The ItemView that is receiving the title
     * @return Either the title or condensed title based on what the ItemView
     *         prefers
     */
    CARAPI GetTitleForItemView(
        /* [in] */ IMenuItemView* itemView,
        /* [out] */ String* title);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title,
        /* [out] */ IMenuItem** menuItem);

    CARAPI SetTitleEx(
        /* [in] */ Int32 title,
        /* [out] */ IMenuItem** menuItem);

    CARAPI GetTitleCondensed(
        /* [out] */ String* title);

    virtual CARAPI SetTitleCondensed(
        /* [in] */ ICharSequence* title,
        /* [out] */ IMenuItem** menuItem);

    CARAPI GetIcon(
        /* [out] */ IDrawable** drawable);

    virtual CARAPI SetIcon(
        /* [in] */ IDrawable* icon,
        /* [out] */ IMenuItem** menuItem);

    virtual CARAPI SetIconEx(
        /* [in] */ Int32 iconResId,
        /* [out] */ IMenuItem** menuItem);

    CARAPI IsCheckable(
        /* [out] */ Boolean* checkable);

    virtual CARAPI SetCheckable(
        /* [in] */ Boolean checkable,
        /* [out] */ IMenuItem** menuItem);

    CARAPI SetExclusiveCheckable(
        /* [in] */ Boolean exclusive);

    CARAPI IsExclusiveCheckable(
        /* [out] */ Boolean* checkable);

    CARAPI IsChecked(
        /* [out] */ Boolean* checked) ;

    virtual CARAPI SetChecked(
        /* [in] */ Boolean checked,
        /* [out] */ IMenuItem** menuItem);

    CARAPI SetCheckedInt(
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
    CARAPI SetVisibleInt(
        /* [in] */ Boolean show,
        /* [out] */ Boolean* visiable);

    virtual CARAPI SetVisible(
        /* [in] */ Boolean shown,
        /* [out] */ IMenuItem** menuItem);

    CARAPI SetOnMenuItemClickListener(
        /* [in] */ IOnMenuItemClickListener* clickListener,
        /* [out] */ IMenuItem** menuItem);

    CARAPI GetItemView(
        /* [in] */ Int32 menuType,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI ClearItemViews();

    //@Override
    CARAPI ToString(
        /* [out] */ String* string);

    CARAPI SetMenuInfo(
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
        /* [out] */ ILayoutInflater** LayoutInflater);

    /**
     * @return Whether the given menu type should show icons for menu items.
     */
    CARAPI ShouldShowIcon(
        /* [in] */ Int32 menuType,
        /* [out] */ Boolean* show);

    CARAPI OnMenuItemClick(
        /* [in] */ IMenuItem*,
        /* [in] */ Elastos::Boolean*);
private:
    /**
     * Refreshes the shortcut shown on the ItemViews.  This method retrieves current
     * shortcut state (mode and shown) from the menu that contains this item.
     */
    CARAPI RefreshShortcutOnItemViews();

    CARAPI_(Boolean) HasItemView(
        /* [in] */ Int32 menuType);

    CARAPI SetIconOnViews(
        /* [in] */ IDrawable* icon) ;

    CARAPI_(Boolean) HaveAnyOpenedIconCapableItemViews();

    /**
     * Create and initializes a menu item view that implements {@link MenuView.ItemView}.
     * @param menuType The type of menu to get a View for (must be one of
     *            {@link MenuBuilder#TYPE_ICON}, {@link MenuBuilder#TYPE_EXPANDED},
     *            {@link MenuBuilder#TYPE_SUB}, {@link MenuBuilder#TYPE_CONTEXT}).
     * @return The inflated {@link MenuView.ItemView} that is ready for use
     */
     CARAPI CreateItemView(
        /* [in] */ int menuType,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IMenuItemView** itemView);

private:
    static String TAG;
    static const Int32 CHECKABLE      = 0x00000001;
    static const Int32 CHECKED        = 0x00000002;
    static const Int32 EXCLUSIVE      = 0x00000004;
    static const Int32 HIDDEN         = 0x00000008;
    static const Int32 ENABLED        = 0x00000010;

    /** Used for the icon resource ID if this item does not have an icon */
    static const Int32 NO_ICON = 0;

    Int32 mId;
    Int32 mGroup;
    Int32 mCategoryOrder;
    Int32 mOrdering;
    String mTitle;
    String mTitleCondensed;
    AutoPtr<IIntent> mIntent;
    Char8 mShortcutNumericChar;
    Char8 mShortcutAlphabeticChar;

    /** The icon's drawable which is only created as needed */
    IDrawable* mIconDrawable;
    /**
     * The icon's resource ID which is used to get the Drawable when it is
     * needed (if the Drawable isn't already obtained--only one of the two is
     * needed).
     */
    Int32 mIconResId;

    /** The (cached) menu item views for this item */
    AutoPtr<IMenuItemView>* mItemViews;

    /** The menu to which this item belongs */
    AutoPtr<IMenuBuilder> mMenu;
    /** If this item should launch a sub menu, this is the sub menu to launch */
    //AutoPtr<CSubMenuBuilder> mSubMenu;
    AutoPtr<ISubMenu> mSubMenu;

    AutoPtr<IRunnable> mItemCallback;
    AutoPtr<IOnMenuItemClickListener> mClickListener;

    Int32 mFlags;

    /**
     * Current use case is for context menu: Extra information linked to the
     * View that added this item to the context menu.
     */
    AutoPtr<IContextMenuInfo> mMenuInfo;

    static String sPrependShortcutLabel;
    static String sEnterShortcutLabel;
    static String sDeleteShortcutLabel;
    static String sSpaceShortcutLabel;
};

#endif    //__MENUITEMIMPL_H__
