
#include "app/CListActivity.h"


ECode CListActivity::OnContentChanged()
{
    return ListActivity::OnContentChanged();
}

ECode CListActivity::SetListAdapter(
    /* [in] */ IListAdapter* adapter)
{
    return ListActivity::SetListAdapter(adapter);
}

ECode CListActivity::SetSelection(
    /* [in] */ Int32 position)
{
    return ListActivity::SetSelection(position);
}

ECode CListActivity::GetSelectedItemPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return ListActivity::GetSelectedItemPosition(position);
}

ECode CListActivity::GetSelectedItemId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    return ListActivity::GetSelectedItemId(id);
}

ECode CListActivity::GetListView(
    /* [out] */ IListView** listview)
{
    VALIDATE_NOT_NULL(listview);
    return ListActivity::GetListView(listview);
}

ECode CListActivity::GetListAdapter(
    /* [out] */ IListAdapter** listadapter)
{
    VALIDATE_NOT_NULL(listadapter);
    return ListActivity::GetListAdapter(listadapter);
}

