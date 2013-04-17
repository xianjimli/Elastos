
#include "app/CListActivity.h"


ListActivity::ListActivity()
    : mFinishedStart(FALSE)
{}

void ListActivity::OnListItemClick(
    /* [in] */ IListView* l,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{}

ECode ListActivity::OnRestoreInstanceState(
    /* [in] */ IBundle* state)
{
    EnsureList();
    Activity::OnRestoreInstanceState(state);
    return NOERROR;
}

ECode ListActivity::OnDestroy()
{
//    mHandler.removeCallbacks(mRequestFocus);
    Activity::OnDestroy();
    return NOERROR;
}

ECode ListActivity::OnContentChanged()
{
    Activity::OnContentChanged();
    AutoPtr<IView> emptyView = FindViewById(0x01020004/*com.android.internal.R.id.empty*/);
    AutoPtr<IView> fView = FindViewById(0x0102000a/*com.android.internal.R.id.list*/);
    mList = IListView::Probe(fView.Get());
    if (mList == NULL) {
//        throw new RuntimeException(
//                "Your content must have a ListView whose id attribute is " +
//                "'android.R.id.list'");
        return E_RUNTIME_EXCEPTION;
    }
    if (emptyView != NULL) {
        mList->SetEmptyView(emptyView);
    }
    mList->SetOnItemClickListener(mOnClickListener);
    if (mFinishedStart) {
        SetListAdapter(mAdapter);
    }
//    mHandler.post(mRequestFocus);
    mFinishedStart = TRUE;
    return NOERROR;
}

ECode ListActivity::SetListAdapter(
    /* [in] */ IListAdapter* adapter)
{
    Mutex::Autolock lock(mSync);
    EnsureList();
    mAdapter = adapter;
    mList->SetAdapter(adapter);
    return NOERROR;
}

ECode ListActivity::SetSelection(
    /* [in] */ Int32 position)
{
    return mList->SetSelection(position);
}

ECode ListActivity::GetSelectedItemPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return mList->GetSelectedItemPosition(position);
}

ECode ListActivity::GetSelectedItemId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    return mList->GetSelectedItemId(id);
}

ECode ListActivity::GetListView(
    /* [out] */ IListView** listview)
{
    VALIDATE_NOT_NULL(listview);
    EnsureList();
    *listview = mList;
    return NOERROR;
}

ECode ListActivity::GetListAdapter(
    /* [out] */ IListAdapter** listadapter)
{
    VALIDATE_NOT_NULL(listadapter);
    *listadapter = mAdapter;
    return NOERROR;
}

void ListActivity::EnsureList()
{
    if (mList != NULL) {
        return;
    }
    SetContentView(0/*com.android.internal.R.layout.list_content*/);
}
