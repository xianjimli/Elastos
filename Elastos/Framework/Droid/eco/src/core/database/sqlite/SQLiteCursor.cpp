
#include "database/sqlite/SQLiteCursor.h"
#include "database/sqlite/SQLiteDebug.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

const CString SQLiteCursor::TAG = "Cursor";
const Int32 SQLiteCursor::NO_COUNT;

SQLiteCursor::SQLiteCursor(
    /*[in]*/ ISQLiteDatabase* db,
    /*[in]*/ ISQLiteCursorDriver* driver,
    /*[in]*/ const String& editTable,
    /*[in]*/ ISQLiteQuery* query)
    : AbstractWindowedCursor() // The AbstractCursor constructor needs to do some setup.
    , mEditTable(editTable)
    , mColumns(NULL)
    , mQuery((SQLiteQuery*)query)
    , mDatabase((SQLiteDatabase*)db)
    , mDriver(driver)
    , mCount(NO_COUNT)
    , mColumnNameMap(NULL)
    , mMaxRead(Math::INT32_MAX_VALUE)
    , mInitialRead(Math::INT32_MAX_VALUE)
    , mCursorState(0)
    , mLock(NULL)
    , mPendingData(FALSE)
{
    // mStackTrace = new DatabaseObjectNotClosedException().fillInStackTrace();

    // try {
    mDatabase->Lock();

    // Setup the list of columns
    Int32 columnCount = mQuery->ColumnCountLocked();
    mColumns = ArrayOf<String>::Alloc(columnCount);

    // Read in all column names
    for (Int32 i = 0; i < columnCount; i++) {
        String columnName = mQuery->ColumnNameLocked(i);
        (*mColumns)[i] = columnName;
        // if (Config.LOGV) {
        //     Log.v("DatabaseWindow", "mColumns[" + i + "] is "
        //             + mColumns[i]);
        // }

        // Make note of the row ID column index for quick access to it
        if (columnName.Equals("_id")) {
            mRowIdColumnIndex = i;
        }
    }
    // } finally {
    mDatabase->Unlock();
    // }
}

SQLiteCursor::~SQLiteCursor()
{
    // try {
    // if the cursor hasn't been closed yet, close it first
    if (mWindow != NULL) {
        // if (StrictMode.vmSqliteObjectLeaksEnabled()) {
        //     int len = mQuery.mSql.length();
        //     StrictMode.onSqliteObjectLeaked(
        //         "Finalizing a Cursor that has not been deactivated or closed. " +
        //         "database = " + mDatabase.getPath() + ", table = " + mEditTable +
        //         ", query = " + mQuery.mSql.substring(0, (len > 100) ? 100 : len),
        //         mStackTrace);
        // }
        Close();
        SQLiteDebug::NotifyActiveCursorFinalized();
    }
    else {
        // if (Config.LOGV) {
        //     Log.v(TAG, "Finalizing cursor on database = " + mDatabase.getPath() +
        //             ", table = " + mEditTable + ", query = " + mQuery.mSql);
        // }
    }
    // } finally {
    //     super.finalize();
    // }

    if (mColumns != NULL) {
        for (Int32 i = 0; i < mColumns->GetLength(); ++i) {
            (*mColumns)[i] = NULL;
        }
        ArrayOf<String>::Free(mColumns);
    }

    if (mColumnNameMap != NULL) {
        mColumnNameMap->Clear();
        delete mColumnNameMap;
    }

    if (mLock != NULL) {
        delete mLock;
    }
}

ECode SQLiteCursor::SetLoadStyle(
    /* [in] */ Int32 initialRead,
    /* [in] */ Int32 maxRead)
{
    mMaxRead = maxRead;
    mInitialRead = initialRead;
    mLock = new ReentrantLock();
    return NOERROR;
}

void SQLiteCursor::QueryThreadLock()
{
    if (mLock != NULL) {
        mLock->Lock();
    }
}

void SQLiteCursor::QueryThreadUnlock()
{
    if (mLock != NULL) {
        mLock->Unlock();
    }
}

ECode SQLiteCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    ECode ec;

    FAIL_RETURN(AbstractWindowedCursor::RegisterDataSetObserver(observer));
    // if ((Math::INT32_MAX_VALUE != mMaxRead || Math::INT32_MAX_VALUE != mInitialRead) &&
    //         mNotificationHandler == NULL) {
    //     QueryThreadLock();
    //     // try {
    //     mNotificationHandler = new MainThreadNotificationHandler();
    //     if (mPendingData) {
    //         NotifyDataSetChange();
    //         mPendingData = FALSE;
    //     }
    //     // } finally {
    //     QueryThreadUnlock();
    //     // }
    // }
    return ec;
}

AutoPtr<ISQLiteDatabase> SQLiteCursor::GetDatabase()
{
    return mDatabase;
}

