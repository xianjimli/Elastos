
#ifndef __CCURSORWRAPPERINNER_H__
#define __CCURSORWRAPPERINNER_H__

#include "_CCursorWrapperInner.h"

CarClass(CCursorWrapperInner)
{
public:
    /**
     * Closes the Cursor, releasing all of its resources and making it completely invalid.
     */
    CARAPI Close();

    /**
     * Retrieves the requested column text and stores it in the buffer provided.
     */
    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer* buffer);

    /**
     * Deactivates the Cursor, making all calls on it fail until requery() is called.
     */
    CARAPI Deactivate();

    /**
     * Returns the value of the requested column as a byte array.
     *
     */
    CARAPI GetBlob(
        /* [in] */ Int32 columnIndex,
        /* [out, callee] */ ArrayOf<Byte>** blob);

    /**
     * Return total number of columns.
     */
    CARAPI GetColumnCount(
        /* [out] */ Int32* columnCount);

    /**
     * Returns the zero-based index for the given column name, or -1 if the column doesn't exist.
     */
    CARAPI GetColumnIndex(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* columnIndex);

    /**
     * Returns the column name at the given zero-based column index.
     */
    CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* columnName);

    /**
     * Returns a string array holding the names of all of the columns in the result set in the order in which they were listed in the result.
     */
    CARAPI GetColumnNames(
        /* [out] */ ArrayOf<String>** columnNames);

    /**
     * Returns the numbers of rows in the cursor.
     */
    CARAPI GetCount(
        /* [out] */ Int32* count);

    /**
     * Returns the value of the requested column as a double.
     */
    CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* columnValue);

    /**
     * Returns a bundle of extra values.
     */
    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    /**
     * Returns the value of the requested column as a float.
     */
    CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* columnValue);

    /**
     * Returns the value of the requested column as an int.
     */
    CARAPI GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* columnValue);

    /**
     * Returns the value of the requested column as a long.
     */
    CARAPI GetInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64* columnValue);

    /**
     * Returns the current position of the cursor in the row set.
     */
    CARAPI GetPosition(
        /* [out] */ Int32* position);

    /**
     * Returns the value of the requested column as a short.
     */
    CARAPI GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* columnValue);

    /**
     * Returns the value of the requested column as a String.
     */
    CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* columnValue);

    /**
     * onMove() will only be called across processes if this method returns true.
     */
    CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* value);

    /**
     * Returns whether the cursor is pointing to the position after the last row.
     */
    CARAPI IsAfterLast(
        /* [out] */ Boolean* value);

    /**
     * Returns whether the cursor is pointing to the position before the first row.
     */
    CARAPI IsBeforeFirst(
        /* [out] */ Boolean* value);

    /**
     * return true if the cursor is closed.
     */
    CARAPI IsClosed(
        /* [out] */ Boolean* value);

    /**
     * Returns whether the cursor is pointing to the first row.
     */
    CARAPI IsFirst(
        /* [out] */ Boolean* value);

    /**
     * Returns whether the cursor is pointing to the last row.
     */
    CARAPI IsLast(
        /* [out] */ Boolean* value);

    /**
     * Returns true if the value in the indicated column is null.
     */
    CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value);

    /**
     * Move the cursor by a relative amount, forward or backward, from the current position.
     */
    CARAPI Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* value);

    /**
     * Move the cursor to the first row.
     */
    CARAPI MoveToFirst(
        /* [out] */ Boolean* value);

    /**
     * Move the cursor to the last row.
     */
    CARAPI MoveToLast(
        /* [out] */ Boolean* value);

    /**
     * Move the cursor to the next row.
     */
    CARAPI MoveToNext(
        /* [out] */ Boolean* value);

    /**
     * Move the cursor to an absolute position.
     */
    CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* value);

    /**
     * Move the cursor to the previous row.
     */
    CARAPI MoveToPrevious(
        /* [out] */ Boolean* value);

    /**
     * Register an observer that is called when changes happen to the content backing this cursor.
     */
    CARAPI RegisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    /**
     * Register an observer that is called when changes happen to the contents of the this cursors data set,
     * for example, when the data set is changed via requery(), deactivate(), or close().
     */
    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    /**
     * Performs the query that created the cursor again, refreshing its contents.
     */
    CARAPI Requery(
        /* [out] */ Boolean* value);

    /**
     * Unregister an observer that has previously been registered with
     * this cursor via registerContentObserver(ContentObserver).
     */
    CARAPI UnregisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    /**
     * Unregister an observer that has previously been registered with
     * this cursor via registerContentObserver(ContentObserver).
     */
    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI DeleteRow(
        /* [out] */ Boolean* result);

    CARAPI GetColumnIndexOrThrow(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* columnIndex);

    CARAPI SupportsUpdates(
        /* [out] */ Boolean* result);

    CARAPI HasUpdates(
        /* [out] */ Boolean* result);

    CARAPI UpdateBlob(
        /* [in] */ Int32 columnIndex,
        /* [in] */ const ArrayOf<Byte>& value,
        /* [out] */ Boolean* succeeded);

    CARAPI UpdateString(
        /* [in] */ Int32 columnIndex,
        /* [in] */ const String& value,
        /* [out] */ Boolean* succeeded);

    CARAPI UpdateInt16(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int16 value,
        /* [out] */ Boolean* succeeded);

    CARAPI UpdateInt32(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 value,
        /* [out] */ Boolean* succeeded);

    CARAPI UpdateInt64(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int64 value,
        /* [out] */ Boolean* succeeded);

    CARAPI UpdateFloat(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Float value,
        /* [out] */ Boolean* succeeded);

    CARAPI UpdateDouble(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Double value,
        /* [out] */ Boolean* succeeded);

    CARAPI UpdateToNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* succeeded);

    CARAPI CommitUpdates(
        /* [out] */ Boolean* succeeded);

    CARAPI AbortUpdates();

    CARAPI SetNotificationUri(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** result);

    ECode constructor(
        /* [in] */ ICursor* cursor,
        /* [in] */ IContentProvider* provider);
};

#endif //__CCURSORWRAPPERINNER_H__
