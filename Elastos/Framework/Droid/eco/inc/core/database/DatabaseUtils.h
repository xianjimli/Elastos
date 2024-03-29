#ifndef __DATABASEUTILS_H__
#define __DATABASEUTILS_H__

#include "ext/frameworkext.h"
#include "text/TextUtils.h"
#include "utils/Config.h"
#include <StringBuffer.h>
#include <elastos/AutoPtr.h>
#include <elastos/HashMap.h>

/**
 * Static utility methods for dealing with databases and {@link Cursor}s.
 */
class DatabaseUtils
{
public:
    /**
     * This class allows users to do multiple inserts into a table but
     * compile the SQL insert statement only once, which may increase
     * performance.
     */
    class InsertHelper
    {
    public:
        /**
         * @param db the SQLiteDatabase to insert into
         * @param tableName the name of the table to insert into
         */
        InsertHelper(
            /* [in] */ ISQLiteDatabase* db,
            /* [in] */ const String& tableName);

        ~InsertHelper();

        /**
         * Returns the index of the specified column. This is index is suitagble for use
         * in calls to bind().
         * @param key the column name
         * @return the index of the column
         */
        CARAPI GetColumnIndex(
            /* [in] */ const String& key,
            /* [out] */ Int32* columnIndex);

        /**
         * Bind the value to an index. A prepareForInsert() or prepareForReplace()
         * without a matching execute() must have already have been called.
         * @param index the index of the slot to which to bind
         * @param value the value to bind
         */
        CARAPI Bind(
            /* [in] */ Int32 index,
            /* [in] */ Double value);

        /**
         * Bind the value to an index. A prepareForInsert() or prepareForReplace()
         * without a matching execute() must have already have been called.
         * @param index the index of the slot to which to bind
         * @param value the value to bind
         */
        CARAPI Bind(
            /* [in] */ Int32 index,
            /* [in] */ Float value);

        /**
         * Bind the value to an index. A prepareForInsert() or prepareForReplace()
         * without a matching execute() must have already have been called.
         * @param index the index of the slot to which to bind
         * @param value the value to bind
         */
        CARAPI Bind(
            /* [in] */ Int32 index,
            /* [in] */ Int64 value);

        /**
         * Bind the value to an index. A prepareForInsert() or prepareForReplace()
         * without a matching execute() must have already have been called.
         * @param index the index of the slot to which to bind
         * @param value the value to bind
         */
        CARAPI Bind(
            /* [in] */ Int32 index,
            /* [in] */ Int32 value);

        /**
         * Bind the value to an index. A prepareForInsert() or prepareForReplace()
         * without a matching execute() must have already have been called.
         * @param index the index of the slot to which to bind
         * @param value the value to bind
         */
        CARAPI Bind(
            /* [in] */ Int32 index,
            /* [in] */ Boolean value);

        /**
         * Bind null to an index. A prepareForInsert() or prepareForReplace()
         * without a matching execute() must have already have been called.
         * @param index the index of the slot to which to bind
         */
        CARAPI BindNull(
            /* [in] */ Int32 index);

        /**
         * Bind the value to an index. A prepareForInsert() or prepareForReplace()
         * without a matching execute() must have already have been called.
         * @param index the index of the slot to which to bind
         * @param value the value to bind
         */
        CARAPI Bind(
            /* [in] */ Int32 index,
            /* [in] */ ArrayOf<Byte>* value);

        /**
         * Bind the value to an index. A prepareForInsert() or prepareForReplace()
         * without a matching execute() must have already have been called.
         * @param index the index of the slot to which to bind
         * @param value the value to bind
         */
        CARAPI Bind(
            /* [in] */ Int32 index,
            /* [in] */ const String& value);

        /**
         * Performs an insert, adding a new row with the given values.
         * If the table contains conflicting rows, an error is
         * returned.
         *
         * @param values the set of values with which to populate the
         * new row
         *
         * @return the row ID of the newly inserted row, or -1 if an
         * error occurred
         */
        CARAPI Insert(
            /* [in] */ IContentValues* values,
            /* [out] */ Int64* rowId);


