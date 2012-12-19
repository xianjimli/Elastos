#ifndef __SQLITECURSOR_H__
#define __SQLITECURSOR_H__

#include "ext/frameworkext.h"
#include "database/AbstractWindowedCursor.h"
#include "database/sqlite/SQLiteQuery.h"
#include "database/sqlite/SQLiteDatabase.h"
#include <elastos/Map.h>
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>

/**
 * A Cursor implementation that exposes results from a query on a
 * {@link SQLiteDatabase}.
 *
 * SQLiteCursor is not internally synchronized so code using a SQLiteCursor from multiple
 * threads should perform its own synchronization when using the SQLiteCursor.
 */
class SQLiteCursor : public AbstractWindowedCursor
{
    // /**
    //  * @hide
    //  */
    // final private class QueryThread implements Runnable {
    //     private final int mThreadState;
    //     QueryThread(int version) {
    //         mThreadState = version;
    //     }
    //     private void sendMessage() {
    //         if (mNotificationHandler != null) {
    //             mNotificationHandler.sendEmptyMessage(1);
    //             mPendingData = false;
    //         } else {
    //             mPendingData = true;
    //         }

    //     }
    //     public void run() {
    //          // use cached mWindow, to avoid get null mWindow
    //         CursorWindow cw = mWindow;
    //         Process.setThreadPriority(Process.myTid(), Process.THREAD_PRIORITY_BACKGROUND);
    //         // the cursor's state doesn't change
    //         while (true) {
    //             mLock.lock();
    //             if (mCursorState != mThreadState) {
    //                 mLock.unlock();
    //                 break;
    //             }
    //             try {
    //                 int count = mQuery.fillWindow(cw, mMaxRead, mCount);
    //                 // return -1 means not finished
    //                 if (count != 0) {
    //                     if (count == NO_COUNT){
    //                         mCount += mMaxRead;
    //                         sendMessage();
    //                     } else {
    //                         mCount = count;
    //                         sendMessage();
    //                         break;
    //                     }
    //                 } else {
    //                     break;
    //                 }
    //             } catch (Exception e) {
    //                 // end the tread when the cursor is close
    //                 break;
    //             } finally {
    //                 mLock.unlock();
    //             }
    //         }
    //     }
    // }

    // /**
    //  * @hide
    //  */
    // protected class MainThreadNotificationHandler extends Handler {
    //     public void handleMessage(Message msg) {
    //         notifyDataSetChange();
    //     }
    // }

public:
    /**
     * Execute a query and provide access to its result set through a Cursor
     * interface. For a query such as: {@code SELECT name, birth, phone FROM
     * myTable WHERE ... LIMIT 1,20 ORDER BY...} the column names (name, birth,
     * phone) would be in the projection argument and everything from
     * {@code FROM} onward would be in the params argument. This constructor
     * has package scope.
     *
     * @param db a reference to a Database object that is already constructed
     *     and opened
     * @param editTable the name of the table used for this query
     * @param query the rest of the query terms
     *     cursor is finalized
     */
    SQLiteCursor(
        /*[in]*/ ISQLiteDatabase* db,
        /*[in]*/ ISQLiteCursorDriver* driver,
        /*[in]*/ const String& editTable,
        /*[in]*/ ISQLiteQuery* query);

    virtual ~SQLiteCursor();

    virtual CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid) = 0;

    /**
     *  support for a cursor variant that doesn't always read all results
     *  initialRead is the initial number of items that cursor window reads
     *  if query contains more than this number of items, a thread will be
     *  created and handle the left over items so that caller can show
     *  results as soon as possible
     * @param initialRead initial number of items that cursor read
     * @param maxRead leftover items read at maxRead items per time
     * @hide
     */
    virtual CARAPI SetLoadStyle(
        /* [in] */ Int32 initialRead,
        /* [in] */ Int32 maxRead);

    virtual CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    /**
     * @return the SQLiteDatabase that this cursor is associated with.
     */
    virtual CARAPI_(AutoPtr<ISQLiteDatabase>) GetDatabase();

    //@Override
    CARAPI_(Boolean) OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition);

    //@Override
    CARAPI_(Int32) GetCount();

    // @Override
    CARAPI_(Int32) GetColumnIndex(
        /* [in] */ const String& columnName);

    /**
     * @hide
     * @deprecated
     */
    // @Override
    CARAPI DeleteRow(
        /* [out] */ Boolean* succeeded);

    // @Override
    CARAPI_(ArrayOf<String>*) GetColumnNames();

    /**
     * @hide
     * @deprecated
     */
    // @Override
    CARAPI_(Boolean) SupportsUpdates();

    // /**
    //  * @hide
    //  * @deprecated
    //  */
    // @Override
    // public boolean commitUpdates(Map<? extends Long,
    //         ? extends Map<String, Object>> additionalValues)

    // @Override
    CARAPI Deactivate();

    // @Override
    CARAPI Close();

    // @Override
    CARAPI Requery(
        /* [out] */ Boolean* succeeded);

    // @Override
    CARAPI SetWindow(
        /* [in] */ ICursorWindow* window);

    /**
     * Changes the selection arguments. The new values take effect after a call to requery().
     */
    virtual CARAPI SetSelectionArguments(
        /* [in] */ ArrayOf<String>* selectionArgs);

protected:
    SQLiteCursor();

    CARAPI Init(
        /*[in]*/ ISQLiteDatabase* db,
        /*[in]*/ ISQLiteCursorDriver* driver,
        /*[in]*/ const String& editTable,
        /*[in]*/ ISQLiteQuery* query);

private:
    CARAPI_(void) QueryThreadLock();

    CARAPI_(void) QueryThreadUnlock();

    CARAPI_(void) FillWindow (
        /* [in] */ Int32 startPos);

    CARAPI_(void) DeactivateCommon();

public:
    static const CString TAG;
    static const Int32  NO_COUNT = -1;

protected:
    /**
     * @hide
     */
    // protected MainThreadNotificationHandler mNotificationHandler;

private:
    /** The name of the table to edit */
    String mEditTable;

    /** The names of the columns in the rows */
    ArrayOf<String>* mColumns;

    /** The query object for the cursor */
    AutoPtr<SQLiteQuery> mQuery;

    /** The database the cursor was created from */
    AutoPtr<SQLiteDatabase> mDatabase;

    /** The compiled query this cursor came from */
    AutoPtr<ISQLiteCursorDriver> mDriver;

    /** The number of rows in the cursor */
    Int32 mCount;

    /** A mapping of column names to column indices, to speed up lookups */
    HashMap<String, Int32>* mColumnNameMap;

    /** Used to find out where a cursor was allocated in case it never got released. */
//   Throwable mStackTrace;

    /**
     *  mMaxRead is the max items that each cursor window reads
     *  default to a very high value
     */
    Int32 mMaxRead;
    Int32 mInitialRead;
    Int32 mCursorState;
    ReentrantLock* mLock;
    Boolean mPendingData;
};


#endif //__SQLITECURSOR_H__