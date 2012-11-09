
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
CarClass(CMenuInflater) {
public:
    /**
     * Constructs a menu inflater.
     *
     * @see Activity#getMenuInflater()
     */
    CMenuInflater(
        /* [in] */ IContext* ctx);

    CMenuInflater();

    ECode constructor (
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
    ECode Inflate(
        /* [in] */ Int32 menuRes,
        /* [in] */ IMenu* menu);

private:
    /**
     * Called internally to fill the given menu. If a sub menu is seen, it will
     * call this recursively.
     */
    void ParseMenu(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ IMenu* menu);


private:
    /** Menu tag name in XML. */
    static const CString XML_MENU;

    /** Group tag name in XML. */
    static const CString XML_GROUP;

    /** Item tag name in XML. */
    static const CString XML_ITEM;

    static const Int32 NO_ID = 0;

    AutoPtr<IContext> mContext;

    /**
     * State for the current menu.
     * <p>
     * Groups can not be nested unless there is another menu (which will have
     * its state class).
     */
private:
    class MenuState {
        public:
            MenuState(
                /* [in] */ IMenu* menu,
                /* [in] */ CMenuInflater* host);

            void ResetGroup();

            /**
             * Called when the parser is pointing to a group tag.
             */
            void ReadGroup(
                /* [in] */ IAttributeSet* attrs);

            /**
             * Called when the parser is pointing to an item tag.
             */
            void ReadItem(
                /* [in] */ IAttributeSet* attrs);

            void AddItem();

            AutoPtr<ISubMenu> AddSubMenuItem();

            Boolean HasAddedItem();

        private:
            Char8 GetShortcut(
                /* [in] */ String shortcutString);

            void SetItem(
                /* [in] */ IMenuItem* item);

        private:
            AutoPtr<IMenu> mMenu;
            CMenuInflater* mHost;

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
            Char8 mItemAlphabeticShortcut;
            Char8 mItemNumericShortcut;
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

            static const Int32 mDefaultGroupId = NO_ID;
            static const Int32 mDefaultItemId = NO_ID;
            static const Int32 mDefaultItemCategory = 0;
            static const Int32 mDefaultItemOrder = 0;
            static const Int32 mDefaultItemCheckable = 0;
            static const Boolean mDefaultItemChecked = FALSE;
            static const Boolean mDefaultItemVisible = TRUE;
            static const Boolean mDefaultItemEnabled = TRUE;
    };
};

#endif  //__CMENUINFLATER_H__