//@Override
Boolean SQLiteCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition)
{
    // Make sure the row at newPosition is present in the window
    if (mWindow == NULL) {
        FillWindow(newPosition);
    }
    else {
        Int32 startPos, rowNum;
        mWindow->GetStartPosition(&startPos);
        mWindow->GetNumRows(&rowNum);
        if (newPosition < startPos || newPosition >= (startPos + rowNum)) {
            FillWindow(newPosition);
        }
    }

    return TRUE;
}

//@Override
Int32 SQLiteCursor::GetCount()
{
    if (mCount == NO_COUNT) {
        FillWindow(0);
    }
    return mCount;
}

void SQLiteCursor::FillWindow (
    /* [in] */ Int32 startPos)
{
    if (mWindow == NULL) {
        // If there isn't a window set already it will only be accessed locally
        // mWindow = new CursorWindow(true /* the window is local only */);
    }
    else {
        mCursorState++;
        QueryThreadLock();
        // try {
        mWindow->Clear();
        // } finally {
        QueryThreadUnlock();
        // }
    }
    mWindow->SetStartPosition(startPos);
    mQuery->FillWindow(mWindow, mInitialRead, 0, &mCount);
    // return -1 means not finished
    if (mCount == NO_COUNT){
        mCount = startPos + mInitialRead;
        // Thread t = new Thread(new QueryThread(mCursorState), "query thread");
        // t.start();
    }
}

//@Override
Int32 SQLiteCursor::GetColumnIndex(
    /* [in] */ const String& columnName)
{
    String _columnName = columnName;

    // Create mColumnNameMap on demand
    if (mColumnNameMap == NULL) {
        ArrayOf<String>* columns = mColumns;
        Int32 columnCount = columns->GetLength();
        HashMap<String, Int32>* map = new HashMap<String, Int32>(columnCount);
        for (Int32 i = 0; i < columnCount; i++) {
            (*map)[(*columns)[i]] = i;
        }
        mColumnNameMap = map;
    }

    // Hack according to bug 903852
    const Int32 periodIndex = _columnName.LastIndexOf('.');
    if (periodIndex != -1) {
        // Exception e = new Exception();
        // Log.e(TAG, "requesting column name with table name -- " + columnName, e);
        _columnName = _columnName.Substring(periodIndex + 1);
    }

    HashMap<String, Int32>::Iterator it = mColumnNameMap->Find(_columnName);
    if (it != mColumnNameMap->End()) {
        return it->mSecond;
    }
    else {
        return -1;
    }
}

/**
 * @hide
 * @deprecated
 */
//@Override
ECode SQLiteCursor::DeleteRow(
    /* [out] */ Boolean* succeeded)
{
    CheckPosition();

    // Only allow deletes if there is an ID column, and the ID has been read from it
    if (mRowIdColumnIndex == -1/* || mCurrentRowID == null*/) {
        // Log.e(TAG,
        //         "Could not delete row because either the row ID column is not available or it" +
        //         "has not been read.");
        *succeeded = FALSE;
        return NOERROR;
    }

    Boolean success;

    /*
     * Ensure we don't change the state of the database when another
     * thread is holding the database lock. requery() and moveTo() are also
     * synchronized here to make sure they get the state of the database
     * immediately following the DELETE.
     */
    mDatabase->Lock();
    // try {
        // try {
            // mDatabase.delete(mEditTable, mColumns[mRowIdColumnIndex] + "=?",
            //         new String[] {mCurrentRowID.toString()});
            success = TRUE;
        // } catch (SQLException e) {
        //     success = false;
        // }

        Int32 pos = mPos;
        Requery(succeeded);

        /*
         * Ensure proper cursor state. Note that mCurrentRowID changes
         * in this call.
         */
        MoveToPosition(pos);
    // } finally {
        mDatabase->Unlock();
    // }

    if (success) {
        OnChange(TRUE);
        *succeeded = TRUE;
        return NOERROR;
    }
    else {
        *succeeded = FALSE;
        return NOERROR;
    }
}

//@Override
ArrayOf<String>* SQLiteCursor::GetColumnNames()
{
    return mColumns;
}

/**
 * @hide
 * @deprecated
 */
//@Override
Boolean SQLiteCursor::SupportsUpdates()
{
    return /*AbstractWindowedCursor::SupportsUpdates() &&*/ !mEditTable.IsNullOrEmpty();
}

/**
 * @hide
 * @deprecated
 */
// @Override
// public boolean commitUpdates(Map<? extends Long,
//         ? extends Map<String, Object>> additionalValues) {
//     if (!supportsUpdates()) {
//         Log.e(TAG, "commitUpdates not supported on this cursor, did you "
//                 + "include the _id column?");
//         return false;
//     }

//     /*
//      * Prevent other threads from changing the updated rows while they're
//      * being processed here.
//      */
//     synchronized (mUpdatedRows) {
//         if (additionalValues != null) {
//             mUpdatedRows.putAll(additionalValues);
//         }

//         if (mUpdatedRows.size() == 0) {
//             return true;
//         }

