#ifndef __SQLITEQUERY_H__
#define __SQLITEQUERY_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteProgram.h"
#include <elastos/ElRefBase.h>
#include <sqlite3.h>

/**
 * A SQLite program that represents a query that reads the resulting rows into a CursorWindow.
 * This class is used by SQLiteCursor and isn't useful itself.
 *
 * SQLiteQuery is not internally synchronized so code using a SQLiteQuery from multiple
 * threads should perform its own synchronization when using the SQLiteQuery.
 */
class SQLiteQuery
    : public ElRefBase
    , public SQLiteProgram
    , public ISQLiteQuery
{
public:
    /**
     * Create a persistent query object.
     *
     * @param db The database that this query object is associated with
     * @param query The SQL string for this query.
     * @param offsetIndex The 1-based index to the OFFSET parameter,
     */
    /* package */ SQLiteQuery(
        /* [in] */ SQLiteDatabase* db,
        /* [in] */ const String& query,
        /* [in] */ Int32 offsetIndex,
        /* [in] */ ArrayOf<String>* bindArgs);

    ~SQLiteQuery();

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Reads rows into a buffer. This method acquires the database lock.
     *
     * @param window The window to fill into
     * @return number of total rows in the query
     */
    /* package */ virtual CARAPI FillWindow(
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
    /* package */ virtual CARAPI_(Int32) ColumnCountLocked();

    /**
     * Retrieves the column name for the given column index. The database must be locked
     * when calling this method.
     *
     * @param columnIndex the index of the column to get the name for
     * @return The requested column's name
     */
    /* package */
    virtual CARAPI_(String) ColumnNameLocked(
        /* [in] */ Int32 columnIndex);

    //@Override
    CARAPI ToString(
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
    CARAPI BindInt64(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    //@Override
    CARAPI BindDouble(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    //@Override
    CARAPI BindString(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

    CARAPI GetUniqueId(
        /* [out] */ Int32* value);

    CARAPI BindBlob(
        /* [in] */ Int32 index,
        /* [in] */ const ArrayOf<Byte>& value);

    CARAPI ClearBindings();

private:
    CARAPI NativeFillWindow(
        /* [in] */ ICursorWindow* window,
        /* [in] */ Int32 startPos,
        /* [in] */ Int32 offsetParam,
        /* [in] */ Int32 maxRead,
        /* [in] */ Int32 lastPos,
        /* [out] */ Int32* rowNum);

    CARAPI_(Int32) NativeColumnCount();

    CARAPI_(String) NativeColumnName(
        /* [in] */ Int32 columnIndex);

private:
    const static CString TAG;

    /** The index of the unbound OFFSET parameter */
    Int32 mOffsetIndex;

    /** Args to bind on requery */
    ArrayOf<String>* mBindArgs;

    Boolean mClosed;
};
#endif //__SQLITEQUERY_H__