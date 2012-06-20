
#ifndef __BASEADAPTER_H__
#define __BASEADAPTER_H__

#include "ext/frameworkext.h"
#include "database/DataSetObservable.h"
#include <elastos/AutoPtr.h>

/**
 * Common base class of common implementation for an {@link Adapter} that can be
 * used in both {@link ListView} (by implementing the specialized
 * {@link ListAdapter} interface} and {@link Spinner} (by implementing the
 * specialized {@link SpinnerAdapter} interface.
 */
class BaseAdapter
{
protected:
    BaseAdapter() {}

public: 
    virtual CARAPI NotifyDataSetChanged();

    virtual CARAPI NotifyDataSetInvalidated();

    virtual CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    virtual CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    virtual CARAPI_(Int32) GetCount() = 0;

    virtual CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position) = 0;

    virtual CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position) = 0;

    virtual CARAPI_(Boolean) HasStableIds();

    virtual CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent) = 0;

    virtual CARAPI_(Int32) GetItemViewType(
        /* [in] */ Int32 position);

    virtual CARAPI_(Int32) GetViewTypeCount();

    virtual CARAPI_(Boolean) IsEmpty();

    virtual CARAPI_(Boolean) AreAllItemsEnabled();

    virtual CARAPI_(Boolean) IsEnabled(
        /* [in] */ Int32 position);

    virtual CARAPI_(AutoPtr<IView>) GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

private:
    AutoPtr<DataSetObservable> mDataSetObservable;
};
#endif //__BASEADAPTER_H__
