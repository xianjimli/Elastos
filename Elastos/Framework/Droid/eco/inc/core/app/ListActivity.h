
#include "app/Activity.h"
#include <elastos/Mutex.h>


class ListActivity : public Activity
{
public:
    ListActivity();
    /**
     * Updates the screen state (current list and other views) when the
     * content changes.
     *
     * @see Activity#onContentChanged()
     */
    //@Override
    CARAPI OnContentChanged();

    /**
     * Provide the cursor for the list view.
     */
    CARAPI SetListAdapter(
        /* [in] */ IListAdapter* adapter);

    /**
     * Set the currently selected list item to the specified
     * position with the adapter's data
     *
     * @param position
     */
    CARAPI SetSelection(
        /* [in] */ Int32 position);

    /**
     * Get the position of the currently selected list item.
     */
    CARAPI GetSelectedItemPosition(
        /* [out] */ Int32* position);

    /**
     * Get the cursor row ID of the currently selected list item.
     */
    CARAPI GetSelectedItemId(
        /* [out] */ Int64* id);

    /**
     * Get the activity's list view widget.
     */
    CARAPI GetListView(
        /* [out] */ IListView** listview);

    /**
     * Get the ListAdapter associated with this activity's ListView.
     */
    CARAPI GetListAdapter(
        /* [out] */ IListAdapter** listadapter);

protected:
        /**
     * This method will be called when an item in the list is selected.
     * Subclasses should override. Subclasses can call
     * getListView().getItemAtPosition(position) if they need to access the
     * data associated with the selected item.
     *
     * @param l The ListView where the click happened
     * @param v The view that was clicked within the ListView
     * @param position The position of the view in the list
     * @param id The row id of the item that was clicked
     */
    CARAPI_(void) OnListItemClick(
        /* [in] */ IListView* l,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    /**
     * Ensures the list view has been created before Activity restores all
     * of the view states.
     *
     *@see Activity#onRestoreInstanceState(Bundle)
     */
    //@Override
    virtual CARAPI OnRestoreInstanceState(
        /* [in] */ IBundle* state);

    /**
     * @see Activity#onDestroy()
     */
    //@Override
    virtual CARAPI OnDestroy();

private:
    CARAPI_(void) EnsureList();

protected:
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IListAdapter> mAdapter;

    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IListView> mList;

private:
    Boolean mFinishedStart;
    AutoPtr<IOnItemClickListener> mOnClickListener;
    Mutex mSync;
};
