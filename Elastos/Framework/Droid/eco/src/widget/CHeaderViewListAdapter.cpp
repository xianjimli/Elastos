
#include "widget/CHeaderViewListAdapter.h"

CHeaderViewListAdapter::CHeaderViewListAdapter()
    : mIsFilterable(FALSE)
    , mAreAllFixedViewsSelectable(FALSE)
{}

ECode CHeaderViewListAdapter::constructor(
    /* [in] */ IObjectContainer* headerViewInfos,
    /* [in] */ IObjectContainer* footerViewInfos,
    /* [in] */ IListAdapter* adapter)
{
    mAdapter = adapter;
    mIsFilterable = adapter->Probe(EIID_IFilterable) != NULL;

    if (headerViewInfos != NULL) {
        AutoPtr<IObjectEnumerator> it;
        headerViewInfos->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean hasNext;
        while (it->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> obj;
            it->Current((IInterface**)&obj);
            IFixedViewInfo* info = IFixedViewInfo::Probe(obj.Get());
            mHeaderViewInfos.PushBack((ListView::FixedViewInfo*)info);
        }
    }

    if (footerViewInfos != NULL) {
        AutoPtr<IObjectEnumerator> it;
        footerViewInfos->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean hasNext;
        while (it->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> obj;
            it->Current((IInterface**)&obj);
            IFixedViewInfo* info = IFixedViewInfo::Probe(obj.Get());
            mFooterViewInfos.PushBack((ListView::FixedViewInfo*)info);
        }
    }

    mAreAllFixedViewsSelectable =
        AreAllListInfosSelectable(mHeaderViewInfos)
        && AreAllListInfosSelectable(mFooterViewInfos);

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetHeadersCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mHeaderViewInfos.GetSize();

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetFootersCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mFooterViewInfos.GetSize();

    return NOERROR;
}

ECode CHeaderViewListAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    *isEmpty = TRUE;
    if (mAdapter != NULL) {
        mAdapter->IsEmpty(isEmpty);
    }

    return NOERROR;
}

Boolean CHeaderViewListAdapter::AreAllListInfosSelectable(
    /* [in] */ Vector<AutoPtr<ListView::FixedViewInfo> >& infos)
{
    Vector<AutoPtr<ListView::FixedViewInfo> >::Iterator iter;
    for (iter = infos.Begin(); iter != infos.End(); ++iter) {
        if (!(*iter)->mIsSelectable) {
            return FALSE;
        }
    }

    return TRUE;
}

ECode CHeaderViewListAdapter::RemoveHeader(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;

    Vector<AutoPtr<ListView::FixedViewInfo> >::Iterator iter;
    for (iter = mHeaderViewInfos.Begin(); iter != mHeaderViewInfos.End(); ++iter) {
        if ((*iter)->mView.Get() == v) {
            mHeaderViewInfos.Erase(iter);

            mAreAllFixedViewsSelectable =
                AreAllListInfosSelectable(mHeaderViewInfos)
                && AreAllListInfosSelectable(mFooterViewInfos);

            *result = TRUE;
            break;
        }
    }

    return NOERROR;
}

ECode CHeaderViewListAdapter::RemoveFooter(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;

    Vector<AutoPtr<ListView::FixedViewInfo> >::Iterator iter;
    for (iter = mFooterViewInfos.Begin(); iter != mFooterViewInfos.End(); ++iter) {
        if ((*iter)->mView.Get() == v) {
            mFooterViewInfos.Erase(iter);

            mAreAllFixedViewsSelectable =
                AreAllListInfosSelectable(mHeaderViewInfos)
                && AreAllListInfosSelectable(mFooterViewInfos);

            *result = TRUE;
            break;
        }
    }

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    if (mAdapter != NULL) {
        mAdapter->GetCount(count);
        *count = mFooterViewInfos.GetSize() + mHeaderViewInfos.GetSize() + *count;
    }
    else {
        *count = mFooterViewInfos.GetSize() + mHeaderViewInfos.GetSize();
    }

    return NOERROR;
}

ECode CHeaderViewListAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    if (mAdapter != NULL) {
        mAdapter->AreAllItemsEnabled(enabled);
        *enabled = mAreAllFixedViewsSelectable && *enabled;
    }
    else {
        *enabled = TRUE;
    }

    return NOERROR;
}

