#ifndef __SQLITEQUERY_H__
#define __SQLITEQUERY_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteProgram.h"
#include <sqlite3.h>

class SQLiteQuery : public SQLiteProgram
{
public:
    SQLiteQuery();

    ~SQLiteQuery();

    /**
     * Create a persistent query object.
     * 
     * @param db The database that this query object is associated with
     * @param query The SQL string for this query. 
     * @param offsetIndex The 1-based index to the OFFSET parameter, 
     */
    SQLiteQuery(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ String query,
        /* [in] */ Int32 offsetIndex,
        /* [in] */ ArrayOf<String>* bindArgs);

    /**
     * Reads rows into a buffer. This method acquires the database lock.
     *
     * @param window The window to fill into
     * @return number of total rows in the query
     */
    virtual CARAPI FillWindow(
        /* [in] */ ICursorWindow* window,
        /* [in] */ Int32 maxRead,
        /* [in] */ Int32 lastPos,
        /* [out] */ Int32* rowNum);

    /**
     * Get the column count for the statement. Only valid on query based
     * statements. The database must be locked
     * when calling this method.
     * 
     * @return The number of column in the statement's result set.
     */
    virtual CARAPI ColumnCountLocked(
        /* [out] */ Int32* value);

    /**
     * Retrieves the column name for the given column index. The database must be locked
     * when calling this method.
     * 
     * @param columnIndex the index of the column to get the name for
     * @return The requested column's name
     */
    /* package */ 
    virtual CARAPI ColumnNameLocked(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* value);

    //@Override
    CARAPI toString(
        /* [out] */ String* value);

    //@Override
    CARAPI Close();

    /**
     * Called by SQLiteCursor when it is requeried.
     */
    /* package */
    virtual CARAPI Requery();

    //@Override
    CARAPI BindNull(
        /* [in] */ Int32 index);

    //@Override
    CARAPI BindLong(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    //@Override
    CARAPI BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    //@Override
    CARAPI BindString(
        /* [in] */ Int32 index,
        /* [in] */ String value);

private:
    CARAPI_(Int32) Native_Fill_Window(
        /* [in] */ ICursorWindow* window,
        /* [in] */ Int32 startPos,
        /* [in] */ Int32 offsetParam,
        /* [in] */ Int32 maxRead,
        /* [in] */ Int32 lastPos);
    
    CARAPI_(Int32) Native_Column_Count();

    CARAPI_(String) Native_Column_Name(
        /* [in] */ Int32 columnIndex);

    const static String TAG;

    /** The index of the unbound OFFSET parameter */
    Int32 mOffsetIndex;

    /** Args to bind on requery */
    ArrayOf<String>* mBindArgs;


    Boolean mClosed;
};
#endif //__SQLITEQUERY_H__