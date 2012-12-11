#include "database/BulkCursorToCursorAdaptor.h"
#include "database/CCursorWindow.h"
const String BulkCursorToCursorAdaptor::TAG = String("BulkCursor");

ECode BulkCursorToCursorAdaptor::Set(
            /* [in] */ IBulkCursor* bulkCursor)
{
    mBulkCursor = bulkCursor;
//    try {
    Int32 cnt;
    mBulkCursor->Count(&cnt);
    mCount = cnt;

    Boolean rst;
    mBulkCursor->GetWantsAllOnMoveCalls(&rst);
    mWantsAllOnMoveCalls = rst;

    ArrayOf<String>* names;
    mBulkCursor->GetColumnNames(&names);
    mColumns = names;

    Int32 index;
    FindRowIdColumnIndex(names, &index);
    mRowIdColumnIndex = index;
//    } catch (RemoteException ex) {
//        Log.e(TAG, "Setup failed because the remote process is dead");
//    }
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::SetEx(
            /* [in] */ IBulkCursor* bulkCursor,
            /* [in] */ Int32 count,
            /* [in] */ Int32 idIndex)
{
    mBulkCursor = bulkCursor;
    mColumns = NULL;
    mCount = count;
    mRowIdColumnIndex = idIndex;
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::FindRowIdColumnIndex(
            /* [in] */ ArrayOf<String>* columnNames,
            /* [out] */ Int32* index)
{
    assert(index != NULL);
    Int32 length = columnNames->GetLength();
    for (Int32 i = 0; i < length; i++) {
        if((*columnNames)[i].Equals("_id") ) {
            *index = i;
            return NOERROR;
        }
    }
    *index = -1;
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::GetObserver(
            /* [out] */ IContentObserver** ico)
{
//        if (mObserverBridge == null) {
//            mObserverBridge = new SelfContentObserver(this);
//        }
//        return mObserverBridge.getContentObserver();
    return E_NOT_IMPLEMENTED;
}

ECode BulkCursorToCursorAdaptor::GetCount(
            /* [out] */ Int32* cnt)
{
    assert(cnt != NULL);
    *cnt = mCount;
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::OnMove(
            /* [in] */ Int32 oldPosition,
            /* [in] */ Int32 newPosition,
            /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
//    try {
    if (mWindow != NULL) {
        Int32 pos, num;
        mWindow->GetStartPosition(&pos);
        mWindow->GetNumRows(&num);
        if (newPosition < pos ||
                newPosition >= (pos + num) ) {
                mBulkCursor->GetWindow(newPosition, (ICursorWindow**)&mWindow);
        } else if (mWantsAllOnMoveCalls) {
            mBulkCursor->OnMove(newPosition);
        }
    } else {
        mBulkCursor->GetWindow(newPosition, (ICursorWindow**)&mWindow);
    }
//    } catch (RemoteException ex) {
//    // We tried to get a window and failed
//    Log.e(TAG, "Unable to get window because the remote process is dead");
//    return false;
//}
    if (mWindow == NULL) {
        *rst = FALSE;
        return NOERROR;
    }
    *rst = TRUE;
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::Deactivate()
{
    AbstractWindowedCursor::Deactivate();

//    try {
    mBulkCursor->Deactivate();
//    } catch (RemoteException ex) {
//        Log.w(TAG, "Remote process exception when deactivating");
//    }
    mWindow = NULL;
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::Close()
{
    AbstractWindowedCursor::Close();
//    try {
    mBulkCursor->Close();
//    } catch (RemoteException ex) {
//        Log.w(TAG, "Remote process exception when closing");
//    }
    mWindow = NULL;
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::Requery(
            /* [out] */ Boolean* rst)
{
//    try {
        //Int32 oldCount = mCount;
        //TODO get the window from a pool somewhere to avoid creating the memory dealer
        AutoPtr<IContentObserver> observer;
        GetObserver((IContentObserver**)&observer);

        AutoPtr<ICursorWindow> cw;
        CCursorWindow::New(FALSE, (ICursorWindow**)&cw);

        mBulkCursor->Requery(observer, (ICursorWindow*)cw, &mCount);

        if (mCount != -1) {
            mPos = -1;
            mWindow = NULL;

            // super.requery() will call onChanged. Do it here instead of relying on the
            // observer from the far side so that observers can see a correct value for mCount
            // when responding to onChanged.
            Boolean result;
            AbstractWindowedCursor::Requery(&result);
            *rst = TRUE;
            return NOERROR;
        } else {
            Deactivate();
            *rst = FALSE;
            return NOERROR;
        }
//    } catch (Exception ex) {
//        Log.e(TAG, "Unable to requery because the remote process exception " + ex.getMessage());
//        deactivate();
//        return false;
//    }
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::GetColumnNames(
            /* [out, callee] */ ArrayOf<String>** names)
{
    if (mColumns == NULL) {
//        try {
            mBulkCursor->GetColumnNames(&mColumns);
//        } catch (RemoteException ex) {
//            Log.e(TAG, "Unable to fetch column names because the remote process is dead");
//            return null;
//        }
    }
    *names = mColumns;
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::GetExtras(
            /* [out] */ IBundle** extras)
{
//    try {
        mBulkCursor->GetExtras(extras);
        return NOERROR;
//    } catch (RemoteException e) {
        // This should never happen because the system kills processes that are using remote
        // cursors when the provider process is killed.
//        throw new RuntimeException(e);
//    }
}

ECode BulkCursorToCursorAdaptor::Respond(
            /* [in] */ IBundle* extras,
            /* [out] */ IBundle** v)
{
//    try {
        mBulkCursor->Respond(extras, v);
        return NOERROR;
//    } catch (RemoteException e) {
        // the system kills processes that are using remote cursors when the provider process
        // is killed, but this can still happen if this is being called from the system process,
        // so, better to log and return an empty bundle.
//        Log.w(TAG, "respond() threw RemoteException, returning an empty bundle.", e);
//        return Bundle.EMPTY;
//    }
}