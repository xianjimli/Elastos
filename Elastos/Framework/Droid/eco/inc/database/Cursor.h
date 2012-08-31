#ifndef __CURSOR_H__
#define __CURSOR_H__

#include "ext/frameworkext.h"

class Cursor
{
public:
    virtual CARAPI GetCount(
            /* [out] */ Int32* count) = 0;

    virtual CARAPI GetPosition(
            /* [out] */ Int32* position) = 0;

    virtual CARAPI Move(
            /* [in] */ Int32 offset,
            /* [out] */ Boolean* value) = 0;

    virtual CARAPI MoveToPosition(
            /* [in] */ Int32 position,
            /* [out] */ Boolean* value) = 0;

    virtual CARAPI MoveToFirst(
            /* [out] */ Boolean* value) = 0;

    virtual CARAPI MoveToLast(
            /* [out] */ Boolean* value) = 0;

    virtual CARAPI MoveToNext(
            /* [out] */ Boolean* value);

    /**
     * Move the cursor to the previous row.
     */
    virtual CARAPI MoveToPrevious(
        /* [out] */ Boolean* value) = 0;

    /**
     * Returns whether the cursor is pointing to the first row.
     */
    virtual CARAPI IsFirst(
        /* [out] */ Boolean* value) = 0;

    /**
     * Returns whether the cursor is pointing to the last row.
     */
    virtual CARAPI IsLast(
        /* [out] */ Boolean* value) = 0;

    /**
     * Returns whether the cursor is pointing to the position before the first row.
     */
    virtual CARAPI IsBeforeFirst(
        /* [out] */ Boolean* value) = 0;

    /**
     * Returns whether the cursor is pointing to the position after the last row.
     */
    virtual CARAPI IsAfterLast(
        /* [out] */ Boolean* value) = 0;

//        @Deprecated
//        boolean deleteRow();

    /**
     * Returns the zero-based index for the given column name, or -1 if the column doesn't exist.
     */
    virtual CARAPI GetColumnIndex(
        /* [in] */ String columnName,
        /* [out] */ Int32* columnIndex) = 0;

    virtual CARAPI GetColumnIndexOrThrow(
        /* [in] */  String columnName,
        /* [out] */ Int32* columnIndex) = 0;

    /**
     * Returns the column name at the given zero-based column index.
     */
    virtual CARAPI GetColumnName(
        /* [in] */  Int32 columnIndex,
        /* [out] */ String* columnName) = 0;

    /**
     * Returns a string array holding the names of all of the columns in the result set in the order in which they were listed in the result.
     */
    virtual CARAPI GetColumnNames(
        /* [out,callee] */ ArrayOf<String>** columnNames) = 0;

    /**
     * Return total number of columns.
     */
    virtual CARAPI GetColumnCount(
        /* [out] */ Int32* columnCount) = 0;

    /**
     * Returns the value of the requested column as a byte array.
     *
     */
    virtual CARAPI GetBlob(
        /* [in] */  Int32 columnIndex,
        /* [out,callee] */ ArrayOf<Byte>** blob) = 0;

    /**
     * Returns the value of the requested column as a String.
     */
    virtual CARAPI GetString(
        /* [in] */  Int32 columnIndex,
        /* [out] */ String* columnValue) = 0;

    /**
     * Retrieves the requested column text and stores it in the buffer provided.
     */
//        CopyStringToBuffer(
//            [in] Int32 columnIndex,
//            [out] ICharArrayBuffer** buf);

    /**
     * Returns the value of the requested column as a short.
     */
    virtual CARAPI GetInt16(
        /* [in] */  Int32 columnIndex,
        /* [out] */ Int16* columnValue) = 0;

    /**
     * Returns the value of the requested column as an int.
     */
    virtual CARAPI GetInt32(
        /* [in] */  Int32 columnIndex,
        /* [out] */ Int32* columnValue) = 0;

    /**
     * Returns the value of the requested column as a long.
     */
    virtual CARAPI GetInt64(
        /* [in] */  Int32 columnIndex,
        /* [out] */ Int64* columnValue) = 0;

    /**
     * Returns the value of the requested column as a float.
     */
    virtual CARAPI GetFloat(
        /* [in] */  Int32 columnIndex,
        /* [out] */ Float* columnValue) = 0;

    /**
     * Returns the value of the requested column as a double.
     */
    virtual CARAPI GetDouble(
        /* [in] */  Int32 columnIndex,
        /* [out] */ Double* columnValue) = 0;

    /**
     * Returns true if the value in the indicated column is null.
     */
    virtual CARAPI IsNull(
        /* [in] */  Int32 columnIndex,
        /* [out] */ Boolean* value) = 0;

//        @Deprecated
//        boolean supportsUpdates();

//        @Deprecated
//        boolean hasUpdates();

//        @Deprecated
//        boolean updateBlob(int columnIndex, byte[] value);

//        @Deprecated
//        boolean updateString(int columnIndex, String value);

//        @Deprecated
//        boolean updateShort(int columnIndex, short value);

//        @Deprecated
//        boolean updateInt(int columnIndex, int value);

//        @Deprecated
//        boolean updateLong(int columnIndex, long value);

//        @Deprecated
//        boolean updateFloat(int columnIndex, float value);

//        @Deprecated
//        boolean updateDouble(int columnIndex, double value);

//        @Deprecated
//        boolean updateToNull(int columnIndex);

//        @Deprecated
//        boolean commitUpdates();

//        @Deprecated
//        boolean commitUpdates(Map<? extends Long,
//                ? extends Map<String,Object>> values);

//        @Deprecated
//        void abortUpdates();

    /**
     * Deactivates the Cursor, making all calls on it fail until requery() is called.
     */
    virtual CARAPI Deactivate() = 0;

    /**
     * Performs the query that created the cursor again, refreshing its contents.
     */
    virtual CARAPI Requery(
        /* [out] */ Boolean* value) = 0;

    /**
     * Closes the Cursor, releasing all of its resources and making it completely invalid.
     */
    virtual CARAPI Close() = 0;

    /**
     * return true if the cursor is closed.
     */
    virtual CARAPI IsClosed(
        /* [out] */ Boolean* value) = 0;

    /**
     * Register an observer that is called when changes happen to the content backing this cursor.
     */
    virtual CARAPI RegisterContentObserver(
        /* [in] */  IContentObserver* observer) = 0;

    /**
     * Unregister an observer that has previously been registered with
     * this cursor via registerContentObserver(ContentObserver).
     */
    virtual CARAPI UnregisterContentObserver(
        /* [in] */  IContentObserver* observer) = 0;

    /**
     * Register an observer that is called when changes happen to the contents of the this cursors data set,
     * for example, when the data set is changed via requery(), deactivate(), or close().
     */
    virtual CARAPI RegisterDataSetObserver(
        /* [in] */  IDataSetObserver* observer) = 0;

    /**
     * Unregister an observer that has previously been registered with
     * this cursor via registerContentObserver(ContentObserver).
     */
    virtual CARAPI UnregisterDataSetObserver(
        /* [in] */  IDataSetObserver* observer) = 0;

    virtual CARAPI SetNotificationUri(
        /* [in] */  IContentResolver* cr,
        /* [in] */  IUri* uri) = 0;

    /**
     * onMove() will only be called across processes if this method returns true.
     */
    virtual CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* value) = 0;

    /**
     * Returns a bundle of extra values.
     */
    virtual CARAPI GetExtras(
        /* [out] */ IBundle** extras) = 0;

    virtual CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** res) = 0;
};
#endif //__CURSOR_H__