        /**
         * Execute the previously prepared insert or replace using the bound values
         * since the last call to prepareForInsert or prepareForReplace.
         *
         * <p>Note that calling bind() and then execute() is not thread-safe. The only thread-safe
         * way to use this class is to call insert() or replace().
         *
         * @return the row ID of the newly inserted row, or -1 if an
         * error occurred
         */
        CARAPI Execute(
            /* [out] */ Int64* value);

        /**
         * Prepare the InsertHelper for an insert. The pattern for this is:
         * <ul>
         * <li>prepareForInsert()
         * <li>bind(index, value);
         * <li>bind(index, value);
         * <li>...
         * <li>bind(index, value);
         * <li>execute();
         * </ul>
         */
        CARAPI PrepareForInsert();

        /**
         * Prepare the InsertHelper for a replace. The pattern for this is:
         * <ul>
         * <li>prepareForReplace()
         * <li>bind(index, value);
         * <li>bind(index, value);
         * <li>...
         * <li>bind(index, value);
         * <li>execute();
         * </ul>
         */
        CARAPI PrepareForReplace();

        /**
         * Performs an insert, adding a new row with the given values.
         * If the table contains conflicting rows, they are deleted
         * and replaced with the new row.
         *
         * @param values the set of values with which to populate the
         * new row
         *
         * @return the row ID of the newly inserted row, or -1 if an
         * error occurred
         */
        CARAPI Replace(
            /* [in] */ IContentValues* values,
            /* [out] */ Int64* rowId);

        /**
         * Close this object and release any resources associated with
         * it.  The behavior of calling <code>insert()</code> after
         * calling this method is undefined.
         */
        CARAPI Close();

    private:
        CARAPI BuildSQL();

        CARAPI GetStatement(
            /* [in] */ Boolean allowReplace,
            /* [out] */ ISQLiteStatement** statement);

        /**
         * Performs an insert, adding a new row with the given values.
         *
         * @param values the set of values with which  to populate the
         * new row
         * @param allowReplace if true, the statement does "INSERT OR
         *   REPLACE" instead of "INSERT", silently deleting any
         *   previously existing rows that would cause a conflict
         *
         * @return the row ID of the newly inserted row, or -1 if an
         * error occurred
         */
        CARAPI_(Int64) InsertInternal(
            /* [in] */ IContentValues* values,
            /* [in] */ Boolean allowReplace);

    public:
        /**
         * {@hide}
         *
         * These are the columns returned by sqlite's "PRAGMA
         * table_info(...)" command that we depend on.
         */
        static const Int32 TABLE_INFO_PRAGMA_COLUMNNAME_INDEX = 1;
        static const Int32 TABLE_INFO_PRAGMA_DEFAULT_INDEX = 4;

    private:
        AutoPtr<ISQLiteDatabase> mDb;
        String mTableName;
        HashMap<String, Int32 >* mColumns;
        String mInsertSQL;
        AutoPtr<ISQLiteStatement> mInsertStatement;
        AutoPtr<ISQLiteStatement> mReplaceStatement;
        AutoPtr<ISQLiteStatement> mPreparedStatement;

        Mutex mLock;
    };

public:
    /**
     * Special function for writing an exception result at the header of
     * a parcel, to be used when returning an exception from a transaction.
     * exception will be re-thrown by the function in another process
     * @param reply Parcel to write to
     * @param e The Exception to be written.
     * @see Parcel#writeNoException
     * @see Parcel#writeException
     */
    static CARAPI_(void) WriteExceptionToParcel(
        /* [in] */ IParcel* reply,
        /* [in] */ ECode e);

    /**
     * Special function for reading an exception result from the header of
     * a parcel, to be used after receiving the result of a transaction.  This
     * will throw the exception for you if it had been written to the Parcel,
     * otherwise return and let you read the normal result data from the Parcel.
     * @param reply Parcel to read from
     * @see Parcel#writeNoException
     * @see Parcel#readException
     */
    static CARAPI ReadExceptionFromParcel(
        /* [in] */ IParcel* reply);

    static CARAPI ReadExceptionWithFileNotFoundExceptionFromParcel(
        /* [in] */ IParcel* reply);

    static CARAPI ReadExceptionWithOperationApplicationExceptionFromParcel(
        /* [in] */ IParcel* reply);

