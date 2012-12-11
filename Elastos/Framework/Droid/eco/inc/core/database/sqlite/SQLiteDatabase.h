#ifndef __SQLITEDATABASE_H__
#define __SQLITEDATABASE_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteClosable.h"
#include "database/sqlite/SQLiteCompiledSql.h"
#include "utils/ReentrantLock.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Map.h>
#include <elastos/HashMap.h>
#include <elastos/HashSet.h>
#include <Logger.h>
#include <StringBuffer.h>
#include <sqlite3.h>

using namespace Elastos::Utility::Logging;

class SQLiteCompiledSql;

_ELASTOS_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<ISQLiteClosable> >
{
    size_t operator()(AutoPtr<ISQLiteClosable> name) const
    {
        return (size_t)name.Get();
    }
};

template<> struct EqualTo<AutoPtr<ISQLiteClosable> >
{
    Boolean operator()(const AutoPtr<ISQLiteClosable>& x,
                       const AutoPtr<ISQLiteClosable>& y) const
    {
        return x.Get() == y.Get();
    }
};

_ELASTOS_NAMESPACE_END

// #define SQLITE_SOFT_HEAP_LIMIT (4 * 1024 * 1024)
// #define UTF16_STORAGE 0
// #define ANDROID_TABLE "android_metadata"

/**
 * Exposes methods to manage a SQLite database.
 * <p>SQLiteDatabase has methods to create, delete, execute SQL commands, and
 * perform other common database management tasks.
 * <p>See the Notepad sample application in the SDK for an example of creating
 * and managing a database.
 * <p> Database names must be unique within an application, not across all
 * applications.
 *
 * <h3>Localized Collation - ORDER BY</h3>
 * <p>In addition to SQLite's default <code>BINARY</code> collator, Android supplies
 * two more, <code>LOCALIZED</code>, which changes with the system's current locale
 * if you wire it up correctly (XXX a link needed!), and <code>UNICODE</code>, which
 * is the Unicode Collation Algorithm and not tailored to the current locale.
 */
