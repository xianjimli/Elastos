
#ifndef __CLISTACTIVITY_H__
#define __CLISTACTIVITY_H__

#include "_CListActivity.h"
#include "app/ListActivity.h"


CarClass(CListActivity), public ListActivity
{
public:
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
};

#endif //__CLISTACTIVITY_H__