    /**
     * Binds the given Object to the given SQLiteProgram using the proper
     * typing. For example, bind numbers as longs/doubles, and everything else
     * as a string by call toString() on it.
     *
     * @param prog the program to bind the object to
     * @param index the 1-based index to bind at
     * @param value the value to bind
     */
    static CARAPI BindObjectToProgram(
        /* [in] */ ISQLiteProgram* prog,
        /* [in] */ Int32 index,
        /* [in] */ IInterface* value);

    /**
     * Appends an SQL string to the given StringBuilder, including the opening
     * and closing single quotes. Any single quotes internal to sqlString will
     * be escaped.
     *
     * This method is deprecated because we want to encourage everyone
     * to use the "?" binding form.  However, when implementing a
     * ContentProvider, one may want to add WHERE clauses that were
     * not provided by the caller.  Since "?" is a positional form,
     * using it in this case could break the caller because the
     * indexes would be shifted to accomodate the ContentProvider's
     * internal bindings.  In that case, it may be necessary to
     * construct a WHERE clause manually.  This method is useful for
     * those cases.
     *
     * @param sb the StringBuilder that the SQL string will be appended to
     * @param sqlString the raw string to be appended, which may contain single
     *                  quotes
     */
    static CARAPI_(void) AppendEscapedSQLString(
        /* [in] */ StringBuffer& sb,
        /* [in] */ const String& sqlString);

    /**
     * SQL-escape a string.
     */
    static CARAPI_(String) SqlEscapeString(
        /* [in] */ const String& value);

    /**
     * Appends an Object to an SQL string with the proper escaping, etc.
     */
    static const CARAPI_(void) AppendValueToSql(
        /* [in] */ StringBuffer& sql,
        /* [in] */ IInterface* value);

    /**
     * Concatenates two SQL WHERE clauses, handling empty or null values.
     * @hide
     */
    static CARAPI_(String) ConcatenateWhere(
        /* [in] */ const String& a,
        /* [in] */ const String& b);

    /**
     * return the collation key
     * @param name
     * @return the collation key
     */
    static CARAPI_(String) GetCollationKey(
        /* [in] */ const String& name);

    /**
     * return the collation key in hex format
     * @param name
     * @return the collation key in hex format
     */
    static CARAPI_(String) GetHexCollationKey(
        /* [in] */ const String& name);

    /**
     * Prints the contents of a Cursor to System.out. The position is restored
     * after printing.
     *
     * @param cursor the cursor to print
     */
    static CARAPI_(void) DumpCursor(
        /* [in] */ ICursor* cursor);

    /**
     * Prints the contents of a Cursor to a PrintSteam. The position is restored
     * after printing.
     *
     * @param cursor the cursor to print
     * @param stream the stream to print to
     */
//    static CARAPI DumpCursor(
//        /* [in] */ ICursor* cursor,
//        /* [in] */ PrintStream stream);

    /**
     * Prints the contents of a Cursor to a StringBuilder. The position
     * is restored after printing.
     *
     * @param cursor the cursor to print
     * @param sb the StringBuilder to print to
     */
    static CARAPI_(void) DumpCursor(
        /* [in] */ ICursor* cursor,
        /* [in] */ StringBuffer& sb);

    /**
     * Prints the contents of a Cursor to a String. The position is restored
     * after printing.
     *
     * @param cursor the cursor to print
     * @return a String that contains the dumped cursor
     */
    static CARAPI_(String) DumpCursorToString(
        /* [in] */ ICursor* cursor);

    /**
     * Prints the contents of a Cursor's current row to System.out.
     *
     * @param cursor the cursor to print from
     */
    static CARAPI_(void) DumpCurrentRow(
        /* [in] */ ICursor* cursor);

    /**
     * Prints the contents of a Cursor's current row to a PrintSteam.
     *
     * @param cursor the cursor to print
     * @param stream the stream to print to
     */
//    static CARAPI DumpCurrentRow(
//        /* [in] */ ICursor* cursor,
//        /* [in] */ PrintStream stream);

    /**
     * Prints the contents of a Cursor's current row to a StringBuilder.
     *
     * @param cursor the cursor to print
     * @param sb the StringBuilder to print to
     */
    static CARAPI_(void) DumpCurrentRow(
        /* [in] */ ICursor* cursor,
        /* [in] */ StringBuffer& sb);

