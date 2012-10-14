
#ifndef __SIMPLEADAPTER_H__
#define __SIMPLEADAPTER_H__

#include "widget/BaseAdapter.h"
#include "widget/Filter.h"
#include <elastos/List.h>

class SimpleAdapter : public BaseAdapter
{
private:
    /**
     * <p>An array filters constrains the content of the array adapter with
     * a prefix. Each item that does not start with the supplied prefix
     * is removed from the list.</p>
     */
    class SimpleFilter : public Filter
    {
    public:
        SimpleFilter(
            /* [in] */ SimpleAdapter* host);

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
        SimpleAdapter* mHost;
    };

public:
    SimpleAdapter();

    SimpleAdapter(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* data,
        /* [in] */ Int32 resource,
        /* [in] */ const ArrayOf<String>& from,
        /* [in] */ const ArrayOf<Int32>& to);

    CARAPI_(Int32) GetCount();

    CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position);

    CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI SetDropDownViewResource(
        /* [in] */ Int32 resource);

    CARAPI_(AutoPtr<IView>) GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI_(AutoPtr<IViewBinder>) GetViewBinder();

    virtual CARAPI SetViewBinder(
        /* [in] */ IViewBinder* viewBinder);

    virtual CARAPI SetViewImage(
        /* [in] */ IImageView* v,
        /* [in] */ Int32 value);

    virtual CARAPI SetViewImage(
        /* [in] */ IImageView* v,
        /* [in] */ const String& value);

    virtual CARAPI SetViewText(
        /* [in] */ ITextView* v,
        /* [in] */ const String& text);

    virtual CARAPI_(AutoPtr<IFilter>) GetFilter();

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* data,
        /* [in] */ Int32 resource,
        /* [in] */ const ArrayOf<String>& from,
        /* [in] */ const ArrayOf<Int32>& to);

private:
    CARAPI_(AutoPtr<IView>) CreateViewFromResource(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [in] */ Int32 resource);

    CARAPI BindView(
        /* [in] */ Int32 position,
        /* [in] */ IView* view);

private:
    ArrayOf<Int32>* mTo;
    ArrayOf<String>* mFrom;
    AutoPtr<IViewBinder> mViewBinder;

    Vector<AutoPtr<IObjectStringMap> > mData;

    Int32 mResource;
    Int32 mDropDownResource;
    AutoPtr<ILayoutInflater> mInflater;

    AutoPtr<SimpleFilter> mFilter;
    Vector<AutoPtr<IObjectStringMap> >* mUnfilteredData;
};
#endif //__SIMPLEADAPTER_H__
