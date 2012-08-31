#ifndef __CURSORJOINER_H__
#define __CURSORJOINER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
/**
 * Does a join on two cursors using the specified columns. The cursors must already
 * be sorted on each of the specified columns in ascending order. This joiner only
 * supports the case where the tuple of key column values is unique.
 */
class CursorJoiner
{
public:
    CursorJoiner();
    ~CursorJoiner();

    /**
     * Initializes the CursorJoiner and resets the cursors to the first row. The left and right
     * column name arrays must have the same number of columns.
     * @param cursorLeft The left cursor to compare
     * @param columnNamesLeft The column names to compare from the left cursor
     * @param cursorRight The right cursor to compare
     * @param columnNamesRight The column names to compare from the right cursor
     */
    CARAPI Init(
        /* [in] */ ICursor* cursorLeft,
        /* [in] */ ArrayOf<String>* columnNamesLeft,
        /* [in] */ ICursor* cursorRight,
        /* [in] */ ArrayOf<String>* columnNamesRight);

//    CARAPI Iterator(
//        /* [out] */ Iterator<Result>* value);

    /**
     * Returns whether or not there are more rows to compare using next().
     * @return true if there are more rows to compare
     */
    CARAPI HasNext(
        /* [out] */ Boolean* rst);

    /**
     * Returns the comparison result of the next row from each cursor. If one cursor
     * has no more rows but the other does then subsequent calls to this will indicate that
     * the remaining rows are unique.
     * <p>
     * The caller must check that hasNext() returns true before calling this.
     * <p>
     * Once next() has been called the cursors specified in the result of the call to
     * next() are guaranteed to point to the row that was indicated. Reading values
     * from the cursor that was not indicated in the call to next() will result in
     * undefined behavior.
     * @return LEFT, if the row pointed to by the left cursor is unique, RIGHT
     *   if the row pointed to by the right cursor is unique, BOTH if the rows in both
     *   cursors are the same.
     */
//    CARAPI Next(
//        /* [out] */ Result value);

    CARAPI Remove();

private:
    /**
     * Lookup the indicies of the each column name and return them in an array.
     * @param cursor the cursor that contains the columns
     * @param columnNames the array of names to lookup
     * @return an array of column indices
     */
//    int[] buildColumnIndiciesArray(Cursor cursor, String[] columnNames);

    /**
     * Reads the strings from the cursor that are specifed in the columnIndicies
     * array and saves them in values beginning at startingIndex, skipping a slot
     * for each value. If columnIndicies has length 3 and startingIndex is 1, the
     * values will be stored in slots 1, 3, and 5.
     * @param values the String[] to populate
     * @param cursor the cursor from which to read
     * @param columnIndicies the indicies of the values to read from the cursor
     * @param startingIndex the slot in which to start storing values, and must be either 0 or 1.
     */
//    static void populateValues(String[] values, Cursor cursor, int[] columnIndicies,
//            int startingIndex);

    /**
     * Increment the cursors past the rows indicated in the most recent call to next().
     * This will only have an affect once per call to next().
     */
    CARAPI IncrementCursors();

    /**
     * Compare the values. Values contains n pairs of strings. If all the pairs of strings match
     * then returns 0. Otherwise returns the comparison result of the first non-matching pair
     * of values, -1 if the first of the pair is less than the second of the pair or 1 if it
     * is greater.
     * @param values the n pairs of values to compare
     * @return -1, 0, or 1 as described above.
     */
//    static int compareStrings(String... values)

public:
    /**
     * The result of a call to next().
     */
    enum Result {
        /** The row currently pointed to by the left cursor is unique */
        RIGHT,
        /** The row currently pointed to by the right cursor is unique */
        LEFT,
        /** The rows pointed to by both cursors are the same */
        BOTH
    };

private:
    AutoPtr<ICursor> mCursorLeft;
    AutoPtr<ICursor> mCursorRight;
    Boolean mCompareResultIsValid;
    Result mCompareResult;
    ArrayOf<Int32>* mColumnsLeft;
    ArrayOf<Int32>* mColumnsRight;
    ArrayOf<String>* mValues;


};
#endif //__CURSORJOINER_H__