class SQLiteDatabase
    : public ElRefBase
    , public SQLiteClosable
    , public ISQLiteDatabase
{
public:
    struct HashKey
    {
        size_t operator()(const SQLiteDatabase* s) const
        {
            return (size_t)s->GetHashCode();
        }
    };

    class ActiveDatabases
    {
    public:
        static ActiveDatabases* GetInstance() { return sActiveDatabases; }

    private:
        ActiveDatabases() {} // disable instantiation of this class

    private:
        friend class SQLiteDatabase;

        // HashSet<WeakReference<SQLiteDatabase>> mActiveDatabases =
        //     new HashSet<WeakReference<SQLiteDatabase>>();
        HashSet< AutoPtr<SQLiteDatabase>, SQLiteDatabase::HashKey > mActiveDatabases;

        static ActiveDatabases* sActiveDatabases;
    };

private:
    /**
     * Internal class used to keep track what needs to be marked as changed
     * when an update occurs. This is used for syncing, so the sync engine
     * knows what data has been updated locally.
     */
    class SyncUpdateInfo
    {
    public:
        /**
         * Creates the SyncUpdateInfo class.
         *
         * @param masterTable The table to set _sync_time to NULL in
         * @param deletedTable The deleted table that corresponds to the
         *          master table
         * @param foreignKey The key that refers to the primary key in table
         */
        SyncUpdateInfo(
            /* [in] */ const String& masterTable,
            /* [in] */ const String& deletedTable,
            /* [in] */ const String& foreignKey);

    public:
        /** The table containing the _sync_time column */
        String mMasterTable;

        /** The deleted table that corresponds to the master table */
        String mDeletedTable;

        /** The key in the local table the row in table. It may be _id, if table
         * is the local table. */
        String mForeignKey;
    };

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI_(void) AddSQLiteClosable(
        /* [in] */ ISQLiteClosable* closable);

    virtual CARAPI_(void) RemoveSQLiteClosable(
        /* [in] */ ISQLiteClosable* closable);

    /**
     * Attempts to release memory that SQLite holds but does not require to
     * operate properly. Typically this memory will come from the page cache.
     *
     * @return the number of bytes actually released
     */
     static CARAPI_(Int32) ReleaseMemory();

    /**
     * Control whether or not the SQLiteDatabase is made thread-safe by using locks
     * around critical sections. This is pretty expensive, so if you know that your
     * DB will only be used by a single thread then you should set this to false.
     * The default is true.
     * @param lockingEnabled set to true to enable locks, false otherwise
     */
    virtual CARAPI SetLockingEnabled(
        /* [in] */ Boolean lockingEnabled);

    /**
     * Begins a transaction. Transactions can be nested. When the outer transaction is ended all of
     * the work done in that transaction and all of the nested transactions will be committed or
     * rolled back. The changes will be rolled back if any transaction is ended without being
     * marked as clean (by calling setTransactionSuccessful). Otherwise they will be committed.
     *
     * <p>Here is the standard idiom for transactions:
     *
     * <pre>
     *   db.beginTransaction();
     *   try {
     *     ...
     *     db.setTransactionSuccessful();
     *   } finally {
     *     db.endTransaction();
     *   }
     * </pre>
     */
    virtual CARAPI BeginTransaction();

    /**
     * Begins a transaction. Transactions can be nested. When the outer transaction is ended all of
     * the work done in that transaction and all of the nested transactions will be committed or
     * rolled back. The changes will be rolled back if any transaction is ended without being
     * marked as clean (by calling setTransactionSuccessful). Otherwise they will be committed.
     *
     * <p>Here is the standard idiom for transactions:
     *
     * <pre>
     *   db.beginTransactionWithListener(listener);
     *   try {
     *     ...
     *     db.setTransactionSuccessful();
     *   } finally {
     *     db.endTransaction();
     *   }
     * </pre>
     * @param transactionListener listener that should be notified when the transaction begins,
     * commits, or is rolled back, either explicitly or by a call to
     * {@link #yieldIfContendedSafely}.
     */
    virtual CARAPI BeginTransactionWithListener(
        /* [in] */ ISQLiteTransactionListener* transactionListener);


    /**
     * End a transaction. See beginTransaction for notes about how to use this and when transactions
     * are committed and rolled back.
     */
    virtual CARAPI EndTransaction();

    /**
     * Marks the current transaction as successful. Do not do any more database work between
     * calling this and calling endTransaction. Do as little non-database work as possible in that
     * situation too. If any errors are encountered between this and endTransaction the transaction
     * will still be committed.
     *
     * @throws IllegalStateException if the current thread is not in a transaction or the
     * transaction is already marked as successful.
     */
    virtual CARAPI SetTransactionSuccessful();

    /**
     * return true if there is a transaction pending
     */
    virtual CARAPI InTransaction(
        /* [out] */ Boolean* result);

    /**
     * Checks if the database lock is held by this thread.
     *
     * @return true, if this thread is holding the database lock.
     */
    virtual CARAPI IsDbLockedByCurrentThread(
        /* [out] */ Boolean* result);

    /**
     * Checks if the database is locked by another thread. This is
     * just an estimate, since this status can change at any time,
     * including after the call is made but before the result has
     * been acted upon.
     *
     * @return true, if the database is locked by another thread
     */
    virtual CARAPI IsDbLockedByOtherThreads(
        /* [out] */ Boolean* result);

    /**
     * Temporarily end the transaction to let other threads run. The transaction is assumed to be
     * successful so far. Do not call setTransactionSuccessful before calling this. When this
     * returns a new transaction will have been created but not marked as successful.
     * @return true if the transaction was yielded
     * @deprecated if the db is locked more than once (becuase of nested transactions) then the lock
     *   will not be yielded. Use yieldIfContendedSafely instead.
     */
    //@Deprecated
    virtual CARAPI YieldIfContended(
        /* [out] */ Boolean* result);

    /**
     * Temporarily end the transaction to let other threads run. The transaction is assumed to be
     * successful so far. Do not call setTransactionSuccessful before calling this. When this
     * returns a new transaction will have been created but not marked as successful. This assumes
     * that there are no nested transactions (beginTransaction has only been called once) and will
     * throw an exception if that is not the case.
     * @return true if the transaction was yielded
     */
    virtual CARAPI YieldIfContendedSafely(
        /* [out] */ Boolean* result);

    /**
     * Temporarily end the transaction to let other threads run. The transaction is assumed to be
     * successful so far. Do not call setTransactionSuccessful before calling this. When this
     * returns a new transaction will have been created but not marked as successful. This assumes
     * that there are no nested transactions (beginTransaction has only been called once) and will
     * throw an exception if that is not the case.
     * @param sleepAfterYieldDelay if > 0, sleep this long before starting a new transaction if
     *   the lock was actually yielded. This will allow other background threads to make some
     *   more progress than they would if we started the transaction immediately.
     * @return true if the transaction was yielded
     */
    virtual CARAPI YieldIfContendedSafelyEx(
        /* [in] */ Int64 sleepAfterYieldDelay,
        /* [out] */ Boolean* result);

    virtual CARAPI GetSyncedTables(
        /* [out] */ IObjectStringMap** tables);

    /**
     * Open the database according to the flags {@link #OPEN_READWRITE}
     * {@link #OPEN_READONLY} {@link #CREATE_IF_NECESSARY} and/or {@link #NO_LOCALIZED_COLLATORS}.
     *
     * <p>Sets the locale of the database to the  the system's current locale.
     * Call {@link #setLocale} if you would like something else.</p>
     *
     * @param path to database file to open and/or create
     * @param factory an optional factory class that is called to instantiate a
     *            cursor when query is called, or null for default
     * @param flags to control database access mode
     * @return the newly opened database
     * @throws SQLiteException if the database cannot be opened
     */
    static CARAPI OpenDatabase(
        /* [in] */ const String& path,
        /* [in] */ ICursorFactory* factory,
        /* [in] */ Int32 flags,
        /* [out] */ ISQLiteDatabase** db);


    /**
     * Equivalent to openDatabase(file.getPath(), factory, CREATE_IF_NECESSARY).
     */
    static CARAPI OpenOrCreateDatabase(
        /* [in] */ IFile* file,
        /* [in] */ ICursorFactory* factory,
        /* [out] */ ISQLiteDatabase** db);

    /**
     * Equivalent to openDatabase(path, factory, CREATE_IF_NECESSARY).
     */
    static CARAPI OpenOrCreateDatabase(
        /* [in] */ const String& path,
        /* [in] */ ICursorFactory* factory,
        /* [out] */ ISQLiteDatabase** db);

    /**
     * Create a memory backed SQLite database.  Its contents will be destroyed
     * when the database is closed.
     *
     * <p>Sets the locale of the database to the  the system's current locale.
     * Call {@link #setLocale} if you would like something else.</p>
     *
     * @param factory an optional factory class that is called to instantiate a
     *            cursor when query is called
     * @return a SQLiteDatabase object, or null if the database can't be created
     */
    static CARAPI Create(
        /* [in] */ ICursorFactory* factory,
        /* [out] */ ISQLiteDatabase** db);

    /**
     * Close the database.
     */
    virtual CARAPI Close();

    /**
     * Gets the database version.
     *
     * @return the database version
     */
    virtual CARAPI GetVersion(
        /* [out] */ Int32* version);

    /**
     * Sets the database version.
     *
     * @param version the new database version
     */
    virtual CARAPI SetVersion(
        /* [in] */ Int32 version);

    /**
     * Returns the maximum size the database may grow to.
     *
     * @return the new maximum database size
     */
    virtual CARAPI GetMaximumSize(
        /* [out] */ Int64* maxSize);

    /**
     * Sets the maximum size the database will grow to. The maximum size cannot
     * be set below the current size.
     *
     * @param numBytes the maximum database size, in bytes
     * @return the new maximum database size
     */
    virtual CARAPI SetMaximumSize(
        /* [in] */ Int64 numBytes,
        /* [out] */ Int64* maxSize);

    /**
     * Returns the current database page size, in bytes.
     *
     * @return the database page size, in bytes
     */
    virtual CARAPI GetPageSize(
        /* [out] */ Int64* pageSize);

    /**
     * Sets the database page size. The page size must be a power of two. This
     * method does not work if any data has been written to the database file,
     * and must be called right after the database has been created.
     *
     * @param numBytes the database page size, in bytes
     */
    virtual CARAPI SetPageSize(
        /* [in] */ Int64 numBytes);

    /**
     * Mark this table as syncable. When an update occurs in this table the
     * _sync_dirty field will be set to ensure proper syncing operation.
     *
     * @param table the table to mark as syncable
     * @param deletedTable The deleted table that corresponds to the
     *          syncable table
     */
    virtual CARAPI MarkTableSyncable(
        /* [in] */ const String& table,
        /* [in] */ const String& deletedTable);

    /**
     * Mark this table as syncable, with the _sync_dirty residing in another
     * table. When an update occurs in this table the _sync_dirty field of the
     * row in updateTable with the _id in foreignKey will be set to
     * ensure proper syncing operation.
     *
     * @param table an update on this table will trigger a sync time removal
     * @param foreignKey this is the column in table whose value is an _id in
     *          updateTable
     * @param updateTable this is the table that will have its _sync_dirty
     */
    virtual CARAPI MarkTableSyncableEx(
        /* [in] */ const String& table,
        /* [in] */ const String& foreignKey,
        /* [in] */ const String& updateTable);

    /**
     * Finds the name of the first table, which is editable.
     *
     * @param tables a list of tables
     * @return the first table listed
     */
    static CARAPI FindEditTable(
        /* [in] */ const String& tables,
        /* [out] */ String* editable);

    /**
     * Compiles an SQL statement into a reusable pre-compiled statement object.
     * The parameters are identical to {@link #execSQL(String)}. You may put ?s in the
     * statement and fill in those values with {@link SQLiteProgram#bindString}
     * and {@link SQLiteProgram#bindLong} each time you want to run the
     * statement. Statements may not return result sets larger than 1x1.
     *
     * @param sql The raw SQL statement, may contain ? for unknown values to be
     *            bound later.
     * @return A pre-compiled {@link SQLiteStatement} object. Note that
     * {@link SQLiteStatement}s are not synchronized, see the documentation for more details.
     */
    virtual CARAPI CompileStatement(
        /* [in] */ const String& sql,
        /* [out] */ ISQLiteStatement** stmt);

    /**
     * Query the given URL, returning a {@link Cursor} over the result set.
     *
     * @param distinct true if you want each row to be unique, false otherwise.
     * @param table The table name to compile the query against.
     * @param columns A list of which columns to return. Passing null will
     *            return all columns, which is discouraged to prevent reading
     *            data from storage that isn't going to be used.
     * @param selection A filter declaring which rows to return, formatted as an
     *            SQL WHERE clause (excluding the WHERE itself). Passing null
     *            will return all rows for the given table.
     * @param selectionArgs You may include ?s in selection, which will be
     *         replaced by the values from selectionArgs, in order that they
     *         appear in the selection. The values will be bound as Strings.
     * @param groupBy A filter declaring how to group rows, formatted as an SQL
     *            GROUP BY clause (excluding the GROUP BY itself). Passing null
     *            will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in the cursor,
     *            if row grouping is being used, formatted as an SQL HAVING
     *            clause (excluding the HAVING itself). Passing null will cause
     *            all row groups to be included, and is required when row
     *            grouping is not being used.
     * @param orderBy How to order the rows, formatted as an SQL ORDER BY clause
     *            (excluding the ORDER BY itself). Passing null will use the
     *            default sort order, which may be unordered.
     * @param limit Limits the number of rows returned by the query,
     *            formatted as LIMIT clause. Passing null denotes no LIMIT clause.
     * @return A {@link Cursor} object, which is positioned before the first entry. Note that
     * {@link Cursor}s are not synchronized, see the documentation for more details.
     * @see Cursor
     */
    virtual CARAPI Query(
        /* [in] */ Boolean distinct,
        /* [in] */ const String& table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ ICursor** cursor);

    /**
     * Query the given URL, returning a {@link Cursor} over the result set.
     *
     * @param cursorFactory the cursor factory to use, or null for the default factory
     * @param distinct true if you want each row to be unique, false otherwise.
     * @param table The table name to compile the query against.
     * @param columns A list of which columns to return. Passing null will
     *            return all columns, which is discouraged to prevent reading
     *            data from storage that isn't going to be used.
     * @param selection A filter declaring which rows to return, formatted as an
     *            SQL WHERE clause (excluding the WHERE itself). Passing null
     *            will return all rows for the given table.
     * @param selectionArgs You may include ?s in selection, which will be
     *         replaced by the values from selectionArgs, in order that they
     *         appear in the selection. The values will be bound as Strings.
     * @param groupBy A filter declaring how to group rows, formatted as an SQL
     *            GROUP BY clause (excluding the GROUP BY itself). Passing null
     *            will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in the cursor,
     *            if row grouping is being used, formatted as an SQL HAVING
     *            clause (excluding the HAVING itself). Passing null will cause
     *            all row groups to be included, and is required when row
     *            grouping is not being used.
     * @param orderBy How to order the rows, formatted as an SQL ORDER BY clause
     *            (excluding the ORDER BY itself). Passing null will use the
     *            default sort order, which may be unordered.
     * @param limit Limits the number of rows returned by the query,
     *            formatted as LIMIT clause. Passing null denotes no LIMIT clause.
     * @return A {@link Cursor} object, which is positioned before the first entry. Note that
     * {@link Cursor}s are not synchronized, see the documentation for more details.
     * @see Cursor
     */
    virtual CARAPI QueryWithFactory(
        /* [in] */ ICursorFactory* cursorFactory,
        /* [in] */ Boolean distinct,
        /* [in] */ const String& table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ ICursor** cursor);

    /**
     * Query the given table, returning a {@link Cursor} over the result set.
     *
     * @param table The table name to compile the query against.
     * @param columns A list of which columns to return. Passing null will
     *            return all columns, which is discouraged to prevent reading
     *            data from storage that isn't going to be used.
     * @param selection A filter declaring which rows to return, formatted as an
     *            SQL WHERE clause (excluding the WHERE itself). Passing null
     *            will return all rows for the given table.
     * @param selectionArgs You may include ?s in selection, which will be
     *         replaced by the values from selectionArgs, in order that they
     *         appear in the selection. The values will be bound as Strings.
     * @param groupBy A filter declaring how to group rows, formatted as an SQL
     *            GROUP BY clause (excluding the GROUP BY itself). Passing null
     *            will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in the cursor,
     *            if row grouping is being used, formatted as an SQL HAVING
     *            clause (excluding the HAVING itself). Passing null will cause
     *            all row groups to be included, and is required when row
     *            grouping is not being used.
     * @param orderBy How to order the rows, formatted as an SQL ORDER BY clause
     *            (excluding the ORDER BY itself). Passing null will use the
     *            default sort order, which may be unordered.
     * @return A {@link Cursor} object, which is positioned before the first entry. Note that
     * {@link Cursor}s are not synchronized, see the documentation for more details.
     * @see Cursor
     */
    virtual CARAPI QueryEx(
        /* [in] */ const String& table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [out] */ ICursor** cursor);

    /**
     * Query the given table, returning a {@link Cursor} over the result set.
     *
     * @param table The table name to compile the query against.
     * @param columns A list of which columns to return. Passing null will
     *            return all columns, which is discouraged to prevent reading
     *            data from storage that isn't going to be used.
     * @param selection A filter declaring which rows to return, formatted as an
     *            SQL WHERE clause (excluding the WHERE itself). Passing null
     *            will return all rows for the given table.
     * @param selectionArgs You may include ?s in selection, which will be
     *         replaced by the values from selectionArgs, in order that they
     *         appear in the selection. The values will be bound as Strings.
     * @param groupBy A filter declaring how to group rows, formatted as an SQL
     *            GROUP BY clause (excluding the GROUP BY itself). Passing null
     *            will cause the rows to not be grouped.
     * @param having A filter declare which row groups to include in the cursor,
     *            if row grouping is being used, formatted as an SQL HAVING
     *            clause (excluding the HAVING itself). Passing null will cause
     *            all row groups to be included, and is required when row
     *            grouping is not being used.
     * @param orderBy How to order the rows, formatted as an SQL ORDER BY clause
     *            (excluding the ORDER BY itself). Passing null will use the
     *            default sort order, which may be unordered.
     * @param limit Limits the number of rows returned by the query,
     *            formatted as LIMIT clause. Passing null denotes no LIMIT clause.
     * @return A {@link Cursor} object, which is positioned before the first entry. Note that
     * {@link Cursor}s are not synchronized, see the documentation for more details.
     * @see Cursor
     */
    virtual CARAPI QueryEx2(
        /* [in] */ const String& table,
        /* [in] */ ArrayOf<String>* columns,
        /* [in] */ const String& selection,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& groupBy,
        /* [in] */ const String& having,
        /* [in] */ const String& orderBy,
        /* [in] */ const String& limit,
        /* [out] */ ICursor** cursor);

    /**
     * Runs the provided SQL and returns a {@link Cursor} over the result set.
     *
     * @param sql the SQL query. The SQL string must not be ; terminated
     * @param selectionArgs You may include ?s in where clause in the query,
     *     which will be replaced by the values from selectionArgs. The
     *     values will be bound as Strings.
     * @return A {@link Cursor} object, which is positioned before the first entry. Note that
     * {@link Cursor}s are not synchronized, see the documentation for more details.
     */
    virtual CARAPI RawQuery(
        /* [in] */ const String& sql,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ ICursor** cursor);

    /**
     * Runs the provided SQL and returns a cursor over the result set.
     *
     * @param cursorFactory the cursor factory to use, or null for the default factory
     * @param sql the SQL query. The SQL string must not be ; terminated
     * @param selectionArgs You may include ?s in where clause in the query,
     *     which will be replaced by the values from selectionArgs. The
     *     values will be bound as Strings.
     * @param editTable the name of the first table, which is editable
     * @return A {@link Cursor} object, which is positioned before the first entry. Note that
     * {@link Cursor}s are not synchronized, see the documentation for more details.
     */
    virtual CARAPI RawQueryWithFactory(
        /* [in] */ ICursorFactory* cursorFactory,
        /* [in] */ const String& sql,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ const String& editTable,
        /* [out] */ ICursor** cursor);

    /**
     * Runs the provided SQL and returns a cursor over the result set.
     * The cursor will read an initial set of rows and the return to the caller.
     * It will continue to read in batches and send data changed notifications
     * when the later batches are ready.
     * @param sql the SQL query. The SQL string must not be ; terminated
     * @param selectionArgs You may include ?s in where clause in the query,
     *     which will be replaced by the values from selectionArgs. The
     *     values will be bound as Strings.
     * @param initialRead set the initial count of items to read from the cursor
     * @param maxRead set the count of items to read on each iteration after the first
     * @return A {@link Cursor} object, which is positioned before the first entry. Note that
     * {@link Cursor}s are not synchronized, see the documentation for more details.
     *
     * This work is incomplete and not fully tested or reviewed, so currently
     * hidden.
     * @hide
     */
    virtual CARAPI RawQueryEx(
        /* [in] */ const String& sql,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [in] */ Int32 initialRead,
        /* [in] */ Int32 maxRead,
        /* [out] */ ICursor** cursor);

    /**
     * Convenience method for inserting a row into the database.
     *
     * @param table the table to insert the row into
     * @param nullColumnHack optional; may be <code>null</code>.
     *            SQL doesn't allow inserting a completely empty row without
     *            naming at least one column name.  If your provided <code>values</code> is
     *            empty, no column names are known and an empty row can't be inserted.
     *            If not set to null, the <code>nullColumnHack</code> parameter
     *            provides the name of nullable column name to explicitly insert a NULL into
     *            in the case where your <code>values</code> is empty.
     * @param values this map contains the initial column values for the
     *            row. The keys should be the column names and the values the
     *            column values
     * @return the row ID of the newly inserted row, or -1 if an error occurred
     */
    virtual CARAPI Insert(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues* values,
        /* [out] */ Int64* rowId);

    /**
     * Convenience method for inserting a row into the database.
     *
     * @param table the table to insert the row into
     * @param nullColumnHack optional; may be <code>null</code>.
     *            SQL doesn't allow inserting a completely empty row without
     *            naming at least one column name.  If your provided <code>values</code> is
     *            empty, no column names are known and an empty row can't be inserted.
     *            If not set to null, the <code>nullColumnHack</code> parameter
     *            provides the name of nullable column name to explicitly insert a NULL into
     *            in the case where your <code>values</code> is empty.
     * @param values this map contains the initial column values for the
     *            row. The keys should be the column names and the values the
     *            column values
     * @throws SQLException
     * @return the row ID of the newly inserted row, or -1 if an error occurred
     */
    virtual CARAPI InsertOrThrow(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues* values,
        /* [out] */ Int64* rowId);

    /**
     * Convenience method for replacing a row in the database.
     *
     * @param table the table in which to replace the row
     * @param nullColumnHack optional; may be <code>null</code>.
     *            SQL doesn't allow inserting a completely empty row without
     *            naming at least one column name.  If your provided <code>initialValues</code> is
     *            empty, no column names are known and an empty row can't be inserted.
     *            If not set to null, the <code>nullColumnHack</code> parameter
     *            provides the name of nullable column name to explicitly insert a NULL into
     *            in the case where your <code>initialValues</code> is empty.
     * @param initialValues this map contains the initial column values for
     *   the row.
     * @return the row ID of the newly inserted row, or -1 if an error occurred
     */
    virtual CARAPI Replace(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues* initialValues,
        /* [out] */ Int64* rowId);

    /**
     * Convenience method for replacing a row in the database.
     *
     * @param table the table in which to replace the row
     * @param nullColumnHack optional; may be <code>null</code>.
     *            SQL doesn't allow inserting a completely empty row without
     *            naming at least one column name.  If your provided <code>initialValues</code> is
     *            empty, no column names are known and an empty row can't be inserted.
     *            If not set to null, the <code>nullColumnHack</code> parameter
     *            provides the name of nullable column name to explicitly insert a NULL into
     *            in the case where your <code>initialValues</code> is empty.
     * @param initialValues this map contains the initial column values for
     *   the row. The key
     * @throws SQLException
     * @return the row ID of the newly inserted row, or -1 if an error occurred
     */
    virtual CARAPI ReplaceOrThrow(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues* initialValues,
        /* [out] */ Int64* rowId);

    /**
     * General method for inserting a row into the database.
     *
     * @param table the table to insert the row into
     * @param nullColumnHack optional; may be <code>null</code>.
     *            SQL doesn't allow inserting a completely empty row without
     *            naming at least one column name.  If your provided <code>initialValues</code> is
     *            empty, no column names are known and an empty row can't be inserted.
     *            If not set to null, the <code>nullColumnHack</code> parameter
     *            provides the name of nullable column name to explicitly insert a NULL into
     *            in the case where your <code>initialValues</code> is empty.
     * @param initialValues this map contains the initial column values for the
     *            row. The keys should be the column names and the values the
     *            column values
     * @param conflictAlgorithm for insert conflict resolver
     * @return the row ID of the newly inserted row
     * OR the primary key of the existing row if the input param 'conflictAlgorithm' =
     * {@link #CONFLICT_IGNORE}
     * OR -1 if any error
     */
    virtual CARAPI InsertWithOnConflict(
        /* [in] */ const String& table,
        /* [in] */ const String& nullColumnHack,
        /* [in] */ IContentValues* initialValues,
        /* [in] */ Int32 conflictAlgorithm,
        /* [out] */ Int64* rowId);

    /**
     * Convenience method for deleting rows in the database.
     *
     * @param table the table to delete from
     * @param whereClause the optional WHERE clause to apply when deleting.
     *            Passing null will delete all rows.
     * @return the number of rows affected if a whereClause is passed in, 0
     *         otherwise. To remove all rows and get a count pass "1" as the
     *         whereClause.
     */
    virtual CARAPI Delete(
        /* [in] */ const String& table,
        /* [in] */ const String& whereClause,
        /* [in] */ ArrayOf<String>* whereArgs,
        /* [out] */ Int32* value);

    /**
     * Convenience method for updating rows in the database.
     *
     * @param table the table to update in
     * @param values a map from column names to new column values. null is a
     *            valid value that will be translated to NULL.
     * @param whereClause the optional WHERE clause to apply when updating.
     *            Passing null will update all rows.
     * @return the number of rows affected
     */
    virtual CARAPI Update(
        /* [in] */ const String& table,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& whereClause,
        /* [in] */ ArrayOf<String>* whereArgs,
        /* [out] */ Int32* value);

    /**
     * Convenience method for updating rows in the database.
     *
     * @param table the table to update in
     * @param values a map from column names to new column values. null is a
     *            valid value that will be translated to NULL.
     * @param whereClause the optional WHERE clause to apply when updating.
     *            Passing null will update all rows.
     * @param conflictAlgorithm for update conflict resos.append(name);
        DatabaseUtils.appendEscapedSQLString(s, clause);
    }lver
     * @return the number of rows affected
     */
    virtual CARAPI UpdateWithOnConflict(
        /* [in] */ const String& table,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& whereClause,
        /* [in] */ ArrayOf<String>* whereArgs,
        /* [in] */ Int32 conflictAlgorithm,
        /* [out] */ Int32* value);

    /**
     * Execute a single SQL statement that is not a query. For example, CREATE
     * TABLE, DELETE, INSERT, etc. Multiple statements separated by semicolons are not
     * supported.  Takes a write lock.
     *
     * @throws SQLException if the SQL string is invalid
     */
    virtual CARAPI ExecSQL(
        /* [in] */ CString sql);

    /**
     * Execute a single SQL statement that is not a query. For example, CREATE
     * TABLE, DELETE, INSERT, etc. Multiple statements separated by semicolons are not
     * supported.  Takes a write lock.
     *
     * @param sql
     * @param bindArgs only byte[], String, Long and Double are supported in bindArgs.
     * @throws SQLException if the SQL string is invalid
     */
    virtual CARAPI ExecSQLEx(
        /* [in] */ CString sql,
        /* [in] */ ArrayOf<IInterface*>* bindArgs);

    /**
     * return whether the DB is opened as read only.
     * @return true if DB is opened as read only
     */
    virtual CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    /**
     * @return true if the DB is currently open (has not been closed)
     */
    virtual CARAPI IsOpen(
        /* [out] */ Boolean* isOpen);

    virtual CARAPI NeedUpgrade(
        /* [in] */ Int32 newVersion,
        /* [out] */ Boolean* needed);

    /**
     * Getter for the path to the database file.
     *
     * @return the path to our database file.
     */
    CARAPI GetPath(
        /* [out] */ String* path);

    /**
     * Sets the locale for this database.  Does nothing if this database has
     * the NO_LOCALIZED_COLLATORS flag set or was opened read only.
     * @throws SQLException if the locale could not be set.  The most common reason
     * for this is that there is no collator available for the locale you requested.
     * In this case the database remains unchanged.
     */
    virtual CARAPI SetLocale(
        /* [in] */ ILocale* locale);

    /**
     * returns true if the given sql is cached in compiled-sql cache.
     * @hide
     */
    virtual CARAPI IsInCompiledSqlCache(
        /* [in] */ const String& sql,
        /* [out] */ Boolean* result);

    /**
     * purges the given sql from the compiled-sql cache.
     * @hide
     */
    virtual CARAPI PurgeFromCompiledSqlCache(
        /* [in] */ const String& sql);

    /**
     * remove everything from the compiled sql cache
     * @hide
     */
    virtual CARAPI ResetCompiledSqlCache();

    /**
     * return the current maxCacheSqlCacheSize
     * @hide
     */
    virtual CARAPI GetMaxSqlCacheSize(
        /* [out] */ Int32* size);

    /**
     * set the max size of the compiled sql cache for this database after purging the cache.
     * (size of the cache = number of compiled-sql-statements stored in the cache).
     *
     * max cache size can ONLY be increased from its current size (default = 0).
     * if this method is called with smaller size than the current value of mMaxSqlCacheSize,
     * then IllegalStateException is thrown
     *
     * synchronized because we don't want t threads to change cache size at the same time.
     * @param cacheSize the size of the cache. can be (0 to MAX_SQL_CACHE_SIZE)
     * @throws IllegalStateException if input cacheSize > MAX_SQL_CACHE_SIZE or < 0 or
     * < the value set with previous setMaxSqlCacheSize() call.
     *
     * @hide
     */
    virtual CARAPI SetMaxSqlCacheSize(
        /* [in] */ Int32 cacheSize);

public:
        /* package */ virtual CARAPI_(void) OnCorruption();

    /**
     * Locks the database for exclusive access. The database lock must be held when
     * touch the native sqlite3* object since it is single threaded and uses
     * a polling lock contention algorithm. The lock is recursive, and may be acquired
     * multiple times by the same thread. This is a no-op if mLockingEnabled is false.
     *
     * @see #unlock()
     */
    /* package */ virtual CARAPI_(void) Lock();

    /**
     * Releases the database lock. This is a no-op if mLockingEnabled is false.
     *
     * @see #unlock()
     */
    /* package */ virtual CARAPI_(void) Unlock();

    /**
     * Call for each row that is updated in a cursor.
     *
     * @param table the table the row is in
     * @param rowId the row ID of the updated row
     */
    /* package */ virtual CARAPI_(void) RowUpdated(
        /* [in] */ const String& table,
        /* [in] */ Int32 rowId);

    /* package */ virtual CARAPI_(void) LogTimeStat(
        /* [in] */ CString sql,
        /* [in] */ Int64 beginMillis);

    /* package */ virtual CARAPI_(void) LogTimeStat(
        /* [in] */ CString sql,
        /* [in] */ Int64 beginMillis,
        /* [in] */ CString prefix);

    /*
     * ============================================================================
     *
     *       The following methods deal with compiled-sql cache
     * ============================================================================
     */
    /**
     * adds the given sql and its compiled-statement-id-returned-by-sqlite to the
     * cache of compiledQueries attached to 'this'.
     *
     * if there is already a {@link SQLiteCompiledSql} in compiledQueries for the given sql,
     * the new {@link SQLiteCompiledSql} object is NOT inserted into the cache (i.e.,the current
     * mapping is NOT replaced with the new mapping).
     */
    /* package */ virtual CARAPI_(void) AddToCompiledQueries(
        /* [in] */ const String& sql,
        /* [in] */ SQLiteCompiledSql* compiledStatement);

    /**
     * from the compiledQueries cache, returns the compiled-statement-id for the given sql.
     * returns null, if not found in the cache.
     */
    /* package */ virtual CARAPI_(AutoPtr<SQLiteCompiledSql>) GetCompiledStatementForSql(
        /* [in] */ const String& sql);

/*    class ActiveDatabases {
        private static final ActiveDatabases activeDatabases = new ActiveDatabases();
        private HashSet<WeakReference<SQLiteDatabase>> mActiveDatabases =
            new HashSet<WeakReference<SQLiteDatabase>>();
        private ActiveDatabases() {} // disable instantiation of this class
        static ActiveDatabases getInstance() {return activeDatabases;}
    };*/

    /**
     * this method is used to collect data about ALL open databases in the current process.
     * bugreport is a user of this data.
     */
//    /* package */ static ArrayList<DbStats> getDbStats();

     /**
     * Native call to execute a raw SQL statement. {@link #lock} must be held
     * when calling this method.
     *
     * @param sql The raw SQL string
     * @throws SQLException
     */
    /* package */ CARAPI NativeExecSQL(
        /* [in] */ CString sql);

    /**
     * Native call to set the locale.  {@link #lock} must be held when calling
     * this method.
     * @throws SQLException
     */
    /* package */ CARAPI NativeSetLocale(
        /* [in] */ CString loc,
        /* [in] */ Int32 flags);

    /**
     * Returns the row ID of the last row inserted into the database.
     *
     * @return the row ID of the last row inserted into the database.
     */
    /* package */ CARAPI_(Int64) LastInsertRow();

    /**
     * Returns the number of changes made in the last statement executed.
     *
     * @return the number of changes made in the last statement executed.
     */
    /* package */ CARAPI_(Int32) LastChangeCount();

    CARAPI AcquireReference();

    CARAPI ReleaseReference();

    CARAPI ReleaseReferenceFromContainer();

protected:
    virtual ~SQLiteDatabase();

    //@Override
    CARAPI OnAllReferencesReleased();

private:
    /**
     * Private constructor. See {@link #create} and {@link #openDatabase}.
     *
     * @param path The full path to the database
     * @param factory The factory to use when creating cursors, may be NULL.
     * @param flags 0 or {@link #NO_LOCALIZED_COLLATORS}.  If the database file already
     *              exists, mFlags will be updated appropriately.
     */
    SQLiteDatabase(
        /* [in] */ const String& path,
        /* [in] */ ICursorFactory* factory,
        /* [in] */ Int32 flags);

    /**
     * Locks the database for exclusive access. The database lock must be held when
     * touch the native sqlite3* object since it is single threaded and uses
     * a polling lock contention algorithm. The lock is recursive, and may be acquired
     * multiple times by the same thread.
     *
     * @see #unlockForced()
     */
    CARAPI_(void) LockForced();

    /**
     * Releases the database lock.
     *
     * @see #unlockForced()
     */
    CARAPI_(void) UnlockForced();

    CARAPI_(void) CheckLockHoldTime();

    CARAPI_(Boolean) YieldIfContendedHelper(
        /* [in] */ Boolean checkFullyYielded,
        /* [in] */ Int64 sleepAfterYieldDelay);

    CARAPI CloseClosable();

    /**
     * Native call to close the database.
     */
    CARAPI Dbclose();

    /**
     * Mark this table as syncable, with the _sync_dirty residing in another
     * table. When an update occurs in this table the _sync_dirty field of the
     * row in updateTable with the _id in foreignKey will be set to
     * ensure proper syncing operation.
     *
     * @param table an update on this table will trigger a sync time removal
     * @param foreignKey this is the column in table whose value is an _id in
     *          updateTable
     * @param updateTable this is the table that will have its _sync_dirty
     * @param deletedTable The deleted table that corresponds to the
     *          updateTable
     */
    CARAPI MarkTableSyncable(
        /* [in] */ const String& table,
        /* [in] */ const String& foreignKey,
        /* [in] */ const String& updateTable,
        /* [in] */ const String& deletedTable);

    CARAPI_(String) GetTime();

    /**
     * Removes email addresses from database filenames before they're
     * logged to the EventLog where otherwise apps could potentially
     * read them.
     */
    CARAPI_(String) GetPathForLogs();

    CARAPI_(void) DeallocCachedSqlStatements();

    /**
     * get the specified pragma value from sqlite for the specified database.
     * only handles pragma's that return int/long.
     * NO JAVA locks are held in this method.
     * TODO: use this to do all pragma's in this class
     */
    static CARAPI_(Int64) GetPragmaVal(
        /* [in] */ SQLiteDatabase* db,
        /* [in] */ const String& pragma);

    /**
     * returns list of full pathnames of all attached databases
     * including the main database
     * TODO: move this to {@link DatabaseUtils}
     */
//    static CARAPI GetAttachedDbs(
//        /* [in] */ ISQLiteDatabase* dbObj,
//        /* [out] */ Set<Pair<String, String> >* attachedDbs);

    /**
     * Native call to open the database.
     *
     * @param path The full path to the database
     */
     CARAPI Dbopen(
        /* [in] */ CString path,
        /* [in] */ Int32 flags);

    /**
     * Native call to setup tracing of all sql statements
     *
     * @param path the full path to the database
     */
//    private native void enableSqlTracing(String path);
     CARAPI_(void) EnableSqlTracing(
        /* [in] */ CString path);

    /**
     * Native call to setup profiling of all sql statements.
     * currently, sqlite's profiling = printing of execution-time
     * (wall-clock time) of each of the sql statements, as they
     * are executed.
     *
     * @param path the full path to the database
     */
     CARAPI_(void) EnableSqlProfiling(
        /* [in] */ CString path);

    /**
     * return the SQLITE_DBSTATUS_LOOKASIDE_USED documented here
     * http://www.sqlite.org/c3ref/c_dbstatus_lookaside_used.html
     * @return int value of SQLITE_DBSTATUS_LOOKASIDE_USED
     */
    CARAPI_(Int32) NativeGetDbLookaside();

    CARAPI_(Int32) GetHashCode() const;

public:
    // String prefix for slow database query EventLog records that show
    // lock acquistions of the database.
    /* package */ const static CString GET_LOCK_LOG_PREFIX;

    /** Used by native code, do not rename */
    /* package */ sqlite3* mNativeHandle;

    /** Used to make temp table names unique */
    /* package */ Int32 mTempTableSequence;

    /**
     * for each instance of this class, a cache is maintained to store
     * the compiled query statement ids returned by sqlite database.
     *     key = sql statement with "?" for bind args
     *     value = {@link SQLiteCompiledSql}
     * If an application opens the database and keeps it open during its entire life, then
     * there will not be an overhead of compilation of sql statements by sqlite.
     *
     * why is this cache NOT static? because sqlite attaches compiledsql statements to the
     * struct created when {@link SQLiteDatabase#openDatabase(String, CursorFactory, int)} is
     * invoked.
     *
     * this cache has an upper limit of mMaxSqlCacheSize (settable by calling the method
     * (@link setMaxCacheSize(int)}). its default is 0 - i.e., no caching by default because
     * most of the apps don't use "?" syntax in their sql, caching is not useful for them.
     */
//    /* package */ Map<String, SQLiteCompiledSql> mCompiledQueries = Maps.newHashMap();
    /* package */ HashMap<String, AutoPtr<SQLiteCompiledSql> > mCompiledQueries;
    /* package */ Mutex mCompiledQueriesLock;

private:
    const static CString TAG;
    const static Int32 EVENT_DB_OPERATION = 52000;
    const static Int32 EVENT_DB_CORRUPT = 75004;

//    private static final String[] CONFLICT_VALUES = new String[]
//            {"", " OR ROLLBACK ", " OR ABORT ", " OR FAIL ", " OR IGNORE ", " OR REPLACE "};
    const static CString CONFLICT_VALUES[6];

    const static Int32 OPEN_READ_MASK = 0x00000001;         // update native code if changing

    /**
     * Indicates whether the most-recently started transaction has been marked as successful.
     */
    Boolean mInnerTransactionIsSuccessful;

    /**
     * Valid during the life of a transaction, and indicates whether the entire transaction (the
     * outer one and all of the inner ones) so far has been successful.
     */
    Boolean mTransactionIsSuccessful;

    /**
     * Valid during the life of a transaction.
     */
    AutoPtr<ISQLiteTransactionListener> mTransactionListener;

    /** Synchronize on this when accessing the database */
    ReentrantLock mLock;

    Int64 mLockAcquiredWallTime;
    Int64 mLockAcquiredThreadTime;

    // limit the frequency of complaints about each database to one within 20 sec
    // unless run command adb shell setprop log.tag.Database VERBOSE
    const static Int32 LOCK_WARNING_WINDOW_IN_MS = 20000;
    /** If the lock is held this long then a warning will be printed when it is released. */
    const static Int32 LOCK_ACQUIRED_WARNING_TIME_IN_MS = 300;
    const static Int32 LOCK_ACQUIRED_WARNING_THREAD_TIME_IN_MS = 100;
    const static Int32 LOCK_ACQUIRED_WARNING_TIME_IN_MS_ALWAYS_PRINT = 2000;

    const static Int32 SLEEP_AFTER_YIELD_QUANTUM = 1000;

    // The pattern we remove from database filenames before
    // potentially logging them.
//    private static final Pattern EMAIL_IN_DB_PATTERN = Pattern.compile("[\\w\\.\\-]+@[\\w\\.\\-]+");

    Int64 mLastLockMessageTime;

    // Things related to query logging/sampling for debugging
    // slow/frequent queries during development.  Always log queries
    // which take (by default) 500ms+; shorter queries are sampled
    // accordingly.  Commit statements, which are typically slow, are
    // logged together with the most recently executed SQL statement,
    // for disambiguation.  The 500ms value is configurable via a
    // SystemProperty, but developers actively debugging database I/O
    // should probably use the regular log tunable,
    // LOG_SLOW_QUERIES_PROPERTY, defined below.
    const static Int32 sQueryLogTimeInMillis = 0;  // lazily initialized
    const static Int32 QUERY_LOG_SQL_LENGTH = 64;
    const static CString COMMIT_SQL;
//    private final Random mRandom = new Random();
    String mLastSqlStatement;


    /** The path for the database file */
    String mPath;

    /** The anonymized path for the database file for logging purposes */
    String mPathForLogs;  // lazily populated

    /** The flags passed to open/create */
    Int32 mFlags;

    /** The optional factory to use when creating new Cursors */
    AutoPtr<ICursorFactory> mFactory;

//    private WeakHashMap<SQLiteClosable, Object> mPrograms;
    HashMap<AutoPtr<ISQLiteClosable>, AutoPtr<IInterface> > mPrograms;

    Int32 mMaxSqlCacheSize; // max cache size per Database instance
    Int32 mCacheFullWarnings;
    const static Int32 MAX_WARNINGS_ON_CACHESIZE_CONDITION = 1;

    /** maintain stats about number of cache hits and misses */
    Int32 mNumCacheHits;
    Int32 mNumCacheMisses;

    /** the following 2 members maintain the time when a database is opened and closed */
    String mTimeOpened;
    String mTimeClosed;

    /** Used to find out where this object was created in case it never got closed. */
//    private Throwable mStackTrace = null;

    // System property that enables logging of slow queries. Specify the threshold in ms.
    const static CString LOG_SLOW_QUERIES_PROPERTY;
    Int32 mSlowQueryThreshold;

    Boolean mLockingEnabled;

    /** Maps table names to info about what to which _sync_time column to set
     * to NULL on an update. This is used to support syncing. */
//    private final Map<String, SyncUpdateInfo> mSyncUpdateInfo =
//            new HashMap<String, SyncUpdateInfo>();
    HashMap<String, SyncUpdateInfo*> mSyncUpdateInfo;
    Mutex mSyncUpdateInfoLock;

    Mutex mSyncLock;
};
#endif //__SQLITEDATABASE_H__