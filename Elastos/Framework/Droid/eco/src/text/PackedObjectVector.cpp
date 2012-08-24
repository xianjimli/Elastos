
#include "text/PackedObjectVector.h"


PackedObjectVector::PackedObjectVector(
    /* [in] */ Int32 columns)
    : mColumns(columns)
    , mRows(ArrayUtils::IdealInt32ArraySize(0) / mColumns)
    , mRowGapStart(0)
    , mRowGapLength(mRows)
{
    mValues = ArrayOf<IInterface*>::Alloc(mRows * mColumns);
}

PackedObjectVector::~PackedObjectVector()
{
    if (mValues != NULL) {
        for (Int32 i = 0; i < mValues->GetLength(); i++) {
            if ((*mValues)[i] != NULL) (*mValues)[i]->Release();
        }
        ArrayOf<IInterface*>::Free(mValues);
     }
}

AutoPtr<IInterface> PackedObjectVector::GetValue(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column)
{
    if (row >= mRowGapStart) {
        row += mRowGapLength;
    }

    AutoPtr<IInterface> value = (*mValues)[row * mColumns + column];

    return value;
}

void PackedObjectVector::SetValue(
    /* [in] */ Int32 row,
    /* [in] */ Int32 column,
    /* [in] */ IInterface* value)
{
    if (row >= mRowGapStart) {
        row += mRowGapLength;
    }

    (*mValues)[row * mColumns + column] = value;
    if (value != NULL) value->AddRef();
}

void PackedObjectVector::InsertAt(
    /* [in] */ Int32 row,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    MoveRowGapTo(row);

    if (mRowGapLength == 0) {
        GrowBuffer();
    }

    mRowGapStart++;
    mRowGapLength--;

    if (values == NULL) {
        for (Int32 i = 0; i < mColumns; i++) {
            SetValue(row, i, NULL);
        }
    }
    else {
        for (Int32 i = 0; i < mColumns; i++) {
            SetValue(row, i, (*values)[i]);
        }
    }
}

void PackedObjectVector::DeleteAt(
    /* [in] */ Int32 row,
    /* [in] */ Int32 count)
{
    MoveRowGapTo(row + count);

    mRowGapStart -= count;
    mRowGapLength += count;

    if (mRowGapLength > Size() * 2)
    {
        // dump();
        // growBuffer();
    }
}

Int32 PackedObjectVector::Size()
{
    return mRows - mRowGapLength;
}

Int32 PackedObjectVector::Width()
{
    return mColumns;
}

void PackedObjectVector::GrowBuffer()
{
    Int32 newsize = Size() + 1;
    newsize = ArrayUtils::IdealInt32ArraySize(newsize * mColumns) / mColumns;
    ArrayOf<IInterface*>* newvalues = ArrayOf<IInterface*>::Alloc(newsize * mColumns);

    Int32 after = mRows - (mRowGapStart + mRowGapLength);

    for (Int32 i = 0; i < mColumns * mRowGapStart; i++) {
        (*newvalues)[i] = (*mValues)[i];
    }
    for (Int32 i = 0, srcBegin = (mRows - after) * mColumns,
         desBegin = (newsize - after) * mColumns; i < after * mColumns; i++) {
        (*newvalues)[desBegin + i] = (*mValues)[srcBegin + i];
    }

    mRowGapLength += newsize - mRows;
    mRows = newsize;
    ArrayOf<IInterface*>::Free(mValues);
    mValues = newvalues;
}

void PackedObjectVector::MoveRowGapTo(
    /* [in] */ Int32 where)
{
    if (where == mRowGapStart) {
        return;
    }

    if (where > mRowGapStart) {
        Int32 moving = where + mRowGapLength - (mRowGapStart + mRowGapLength);

        for (Int32 i = mRowGapStart + mRowGapLength;
            i < mRowGapStart + mRowGapLength + moving; i++) {

            Int32 destrow = i - (mRowGapStart + mRowGapLength) + mRowGapStart;

            for (Int32 j = 0; j < mColumns; j++) {
                IInterface* val = (*mValues)[i * mColumns + j];

                Int32 index = destrow * mColumns + j;
                if ((*mValues)[index] != NULL) {
                    (*mValues)[index]->Release();
                }
                (*mValues)[index] = val;
                if (val != NULL) val->AddRef();
            }
        }
    }
    else /* where < mRowGapStart */
    {
        Int32 moving = mRowGapStart - where;

        for (Int32 i = where + moving - 1; i >= where; i--){

            Int32 destrow = i - where + mRowGapStart + mRowGapLength - moving;

            for (Int32 j = 0; j < mColumns; j++){
                IInterface* val = (*mValues)[i * mColumns + j];

                Int32 index = destrow * mColumns + j;
                if ((*mValues)[index] != NULL) {
                    (*mValues)[index]->Release();
                }
                (*mValues)[index] = val;
                if (val != NULL) val->AddRef();
            }
        }
    }

    mRowGapStart = where;
}

void PackedObjectVector::Dump()
{
    for (Int32 i = 0; i < mRows; i++)
    {
        for (Int32 j = 0; j < mColumns; j++)
        {
            IInterface* val = (*mValues)[i * mColumns + j];

            //if (i < mRowGapStart || i >= mRowGapStart + mRowGapLength)
            //    //System.out.print(val + " ");
            //else
            //    //System.out.print("(" + val + ") ");
        }

        //System.out.print(" << \n");
    }

    //System.out.print("-----\n\n");
}
