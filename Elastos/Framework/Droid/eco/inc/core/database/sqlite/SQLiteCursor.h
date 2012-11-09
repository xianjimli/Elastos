#ifndef __SQLITECURSOR_H__
#define __SQLITECURSOR_H__

#include "ext/frameworkext.h"
#include "database/AbstractWindowedCursor.h"
#include <elastos/Map.h>
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>


class SQLiteCursor : public AbstractWindowedCursor
{

    SQLiteCursor(
        /*[in]*/ ISQLiteDatabase* db,
        /*[in]*/ ISQLiteCursorDriver* driver,
        /*[in]*/ String editTable, 
        /*[in]*/ ISQLiteQuery* query);

    ~SQLiteCursor();



    static const String TAG;

    static const Int32  NO_COUNT;

private:
    /** The name of the table to edit */
    String mEditTable;

    /** The names of the columns in the rows */
    ArrayOf<String>* mColumns;

    /** The query object for the cursor */
    AutoPtr<ISQLiteQuery> mQuery;

    /** The database the cursor was created from */
    AutoPtr<ISQLiteDatabase> mDatabase;

    /** The compiled query this cursor came from */
    AutoPtr<ISQLiteCursorDriver> mDriver;

    /** The number of rows in the cursor */
    Int32 mCount;

    /** A mapping of column names to column indices, to speed up lookups */
    Map<String, Int32> mColumnNameMap;

    /** Used to find out where a cursor was allocated in case it never got released. */
//   Throwable mStackTrace;
    
    /** 
     *  mMaxRead is the max items that each cursor window reads 
     *  default to a very high value
     */
//    Int32 mMaxRead = Integer.MAX_VALUE;
//    Int32 mInitialRead = Integer.MAX_VALUE;
    Int32 mCursorState;
//    ReentrantLock mLock = NULL;
    Boolean mPendingData;
};


#endif //__SQLITECURSOR_H__