//         /*
//          * Prevent other threads from changing the database state while
//          * we process the updated rows, and prevents us from changing the
//          * database behind the back of another thread.
//          */
//         mDatabase.beginTransaction();
//         try {
//             StringBuilder sql = new StringBuilder(128);

//             // For each row that has been updated
//             for (Map.Entry<Long, Map<String, Object>> rowEntry :
//                     mUpdatedRows.entrySet()) {
//                 Map<String, Object> values = rowEntry.getValue();
//                 Long rowIdObj = rowEntry.getKey();

//                 if (rowIdObj == null || values == null) {
//                     throw new IllegalStateException("null rowId or values found! rowId = "
//                             + rowIdObj + ", values = " + values);
//                 }

//                 if (values.size() == 0) {
//                     continue;
//                 }

//                 long rowId = rowIdObj.longValue();

//                 Iterator<Map.Entry<String, Object>> valuesIter =
//                         values.entrySet().iterator();

//                 sql.setLength(0);
//                 sql.append("UPDATE " + mEditTable + " SET ");

//                 // For each column value that has been updated
//                 Object[] bindings = new Object[values.size()];
//                 int i = 0;
//                 while (valuesIter.hasNext()) {
//                     Map.Entry<String, Object> entry = valuesIter.next();
//                     sql.append(entry.getKey());
//                     sql.append("=?");
//                     bindings[i] = entry.getValue();
//                     if (valuesIter.hasNext()) {
//                         sql.append(", ");
//                     }
//                     i++;
//                 }

//                 sql.append(" WHERE " + mColumns[mRowIdColumnIndex]
//                         + '=' + rowId);
//                 sql.append(';');
//                 mDatabase.execSQL(sql.toString(), bindings);
//                 mDatabase.rowUpdated(mEditTable, rowId);
//             }
//             mDatabase.setTransactionSuccessful();
//         } finally {
//             mDatabase.endTransaction();
//         }

//         mUpdatedRows.clear();
//     }

//     // Let any change observers know about the update
//     onChange(true);

//     return true;
// }

void SQLiteCursor::DeactivateCommon()
{
    // if (Config.LOGV) Log.v(TAG, "<<< Releasing cursor " + this);
    mCursorState = 0;
    if (mWindow != NULL) {
        mWindow->Close();
        mWindow = NULL;
    }
    // if (Config.LOGV) Log.v("DatabaseWindow", "closing window in release()");
}

//@Override
ECode SQLiteCursor::Deactivate()
{
    FAIL_RETURN(AbstractWindowedCursor::Deactivate());
    DeactivateCommon();
    return mDriver->CursorDeactivated();
}

//@Override
ECode SQLiteCursor::Close()
{
    FAIL_RETURN(AbstractWindowedCursor::Close());
    DeactivateCommon();
    FAIL_RETURN(mQuery->Close());
    return mDriver->CursorClosed();
}

//@Override
ECode SQLiteCursor::Requery(
    /* [out] */ Boolean* succeeded)
{
    if (IsClosed()) {
        *succeeded = FALSE;
        return NOERROR;
    }
    Int64 timeStart = 0;
    // if (Config.LOGV) {
    //     timeStart = System.currentTimeMillis();
    // }
    /*
     * Synchronize on the database lock to ensure that mCount matches the
     * results of mQuery.requery().
     */
    mDatabase->Lock();
    // try {
        if (mWindow != NULL) {
            mWindow->Clear();
        }
        mPos = -1;
        // This one will recreate the temp table, and get its count
        mDriver->CursorRequeried((ICursor*)this->Probe(EIID_ICursor));
        mCount = NO_COUNT;
        mCursorState++;
        QueryThreadLock();
        // try {
        mQuery->Requery();
        // } finally {
        QueryThreadUnlock();
        // }
    // } finally {
    mDatabase->Unlock();
    // }

    // if (Config.LOGV) {
    //     Log.v("DatabaseWindow", "closing window in requery()");
    //     Log.v(TAG, "--- Requery()ed cursor " + this + ": " + mQuery);
    // }
    ECode ec = AbstractWindowedCursor::Requery(succeeded);
    // if (Config.LOGV) {
    //     long timeEnd = System.currentTimeMillis();
    //     Log.v(TAG, "requery (" + (timeEnd - timeStart) + " ms): " + mDriver.toString());
    // }
    return ec;
}

//@Override
ECode SQLiteCursor::SetWindow(
    /* [in] */ ICursorWindow* window)
{
    ECode ec = NOERROR;

    if (mWindow != NULL) {
        mCursorState++;
        QueryThreadLock();
        // try {
        ec = mWindow->Close();
        // } finally {
        QueryThreadUnlock();
        // }
        mCount = NO_COUNT;
    }
    mWindow = window;

    return ec;
}

ECode SQLiteCursor::SetSelectionArguments(
    /* [in] */ ArrayOf<String>* selectionArgs)
{
    return mDriver->SetBindArguments(selectionArgs);
}

