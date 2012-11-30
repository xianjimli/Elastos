
#ifndef __CMENUINFLATER_H__
#define __CMENUINFLATER_H__

#include "_CMenuInflater.h"
#include <elastos/AutoPtr.h>

/**
 * This class is used to instantiate menu XML files into Menu objects.
 * <p>
 * For performance reasons, menu inflation relies heavily on pre-processing of
 * XML files that is done at build time. Therefore, it is not currently possible
 * to use MenuInflater with an XmlPullParser over a plain XML file at runtime;
 * it only works with an XmlPullParser returned from a compiled resource (R.
 * <em>something</em> file.)
 */
CarClass(CMenuInflater)
{
private:
    /**
     * State for the current menu.
     * <p>
     * Groups can not be nested unless there is another menu (which will have
     * its state class).
     */
    class MenuState
    {
    public:
        MenuState(
            /* [in] */ IMenu* menu,
            /* [in] */ CMenuInflater* host);

        CARAPI_(void) ResetGroup();

        /**
         * Called when the parser is pointing to a group tag.
         */
        CARAPI_(void) ReadGroup(
            /* [in] */ IAttributeSet* attrs);

        /**
         * Called when the parser is pointing to an item tag.
         */
        CARAPI_(void) ReadItem(
            /* [in] */ IAttributeSet* attrs);

        CARAPI_(void) AddItem();

        CARAPI_(AutoPtr<ISubMenu>) AddSubMenuItem();

        CARAPI_(Boolean) HasAddedItem();

    private:
        CARAPI_(Char32) GetShortcut(
            /* [in] */ const String& shortcutString);

        CARAPI_(void) SetItem(
            /* [in, out] */ IMenuItem* item);

    private:
        CMenuInflater* mHost;
        AutoPtr<IMenu> mMenu;

        /*
         * Group state is set on items as they are added, allowing an item to
         * override its group state. (As opposed to set on items at the group end tag.)
         */
        Int32 mGroupId;
        Int32 mGroupCategory;
        Int32 mGroupOrder;
        Int32 mGroupCheckable;
        Boolean mGroupVisible;
        Boolean mGroupEnabled;

        Boolean mItemAdded;
        Int32 mItemId;
        Int32 mItemCategoryOrder;
        String mItemTitle;
        String mItemTitleCondensed;
        Int32 mItemIconResId;
        Char32 mItemAlphabeticShortcut;
        Char32 mItemNumericShortcut;
        /**
         * Sync to attrs.xml enum:
         * - 0: none
         * - 1: all
         * - 2: exclusive
         */
        Int32 mItemCheckable;
        Boolean mItemChecked;
        Boolean mItemVisible;
        Boolean mItemEnabled;

        static const Int32 mDefaultGroupId;
        static const Int32 mDefaultItemId;
        static const Int32 mDefaultItemCategory = 0;
        static const Int32 mDefaultItemOrder = 0;
        static const Int32 mDefaultItemCheckable = 0;
        static const Boolean mDefaultItemChecked = FALSE;
        static const Boolean mDefaultItemVisible = TRUE;
        static const Boolean mDefaultItemEnabled = TRUE;
    };

public:
    CMenuInflater();

    /**
     * Constructs a menu inflater.
     *
     * @see Activity#getMenuInflater()
     */
    CARAPI constructor (
        /* [in] */ IContext* ctx);

    /**
     * Inflate a menu hierarchy from the specified XML resource. Throws
     * {@link InflateException} if there is an error.
     *
     * @param menuRes Resource ID for an XML layout resource to load (e.g.,
     *            <code>R.menu.main_activity</code>)
     * @param menu The Menu to inflate into. The items and submenus will be
     *            added to this Menu.
     */
    CARAPI Inflate(
        /* [in] */ Int32 menuRes,
        /* [in, out] */ IMenu* menu);

private:
    /**
     * Called internally to fill the given menu. If a sub menu is seen, it will
     * call this recursively.
     */
    CARAPI ParseMenu(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in, out] */ IMenu* menu);


private:
    /** Menu tag name in XML. */
    static const CString XML_MENU;

    /** Group tag name in XML. */
    static const CString XML_GROUP;

    /** Item tag name in XML. */
    static const CString XML_ITEM;

    static const Int32 NO_ID = 0;

    AutoPtr<IContext> mContext;
};

#endif  //__CMENUINFLATER_H__
