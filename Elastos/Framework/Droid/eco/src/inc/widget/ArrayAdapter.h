
#ifndef __ARRAYADAPTER_H__
#define __ARRAYADAPTER_H__

#include "widget/BaseAdapter.h"
#include "widget/Filter.h"
#include <elastos/List.h>

class ArrayAdapter : public BaseAdapter
{
private:
    /**
     * <p>An array filter constrains the content of the array adapter with
     * a prefix. Each item that does not start with the supplied prefix
     * is removed from the list.</p>
     */
    class ArrayFilter : public Filter
    {
    public:
        ArrayFilter(
            /* [in] */ ArrayAdapter* host);

    protected:
        //@Override
        CARAPI PerformFiltering(
            /* [in] */ ICharSequence* prefix,
            /* [out] */ IFilterResults** filterResults);

        //@Override
        CARAPI PublishResults(
            /* [in] */ ICharSequence* constraint,
            /* [in] */ IFilterResults* results);

    private:
        ArrayAdapter* mHost;
    };

public:
    ArrayAdapter();

    ArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textViewResourceId);

    ArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId);

    ArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects);

    ArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects);

    virtual ~ArrayAdapter();

    virtual CARAPI Add(
        /* [in] */ IInterface* object);

    virtual CARAPI Insert(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 index);

    virtual CARAPI Remove(
        /* [in] */ IInterface* object);

    virtual CARAPI Clear();

    virtual CARAPI Sort(
        /* [in] */ IComparator* comparator);

    CARAPI NotifyDataSetChanged();


    virtual CARAPI SetNotifyOnChange(
        /* [in] */ Boolean notifyOnChange);

    virtual CARAPI_(AutoPtr<IContext>) GetContext();

    Int32 GetCount();

    CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position);

    virtual CARAPI_(Int32) GetPosition(
        /* [in] */ IInterface* item);

    CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI SetDropDownViewResource(
        /* [in] */ Int32 resource);

    //@Override
    CARAPI_(AutoPtr<IView>) GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI_(AutoPtr<IFilter>) GetFilter();

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects = NULL);

private:
    CARAPI_(AutoPtr<IView>) CreateViewFromResource(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [in] */ Int32 resource);

private:
    /**
     * Contains the list of objects that represent the data of this ArrayAdapter.
     * The content of this list is referred to as "the array" in the documentation.
     */
    List<AutoPtr<IInterface> > mObjects;

    /**
     * Lock used to modify the content of {@link #mObjects}. Any write operation
     * performed on the array should be synchronized on this lock. This lock is also
     * used by the filter (see {@link #getFilter()} to make a synchronized copy of
     * the original array of data.
     */
    Mutex mLock;

    /**
     * The resource indicating what views to inflate to display the content of this
     * array adapter.
     */
    Int32 mResource;

    /**
     * The resource indicating what views to inflate to display the content of this
     * array adapter in a drop down widget.
     */
    Int32 mDropDownResource;

    /**
     * If the inflated resource is not a TextView, {@link #mFieldId} is used to find
     * a TextView inside the inflated views hierarchy. This field must contain the
     * identifier that matches the one defined in the resource file.
     */
    Int32 mFieldId;

    /**
     * Indicates whether or not {@link #notifyDataSetChanged()} must be called whenever
     * {@link #mObjects} is modified.
     */
    Boolean mNotifyOnChange;

    AutoPtr<IContext> mContext;

    Vector<AutoPtr<IInterface> >* mOriginalValues;
    AutoPtr<ArrayFilter> mFilter;

    AutoPtr<ILayoutInflater> mInflater;
};
#endif //__ARRAYADAPTER_H__