    /**
     * Dump the contents of a Cursor's current row to a String.
     *
     * @param cursor the cursor to print
     * @return a String that contains the dumped cursor row
     */
    static CARAPI_(String) DumpCurrentRowToString(
        /* [in] */ ICursor* cursor);

    /**
     * Reads a String out of a field in a Cursor and writes it to a Map.
     *
     * @param cursor The cursor to read from
     * @param field The TEXT field to read
     * @param values The {@link ContentValues} to put the value into, with the field as the key
     */
    static CARAPI_(void) CursorStringToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ IContentValues* values);

    /**
     * Reads a String out of a field in a Cursor and writes it to an InsertHelper.
     *
     * @param cursor The cursor to read from
     * @param field The TEXT field to read
     * @param inserter The InsertHelper to bind into
     * @param index the index of the bind entry in the InsertHelper
     */
    static CARAPI_(void) CursorStringToInsertHelper(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ InsertHelper* inserter,
        /* [in] */ Int32 index);

    /**
     * Reads a String out of a field in a Cursor and writes it to a Map.
     *
     * @param cursor The cursor to read from
     * @param field The TEXT field to read
     * @param values The {@link ContentValues} to put the value into, with the field as the key
     * @param key The key to store the value with in the map
     */
    static CARAPI_(void) CursorStringToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& key);

    /**
     * Reads an Integer out of a field in a Cursor and writes it to a Map.
     *
     * @param cursor The cursor to read from
     * @param field The INTEGER field to read
     * @param values The {@link ContentValues} to put the value into, with the field as the key
     */
    static CARAPI_(void) CursorInt32ToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ IContentValues* values);

    /**
     * Reads a Integer out of a field in a Cursor and writes it to a Map.
     *
     * @param cursor The cursor to read from
     * @param field The INTEGER field to read
     * @param values The {@link ContentValues} to put the value into, with the field as the key
     * @param key The key to store the value with in the map
     */
    static CARAPI_(void) CursorInt32ToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& key);

    /**
     * Reads a Long out of a field in a Cursor and writes it to a Map.
     *
     * @param cursor The cursor to read from
     * @param field The INTEGER field to read
     * @param values The {@link ContentValues} to put the value into, with the field as the key
     */
    static CARAPI_(void) CursorInt64ToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ IContentValues* values);

    /**
     * Reads a Long out of a field in a Cursor and writes it to a Map.
     *
     * @param cursor The cursor to read from
     * @param field The INTEGER field to read
     * @param values The {@link ContentValues} to put the value into
     * @param key The key to store the value with in the map
     */
    static CARAPI_(void) CursorInt64ToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& key);

    /**
     * Reads a Double out of a field in a Cursor and writes it to a Map.
     *
     * @param cursor The cursor to read from
     * @param field The REAL field to read
     * @param values The {@link ContentValues} to put the value into
     */
    static CARAPI_(void) CursorDoubleToCursorValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ IContentValues* values);

    /**
     * Reads a Double out of a field in a Cursor and writes it to a Map.
     *
     * @param cursor The cursor to read from
     * @param field The REAL field to read
     * @param values The {@link ContentValues} to put the value into
     * @param key The key to store the value with in the map
     */
    static CARAPI_(void) CursorDoubleToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ const String& field,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& key);

    /**
     * Read the entire contents of a cursor row and store them in a ContentValues.
     *
     * @param cursor the cursor to read from.
     * @param values the {@link ContentValues} to put the row into.
     */
    static CARAPI_(void) CursorRowToContentValues(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values);

    /**
     * Query the table for the number of rows in the table.
     * @param db the database the table is in
     * @param table the name of the table to query
     * @return the number of rows in the table
     */
    static CARAPI QueryNumEntries(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& table,
        /* [out] */ Int64* numEntries);

    /**
     * Utility method to run the query on the db and return the value in the
     * first column of the first row.
     */
    static CARAPI Int64ForQuery(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& query,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ Int64* value);

    /**
     * Utility method to run the pre-compiled query and return the value in the
     * first column of the first row.
     */
    static CARAPI Int64ForQuery(
        /* [in] */ ISQLiteStatement* prog,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ Int64* value);

    /**
     * Utility method to run the query on the db and return the value in the
     * first column of the first row.
     */
    static CARAPI StringForQuery(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ const String& query,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ String* value);

    /**
     * Utility method to run the pre-compiled query and return the value in the
     * first column of the first row.
     */
    static CARAPI StringForQuery(
        /* [in] */ ISQLiteStatement* prog,
        /* [in] */ ArrayOf<String>* selectionArgs,
        /* [out] */ String* value);

    /**
     * Reads a String out of a column in a Cursor and writes it to a ContentValues.
     * Adds nothing to the ContentValues if the column isn't present or if its value is null.
     *
     * @param cursor The cursor to read from
     * @param column The column to read
     * @param values The {@link ContentValues} to put the value into
     */
    static CARAPI_(void) CursorStringToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& column);

    /**
     * Reads a Long out of a column in a Cursor and writes it to a ContentValues.
     * Adds nothing to the ContentValues if the column isn't present or if its value is null.
     *
     * @param cursor The cursor to read from
     * @param column The column to read
     * @param values The {@link ContentValues} to put the value into
     */
    static CARAPI_(void) CursorInt64ToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& column);

    /**
     * Reads a Short out of a column in a Cursor and writes it to a ContentValues.
     * Adds nothing to the ContentValues if the column isn't present or if its value is null.
     *
     * @param cursor The cursor to read from
     * @param column The column to read
     * @param values The {@link ContentValues} to put the value into
     */
    static CARAPI_(void) CursorInt16ToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& column);

    /**
     * Reads a Integer out of a column in a Cursor and writes it to a ContentValues.
     * Adds nothing to the ContentValues if the column isn't present or if its value is null.
     *
     * @param cursor The cursor to read from
     * @param column The column to read
     * @param values The {@link ContentValues} to put the value into
     */
    static CARAPI_(void) CursorInt32ToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& column);

    /**
     * Reads a Float out of a column in a Cursor and writes it to a ContentValues.
     * Adds nothing to the ContentValues if the column isn't present or if its value is null.
     *
     * @param cursor The cursor to read from
     * @param column The column to read
     * @param values The {@link ContentValues} to put the value into
     */
    static CARAPI_(void) CursorFloatToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& column);

    /**
     * Reads a Double out of a column in a Cursor and writes it to a ContentValues.
     * Adds nothing to the ContentValues if the column isn't present or if its value is null.
     *
     * @param cursor The cursor to read from
     * @param column The column to read
     * @param values The {@link ContentValues} to put the value into
     */
    static CARAPI_(void) CursorDoubleToContentValuesIfPresent(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentValues* values,
        /* [in] */ const String& column);

    /**
     * Creates a db and populates it with the sql statements in sqlStatements.
     *
     * @param context the context to use to create the db
     * @param dbName the name of the db to create
     * @param dbVersion the version to set on the db
     * @param sqlStatements the statements to use to populate the db. This should be a single string
     *   of the form returned by sqlite3's <tt>.dump</tt> command (statements separated by
     *   semicolons)
     */
    static CARAPI CreateDbFromSqlStatements(
            /* [in] */ IContext* context,
            /* [in] */ const String& dbName,
            /* [in] */ Int32 dbVersion,
            /* [in] */ const String& sqlStatements);

private:
    static CARAPI ReadExceptionFromParcel(
        /* [in] */ IParcel* reply,
        /* [in] */ const String& msg,
        /* [in] */ Int32 code);

    static CARAPI_(Int32) GetKeyLen(
        /* [in] */ const ArrayOf<Byte>& arr);

    static CARAPI_(ArrayOf<Byte>*) GetCollationKeyInBytes(
        /* [in] */ const String& name);

private:
    static const CString TAG;

    static const Boolean DEBUG = FALSE;
    static const Boolean LOCAL_LOGV = DEBUG ? Config::LOGD : Config::LOGV;

    static const ArrayOf<CString>* sCountProjection;// = new String[]{"count(*)"};

    //static Collator mColl = null;

    // static const Char8 DIGITS[16];
};

#endif //__DATABASEUTILS_H__
