
#include "widget/CursorAdapter.h"

CursorAdapter::CursorAdapter()
{

}
CursorAdapter::CursorAdapter(
    /* [in] */ IContext* context, 
    /* [in] */ ICursor* c)
{
    Init(context, c, TRUE);
}

CursorAdapter::CursorAdapter(
    /* [in] */ IContext* context, 
    /* [in] */ ICursor* c, 
    /* [in] */ Boolean autoRequery)
{
    Init(context, c, autoRequery);
}

void CursorAdapter::Init(
    /* [in] */ IContext* context, 
    /* [in] */ ICursor* c, 
    /* [in] */ Boolean autoRequery)
{
    Boolean cursorPresent = c != NULL;
    mAutoRequery = autoRequery;
    mCursor = c;
    mDataValid = cursorPresent;
    mContext = context;
    //mRowIDColumn = cursorPresent ? c->GetColumnIndexOrThrow("_id") : -1;
    //mChangeObserver = new ChangeObserver();
    if (cursorPresent) {
        //c->RegisterContentObserver(mChangeObserver);
        c->RegisterDataSetObserver(mDataSetObserver);
    }
}

AutoPtr<ICursor> CursorAdapter::GetCursor()
{
    return mCursor;
}

Int32 CursorAdapter::GetCount()
{
    if (mDataValid && mCursor != NULL) {
        Int32 count;
        mCursor->GetCount(&count);
        return count;
    } else {
        return 0;
    }
}

AutoPtr<IInterface> CursorAdapter::GetItem(
    /* [in] */ Int32 position)
{
    if (mDataValid && mCursor != NULL) {
        Boolean res;
        mCursor->MoveToPosition(position, &res);
        return mCursor;
    } else {
        return NULL;
    }
}

Int64 CursorAdapter::GetItemId(
    /* [in] */ Int32 position) 
{
    if (mDataValid && mCursor != NULL) {
        Boolean r;
        if (mCursor->MoveToPosition(position, &r)) {
            Int64 res;
            mCursor->GetInt64(mRowIDColumn, &res);
            return res;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

Boolean CursorAdapter::HasStableIds()
{
    return TRUE;
}

AutoPtr<IView> CursorAdapter::GetView(
    /* [in] */ Int32 position, 
    /* [in] */ IView* convertView, 
    /* [in] */ IViewGroup* parent)
{
    if (!mDataValid) {
        //throw new IllegalStateException("this should only be called when the cursor is valid");
    }
    //if (!mCursor.moveToPosition(position)) {
        //throw new IllegalStateException("couldn't move cursor to position " + position);
    //}
    AutoPtr<IView> v;
    if (convertView == NULL) {
        v = NewView(mContext, mCursor, parent);
    } else {
        v = convertView;
    }
    BindView(v, mContext, mCursor);
    return v;
}

AutoPtr<IView> CursorAdapter::GetDropDownView(
    /* [in] */ Int32 position, 
    /* [in] */ IView* convertView, 
    /* [in] */ IViewGroup* parent)
{
    if (mDataValid) {
        Boolean res;
        mCursor->MoveToPosition(position, &res);
        AutoPtr<IView> v;
        if (convertView == NULL) {
            v = NewDropDownView(mContext, mCursor, parent);
        } else {
            v = convertView;
        }
        BindView(v, mContext, mCursor);
        return v;
    } else {
        return NULL;
    }
}

AutoPtr<IView> CursorAdapter::NewDropDownView(
    /* [in] */ IContext* context, 
    /* [in] */ ICursor* cursor, 
    /* [in] */ IViewGroup* parent) 
{
    return NewView(context, cursor, parent);
}

ECode CursorAdapter::ChangeCursor(
    /* [in] */ ICursor* cursor) 
{
    if (cursor == mCursor) {
        return NOERROR;
    }
    if (mCursor != NULL) {
        //mCursor->UnregisterContentObserver(mChangeObserver);
        mCursor->UnregisterDataSetObserver(mDataSetObserver);
        mCursor->Close();
    }
    mCursor = cursor;
    if (cursor != NULL) {
        //cursor->RegisterContentObserver(mChangeObserver);
        cursor->RegisterDataSetObserver(mDataSetObserver);
        //cursor->GetColumnIndexOrThrow("_id", &mRowIDColumn);
        mDataValid = TRUE;
        // notify the observers about the new cursor
        NotifyDataSetChanged();
    } else {
        mRowIDColumn = -1;
        mDataValid = FALSE;
        // notify the observers about the lack of a data set
        NotifyDataSetInvalidated();
    }

    return NOERROR;
}

AutoPtr<ICharSequence> CursorAdapter::ConvertToString(
    /* [in] */ ICursor* cursor) 
{
    //return cursor == NULL ? "" : cursor->ToString();

    return NULL;
}

AutoPtr<ICursor> CursorAdapter::RunQueryOnBackgroundThread(
    /* [in] */ ICharSequence* constraint) 
{
    if (mFilterQueryProvider != NULL) {
        //return mFilterQueryProvider->RunQuery(constraint);
    }

    return mCursor;
}

AutoPtr<IFilter> CursorAdapter::GetFilter()
{
    /*if (mCursorFilter == NULL) {
        mCursorFilter = new CursorFilter(this);
    }
    return mCursorFilter;*/

    return NULL;
}

AutoPtr<IFilterQueryProvider> CursorAdapter::GetFilterQueryProvider()
{
    return mFilterQueryProvider;
}

ECode CursorAdapter::SetFilterQueryProvider(
    /* [in] */ IFilterQueryProvider* filterQueryProvider)
{
    mFilterQueryProvider = filterQueryProvider;

    return NOERROR;
}

void CursorAdapter::OnContentChanged() 
{
    //if (mAutoRequery && mCursor != NULL && !mCursor.isClosed()) {
    //    //if (Config.LOGV) Log.v("Cursor", "Auto requerying " + mCursor + " due to update");
    //    mCursor->Requery(&mDataValid);
    //}
}

PInterface CursorAdapter::ChangeObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)this;
    }
    else if (riid == EIID_ITextWatcher) {
        return (ITextWatcher*)this;
    }

    return NULL;
}

UInt32 CursorAdapter::ChangeObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CursorAdapter::ChangeObserver::Release()
{
    return ElRefBase::Release();
}

ECode CursorAdapter::ChangeObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

CursorAdapter::ChangeObserver::ChangeObserver()
{
    //super(new Handler());
}

Boolean CursorAdapter::ChangeObserver::DeliverSelfNotifications() 
{
    return TRUE;
}

ECode CursorAdapter::ChangeObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    //OnContentChanged();

    return NOERROR;
}

PInterface CursorAdapter::MyDataSetObserver::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 CursorAdapter::MyDataSetObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CursorAdapter::MyDataSetObserver::Release()
{
    return ElRefBase::Release();
}

ECode CursorAdapter::MyDataSetObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorAdapter::MyDataSetObserver::OnChanged()
{
    //mDataValid = TRUE;
    //NotifyDataSetChanged();
}

ECode CursorAdapter::MyDataSetObserver::OnInvalidated()
{
    //mDataValid = FALSE;
    //NotifyDataSetInvalidated();
}