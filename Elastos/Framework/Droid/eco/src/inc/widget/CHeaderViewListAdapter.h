
#ifndef __CHEADERVIEWLISTADAPTER_H__
#define __CHEADERVIEWLISTADAPTER_H__

#include "_CHeaderViewListAdapter.h"
#include "widget/ListView.h"

CarClass(CHeaderViewListAdapter)
{
public:
    CHeaderViewListAdapter();

    CARAPI constructor(
        /* [in] */ IObjectContainer* headerViewInfos,
        /* [in] */ IObjectContainer* footerViewInfos,
        /* [in] */ IListAdapter* adapter);

    CARAPI GetHeadersCount(
        /* [out] */ Int32* count);

    CARAPI GetFootersCount(
        /* [out] */ Int32* count);

    CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty);

    CARAPI RemoveHeader(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

    CARAPI RemoveFooter(
        /* [in] */ IView* v,
        /* [out] */ Boolean* result);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI IsEnabled(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* enabled);

    CARAPI GetItem(
        /* [in] */ Int32 position,
        /* [out] */ IInterface** item);

    CARAPI GetItemId(
        /* [in] */ Int32 position,
        /* [out] */ Int64* itemId);

    CARAPI HasStableIds(
        /* [out] */ Boolean* hasStableIds);

    CARAPI GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI GetItemViewType(
        /* [in] */ Int32 position,
        /* [out] */ Int32* viewType);

    CARAPI GetViewTypeCount(
        /* [out] */ Int32* count);

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterDataSetObserver(
       /* [in] */ IDataSetObserver* observer);

    CARAPI GetFilter(
        /* [out] */ IFilter** filter);
    
    CARAPI GetWrappedAdapter(
        /* [out] */ IListAdapter** adapter);

private:
    Boolean AreAllListInfosSelectable(
        /* [in] */ Vector<AutoPtr<ListView::FixedViewInfo> >& infos);

private:
    AutoPtr<IListAdapter> mAdapter;

    Boolean mIsFilterable;

protected:
    // These two ArrayList are assumed to NOT be NULL.
    // They are indeed created when declared in ListView and then shared.
    Vector<AutoPtr<ListView::FixedViewInfo> > mHeaderViewInfos;
    Vector<AutoPtr<ListView::FixedViewInfo> > mFooterViewInfos;

    Boolean mAreAllFixedViewsSelectable;
};

#endif //__CHEADERVIEWLISTADAPTER_H__
