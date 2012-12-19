
#include "database/BulkCursorToCursorAdaptor.h"
#include "database/CCursorWindow.h"


const CString BulkCursorToCursorAdaptor::TAG = "BulkCursor";

BulkCursorToCursorAdaptor::BulkCursorToCursorAdaptor()
    : mColumns(NULL)
{}

BulkCursorToCursorAdaptor::~BulkCursorToCursorAdaptor()
{
    if (mColumns != NULL) {
        for (Int32 i = 0; i < mColumns->GetLength(); ++i) {
            (*mColumns)[i] = NULL;
        }
        ArrayOf<String>::Free(mColumns);
    }
}

ECode BulkCursorToCursorAdaptor::Set(
    /* [in] */ IBulkCursor* bulkCursor)
{
    mBulkCursor = bulkCursor;

//    try {
    mBulkCursor->GetCount(&mCount);
    mBulkCursor->GetWantsAllOnMoveCalls(&mWantsAllOnMoveCalls);

    // Search for the rowID column index and set it for our parent
    mBulkCursor->GetColumnNames(&mColumns);
    mRowIdColumnIndex = FindRowIdColumnIndex(mColumns);
//    } catch (RemoteException ex) {
//        Log.e(TAG, "Setup failed because the remote process is dead");
//    }
    return NOERROR;
}

ECode BulkCursorToCursorAdaptor::Set(
    /* [in] */ IBulkCursor* bulkCursor,
    /* [in] */ Int32 count,
    /* [in] */ Int32 idIndex)
{
    mBulkCursor = bulkCursor;
    mColumns = NULL; // lazily retrieved
    mCount = count;
    mRowIdColumnIndex = idIndex;
    return NOERROR;
}

Int32 BulkCursorToCursorAdaptor::FindRowIdColumnIndex(
    /* [in] */ ArrayOf<String>* columnNames)
{
    assert(columnNames != NULL);
    Int32 length = columnNames->GetLength();
    for (Int32 i = 0; i < length; i++) {
        if((*columnNames)[i].Equals("_id") ) {
            return i;
        }
    }
    return -1;
}

AutoPtr<IContentObserver> BulkCursorToCursorAdaptor::GetObserver()
{
    Mutex::Autolock lock(mLock);

    // if (mObserverBridge == NULL) {
    //     mObserverBridge = new SelfContentObserver(this);
    // }
    // return mObserverBridge.getContentObserver();
    return NULL;
}

Int32 BulkCursorToCursorAdaptor::GetCount()
{
    return mCount;
}

Boolean BulkCursorToCursorAdaptor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition)
{
//    try {
    // Make sure we have the proper window
    if (mWindow != NULL) {
        Int32 pos, num;
        mWindow->GetStartPosition(&pos);
        mWindow->GetNumRows(&num);
        if (newPosition < pos || newPosition >= (pos + num) ) {
            mBulkCursor->GetWindow(newPosition, (ICursorWindow**)&mWindow);
        }
        else if (mWantsAllOnMoveCalls) {
            mBulkCursor->OnMove(newPosition);
        }
    }
    else {
        mBulkCursor->GetWindow(newPosition, (ICursorWindow**)&mWindow);
    }
//    } catch (RemoteException ex) {
//    // We tried to get a window and failed
//    Log.e(TAG, "Unable to get window because the remote process is dead");
//    return false;
//}

    if (mWindow == NULL) {
        return FALSE;
    }

    return TRUE;
}

ECode BulkCursorToCursorAdaptor::Deactivate()
{
    // This will call onInvalidated(), so make sure to do it before calling release,
    // which is what actually makes the data set invalid.
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
    /* [out] */ Boolean* succeeded)
{
//    try {
    Int32 oldCount = mCount;
    //TODO get the window from a pool somewhere to avoid creating the memory dealer
    AutoPtr<ICursorWindow> window;
    CCursorWindow::New(FALSE, (ICursorWindow**)&window);/* the window will be accessed across processes */
    mBulkCursor->Requery(GetObserver(), window, &mCount);
    if (mCount != -1) {
        mPos = -1;
        mWindow = NULL;

        // super.requery() will call onChanged. Do it here instead of relying on the
        // observer from the far side so that observers can see a correct value for mCount
        // when responding to onChanged.
        AbstractWindowedCursor::Requery(succeeded);
        *succeeded = TRUE;
        return NOERROR;
    }
    else {
        Deactivate();
        *succeeded = FALSE;
        return NOERROR;
    }
//    } catch (Exception ex) {
//        Log.e(TAG, "Unable to requery because the remote process exception " + ex.getMessage());
//        deactivate();
//        return false;
//    }
}

ECode BulkCursorToCursorAdaptor::DeleteRow(
    /* [out] */ Boolean* succeeded)
{
    // try {
    Boolean result;
    mBulkCursor->DeleteRow(mPos, &result);
    if (result != FALSE) {
        // The window contains the old value, discard it
        mWindow = NULL;

        // Fix up the position
        mBulkCursor->GetCount(&mCount);
        if (mPos < mCount) {
            Int32 oldPos = mPos;
            mPos = -1;
            MoveToPosition(oldPos);
        }
        else {
            mPos = mCount;
        }

        // Send the change notification
        OnChange(TRUE);
    }
    *succeeded = result;
    return NOERROR;
    // } catch (RemoteException ex) {
    //     Log.e(TAG, "Unable to delete row because the remote process is dead");
    //     return false;
    // }
}

ECode BulkCursorToCursorAdaptor::GetColumnNames(
    /* [out] */ ArrayOf<String>** names)
{
    if (mColumns == NULL) {
//        try {
        mBulkCursor->GetColumnNames(&mColumns);
//        } catch (RemoteException ex) {
//            Log.e(TAG, "Unable to fetch column names because the remote process is dead");
//            return null;
//        }
    }
    *names = ArrayOf<String>::Alloc(mColumns->GetLength());
    for (Int32 i = 0; i < mColumns->GetLength(); ++i) {
        (**names)[i] = (*mColumns)[i];
    }
    return NOERROR;
}

// public boolean commitUpdates(Map<? extends Long,
//         ? extends Map<String,Object>> additionalValues) {
//     if (!supportsUpdates()) {
//         Log.e(TAG, "commitUpdates not supported on this cursor, did you include the _id column?");
//         return false;
//     }

//     synchronized(mUpdatedRows) {
//         if (additionalValues != null) {
//             mUpdatedRows.putAll(additionalValues);
//         }

//         if (mUpdatedRows.size() <= 0) {
//             return false;
//         }

//         try {
//             boolean result = mBulkCursor.updateRows(mUpdatedRows);

//             if (result == true) {
//                 mUpdatedRows.clear();

//                 // Send the change notification
//                 onChange(true);
//             }
//             return result;
//         } catch (RemoteException ex) {
//             Log.e(TAG, "Unable to commit updates because the remote process is dead");
//             return false;
//         }
//     }
// }

ECode BulkCursorToCursorAdaptor::GetExtras(
    /* [out] */ IBundle** extras)
{
//    try {
    return mBulkCursor->GetExtras(extras);
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
    return mBulkCursor->Respond(extras, v);
//    } catch (RemoteException e) {
        // the system kills processes that are using remote cursors when the provider process
        // is killed, but this can still happen if this is being called from the system process,
        // so, better to log and return an empty bundle.
//        Log.w(TAG, "respond() threw RemoteException, returning an empty bundle.", e);
//        return Bundle.EMPTY;
//    }
}