ECode CHeaderViewListAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    // Header (negative positions will throw an ArrayIndexOutOfBoundsException)
    Int32 numHeaders = mHeaderViewInfos.GetSize();
    if (position < numHeaders) {
        *enabled = mHeaderViewInfos[position]->mIsSelectable;

        return NOERROR;
    }

    // Adapter
    Int32 adjPosition = position - numHeaders;
    Int32 adapterCount = 0;
    if (mAdapter != NULL) {
        mAdapter->GetCount(&adapterCount);
        if (adjPosition < adapterCount) {
            return mAdapter->IsEnabled(adjPosition, enabled);
        }
    }

    // Footer (off-limits positions will throw an ArrayIndexOutOfBoundsException)
    *enabled = mFooterViewInfos[adjPosition - adapterCount]->mIsSelectable;

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    // Header (negative positions will throw an ArrayIndexOutOfBoundsException)
    Int32 numHeaders = mHeaderViewInfos.GetSize();
    if (position < numHeaders) {
        *item = mHeaderViewInfos[position]->mData;
    }

    // Adapter
    Int32 adjPosition = position - numHeaders;
    Int32 adapterCount = 0;
    if (mAdapter != NULL) {
        mAdapter->GetCount(&adapterCount);
        if (adjPosition < adapterCount) {
            return mAdapter->GetItem(adjPosition, item);
        }
    }

    // Footer (off-limits positions will throw an ArrayIndexOutOfBoundsException)
    *item = mFooterViewInfos[adjPosition - adapterCount]->mData;

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId);

    Int32 numHeaders = mHeaderViewInfos.GetSize();
    if (mAdapter != NULL && position >= numHeaders) {
        Int32 adjPosition = position - numHeaders;
        Int32 adapterCount;
        mAdapter->GetCount(&adapterCount);
        if (adjPosition < adapterCount) {
            return mAdapter->GetItemId(adjPosition, itemId);
        }
    }
    *itemId = -1;

    return NOERROR;
}

ECode CHeaderViewListAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds);
    if (mAdapter != NULL) {
        return mAdapter->HasStableIds(hasStableIds);
    }
    *hasStableIds = FALSE;

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    // Header (negative positions will throw an ArrayIndexOutOfBoundsException)
    Int32 numHeaders = mHeaderViewInfos.GetSize();
    if (position < numHeaders) {
        *view = mHeaderViewInfos[position]->mView;
    }

    // Adapter
    Int32 adjPosition = position - numHeaders;
    Int32 adapterCount = 0;
    if (mAdapter != NULL) {
        mAdapter->GetCount(&adapterCount);
        if (adjPosition < adapterCount) {
            return mAdapter->GetView(position, convertView, parent, view);
        }
    }

    // Footer (off-limits positions will throw an ArrayIndexOutOfBoundsException)
    *view = mFooterViewInfos[adjPosition - adapterCount]->mView;

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);

    Int32 numHeaders = mHeaderViewInfos.GetSize();
    if (mAdapter != NULL && position >= numHeaders) {
        Int32 adjPosition = position - numHeaders;
        Int32 adapterCount;
        mAdapter->GetCount(&adapterCount);
        if (adjPosition < adapterCount) {
            return mAdapter->GetItemViewType(adjPosition, viewType);
        }
    }
    *viewType = AdapterView_ITEM_VIEW_TYPE_HEADER_OR_FOOTER;

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    if (mAdapter != NULL) {
        return mAdapter->GetViewTypeCount(count);
    }
    *count = 1;

    return NOERROR;
}

ECode CHeaderViewListAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    if (mAdapter != NULL) {
        mAdapter->RegisterDataSetObserver(observer);
    }

    return NOERROR;
}

ECode CHeaderViewListAdapter::UnregisterDataSetObserver(
   /* [in] */ IDataSetObserver* observer)
{
    if (mAdapter != NULL) {
        mAdapter->UnregisterDataSetObserver(observer);
    }

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    if (mIsFilterable) {
        return ((IFilterable*)mAdapter->Probe(EIID_IFilterable))
            ->GetFilter(filter);
    }
    *filter = NULL;

    return NOERROR;
}

ECode CHeaderViewListAdapter::GetWrappedAdapter(
    /* [out] */ IListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    *adapter = mAdapter;

    return NOERROR;
}
