#ifndef __MATRIXCURSOR_H__
#define __MATRIXCURSOR_H__

#include "ext/frameworkext.h"
#include "database/AbstractCursor.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>

/**
 * A mutable cursor implementation backed by an array of {@code Object}s. Use
 * {@link #newRow()} to add rows. Automatically expands internal capacity
 * as needed.
 */
class MatrixCursor : public AbstractCursor
{
public:
    /**
     * Builds a row, starting from the left-most column and adding one column
     * value at a time. Follows the same ordering as the column names specified
     * at cursor construction time.
     */
    class RowBuilder
        : public ElRefBase
        , public IRowBuilder
    {
    public:
        RowBuilder(
            /* [in] */ Int32 index,
            /* [in] */ Int32 endIndex,
            /* [in] */ MatrixCursor* owner);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        /**
         * Sets the next column value in this row.
         *
         * @throws CursorIndexOutOfBoundsException if you try to add too many
         *  values
         * @return this builder to support chaining
         */
        CARAPI Add(
            /* [in] */ IInterface* columnValue);

    private:
        Int32 mIndex;
        Int32 mEndIndex;
        MatrixCursor* mOwner;
    };

public:
    virtual ~MatrixCursor();

    /**
     * Adds a new row to the end and returns a builder for that row. Not safe
     * for concurrent use.
     *
     * @return builder which can be used to set the column values for the new
     *  row
     */
    virtual CARAPI_(AutoPtr<IRowBuilder>) NewRow();

    /**
     * Adds a new row to the end with the given column values. Not safe
     * for concurrent use.
     *
     * @throws IllegalArgumentException if {@code columnValues.length !=
     *  columnNames.length}
     * @param columnValues in the same order as the the column names specified
     *  at cursor construction time
     */
    virtual CARAPI AddRow(
        /* [in] */ const ArrayOf<IInterface*>& columnValues);

    /**
     * Adds a new row to the end with the given column values. Not safe
     * for concurrent use.
     *
     * @throws IllegalArgumentException if {@code columnValues.size() !=
     *  columnNames.length}
     * @param columnValues in the same order as the the column names specified
     *  at cursor construction time
     */
    virtual CARAPI AddRow(
        /* [in] */ IObjectContainer* columnValues);

    // AbstractCursor implementation.

    //@Override
    CARAPI_(Int32) GetCount();

    //@Override
    CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String>** names);

    //@Override
    CARAPI GetString(
        /* [in] */ Int32 column,
        /* [out] */ String* str);

    //@Override
    CARAPI GetInt16(
        /* [in] */ Int32 column,
        /* [out] */ Int16* value);

    //@Override
    CARAPI GetInt32(
        /* [in] */ Int32 column,
        /* [out] */ Int32* value);

    //@Override
    CARAPI GetInt64(
        /* [in] */ Int32 column,
        /* [out] */ Int64* value);

    //@Override
    CARAPI GetFloat(
        /* [in] */ Int32 column,
        /* [out] */ Float* value);

    //@Override
    CARAPI GetDouble(
        /* [in] */ Int32 column,
        /* [out] */ Double* value);

    //@Override
    CARAPI IsNull(
        /* [in] */ Int32 column,
        /* [out] */ Boolean* isNull);

protected:
    MatrixCursor();

    /**
     * Constructs a new cursor with the given initial capacity.
     *
     * @param columnNames names of the columns, the ordering of which
     *  determines column ordering elsewhere in this cursor
     * @param initialCapacity in rows
     */
    CARAPI Init(
        /* [in] */ ArrayOf<String>* columnNames,
        /* [in] */ Int32 initialCapacity);

    /**
     * Constructs a new cursor.
     *
     * @param columnNames names of the columns, the ordering of which
     *  determines column ordering elsewhere in this cursor
     */
    CARAPI Init(
        /* [in] */ ArrayOf<String>* columnNames);

private:
    /**
     * Gets value at the given column for the current row.
     */
    CARAPI Get(
        /* [in] */ Int32 column,
        /* [out] */ IInterface** value);

    /** Ensures that this cursor has enough capacity. */
    CARAPI_(void) EnsureCapacity(
        /* [in] */ Int32 size);

private:
    ArrayOf<String>* mColumnNames;
    ArrayOf< AutoPtr<IInterface> >* mData;
    Int32 mRowCount;
    Int32 mColumnCount;
};

#endif //__MATRIXCURSOR_H__
