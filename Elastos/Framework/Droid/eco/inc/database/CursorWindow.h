#ifndef __CURSORWINDOW_H__
#define __CURSORWINDOW_H__

#include "ext/frameworkext.h"
#include "database/sqlite/SQLiteClosable.h"
#include <elastos/AutoPtr.h>
/**
 * A buffer containing multiple cursor rows.
 */
class CursorWindow : public SQLiteClosable
{
private:
    CursorWindow(
        /* [in] */ IParcel* source);

protected:
    CARAPI OnAllReferencesReleased();

public:
    CursorWindow();

    ~CursorWindow();

    /**
     * Creates a new empty window.
     *
     * @param localWindow true if this window will be used in this process only
     */
    CursorWindow(
        /* [in] */ Boolean localWindow);

    /**
     * Returns the starting position of this window within the entire
     * Cursor's result set.
     *
     * @return the starting position of this window within the entire
     * Cursor's result set.
     */
    virtual CARAPI GetStartPosition(
        /* [out] */ Int32* pos);

    /**
     * Set the start position of cursor window
     * @param pos
     */
    virtual CARAPI SetStartPosition(
        /* [in] */ Int32 pos);

    /**
     * Returns the number of rows in this window.
     *
     * @return the number of rows in this window.
     */
    virtual CARAPI GetNumRows(
        /* [out] */ Int32* num);

    /**
     * Set number of Columns
     * @param columnNum
     * @return true if success
     */
    virtual CARAPI SetNumColumns(
        /* [in] */ Int32 columnNum,
        /* [out] */ Boolean* rst);

    /**
     * Allocate a row in cursor window
     * @return false if cursor window is out of memory
     */
    virtual CARAPI AllocRow(
        /* [out] */ Boolean* rst);

    /**
     * Free the last row
     */
    virtual CARAPI FreeLastRow();

    /**
     * copy byte array to cursor window
     * @param value
     * @param row
     * @param col
     * @return false if fail to copy
     */
    virtual CARAPI PutBlob(
        /* [in] */ ArrayOf<Byte> value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Copy String to cursor window
     * @param value
     * @param row
     * @param col
     * @return false if fail to copy
     */
    virtual CARAPI PutString(
        /* [in] */ String value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Copy integer to cursor window
     * @param value
     * @param row
     * @param col
     * @return false if fail to copy
     */
    virtual CARAPI PutInt64(
        /* [in] */ Int64 value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Copy double to cursor window
     * @param value
     * @param row
     * @param col
     * @return false if fail to copy
     */
    virtual CARAPI PutDouble(
        /* [in] */ Double value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Set the [row, col] value to NULL
     * @param row
     * @param col
     * @return false if fail to copy
     */
    virtual CARAPI PutNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Returns {@code true} if given field is {@code NULL}.
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return {@code true} if given field is {@code NULL}
     */
    virtual CARAPI IsNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Returns a byte array for the given field.
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return a String value for the given field
     */
    virtual CARAPI GetBlob(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ ArrayOf<Byte>** blob);

    /**
     * Checks if a field contains either a blob or is null.
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return {@code true} if given field is {@code NULL} or a blob
     */
    virtual CARAPI IsBlob(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Checks if a field contains a long
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return {@code true} if given field is a long
     */
    virtual CARAPI IsInt64(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Checks if a field contains a float.
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return {@code true} if given field is a float
     */
    virtual CARAPI IsFloat(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Checks if a field contains either a String or is null.
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return {@code true} if given field is {@code NULL} or a String
     */
    virtual CARAPI IsString(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst);

    /**
     * Returns a String for the given field.
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return a String value for the given field
     */
    virtual CARAPI GetString(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ String* rst);

    /**
     * copy the text for the given field in the provided char array.
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @param buffer the CharArrayBuffer to copy the text into,
     * If the requested string is larger than the buffer
     * a new char buffer will be created to hold the string. and assigne to
     * CharArrayBuffer.data
      */
    virtual CARAPI CopyStringToBuffer(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ ICharArrayBuffer** buffer);

    /**
     * Returns a long for the given field.
     * row is 0 based
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return a long value for the given field
     */
    virtual CARAPI GetInt64(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int64* value);

    /**
     * Returns a double for the given field.
     * row is 0 based
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return a double value for the given field
     */
    virtual CARAPI GetDouble(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Double* value);

    /**
     * Returns a short for the given field.
     * row is 0 based
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return a short value for the given field
     */
    virtual CARAPI GetInt16(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int16* value);

    /**
     * Returns an int for the given field.
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return an int value for the given field
     */
    virtual CARAPI GetInt32(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Int32* value);

    /**
     * Returns a float for the given field.
     * row is 0 based
     *
     * @param row the row to read from, row - getStartPosition() being the actual row in the window
     * @param col the column to read from
     * @return a float value for the given field
     */
    virtual CARAPI GetFloat(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Float* value);

    /**
     * Clears out the existing contents of the window, making it safe to reuse
     * for new data. Note that the number of columns in the window may NOT
     * change across a call to clear().
     */
    virtual CARAPI Clear();

    /**
     * Cleans up the native resources associated with the window.
     */
    virtual CARAPI Close();

    static CARAPI NewFromParcel(
        /* [in] */ IParcel* p,
        /* [out] */ ICursorWindow** cw);

    virtual CARAPI DescribeContents(
        /* [out] */ Int32* value);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest,
        /* [in] */ Int32 flags);

private:
    Int32 nWindow;

    Int32 mStartPos;
};
#endif //__CURSORWINDOW_H__