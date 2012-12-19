
#include "database/CursorJoiner.h"

CursorJoiner::CursorJoiner()
    : mCompareResultIsValid(FALSE)
    , mColumnsLeft(NULL)
    , mColumnsRight(NULL)
    , mValues(NULL)
{
}

CursorJoiner::~CursorJoiner()
{
    if (mColumnsLeft != NULL) {
        ArrayOf<Int32>::Free(mColumnsLeft);
    }
    if (mColumnsRight != NULL) {
        ArrayOf<Int32>::Free(mColumnsRight);
    }
    if (mValues != NULL) {
        for (Int32 i = 0; i < mValues->GetLength(); ++i) {
            (*mValues)[i] = NULL;
        }
        ArrayOf<String>::Free(mValues);
    }
}

ECode CursorJoiner::Init(
    /* [in] */ ICursor* cursorLeft,
    /* [in] */ const ArrayOf<String>& columnNamesLeft,
    /* [in] */ ICursor* cursorRight,
    /* [in] */ const ArrayOf<String>& columnNamesRight)
{
    if(columnNamesLeft.GetLength() != columnNamesRight.GetLength()) {
        // throw new IllegalArgumentException(
        //             "you must have the same number of columns on the left and right, "
        //                     + columnNamesLeft.length + " != " + columnNamesRight.length);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mCursorLeft = cursorLeft;
    mCursorRight = cursorRight;

    Boolean succeeded;
    mCursorLeft->MoveToFirst(&succeeded);
    mCursorRight->MoveToFirst(&succeeded);

    mCompareResultIsValid = FALSE;

    mColumnsLeft = BuildColumnIndiciesArray(cursorLeft, columnNamesLeft);
    mColumnsRight = BuildColumnIndiciesArray(cursorRight, columnNamesRight);

    mValues = ArrayOf<String>::Alloc(mColumnsLeft->GetLength() * 2);
    return NOERROR;
}

// public Iterator<Result> iterator() {
//     return this;
// }

ArrayOf<Int32>* CursorJoiner::BuildColumnIndiciesArray(
    /* [in] */ ICursor* cursor,
    /* [in] */ const ArrayOf<String>& columnNames)
{
    Int32 length = columnNames.GetLength();
    ArrayOf<Int32>* columns = ArrayOf<Int32>::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        ASSERT_SUCCEEDED(cursor->GetColumnIndexOrThrow(
                columnNames[i], &(*columns)[i]));
    }
    return columns;
}

ECode CursorJoiner::HasNext(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

    Boolean r1,r2;
    if(mCompareResultIsValid) {
        switch(mCompareResult) {
            case CursorJoinerResult_BOTH:
                FAIL_RETURN(mCursorLeft->IsLast(&r1));
                FAIL_RETURN(mCursorRight->IsLast(&r2));
                *result = !r1 || !r2;
                return NOERROR;

            case CursorJoinerResult_LEFT:
                FAIL_RETURN(mCursorLeft->IsLast(&r1));
                FAIL_RETURN(mCursorRight->IsAfterLast(&r2));
                *result = !r1 || !r2;
                return NOERROR;

            case CursorJoinerResult_RIGHT:
                FAIL_RETURN(mCursorLeft->IsAfterLast(&r1));
                FAIL_RETURN(mCursorRight->IsLast(&r2));
                *result = !r1 || !r2;
                return NOERROR;

            default:
                // throw new IllegalStateException("bad value for mCompareResult, "
                //             + mCompareResult);
                return E_ILLEGAL_STATE_EXCEPTION;
        }
    }
    else {
        FAIL_RETURN(mCursorLeft->IsAfterLast(&r1));
        FAIL_RETURN(mCursorRight->IsAfterLast(&r2));
        *result = !r1 || !r2;
        return NOERROR;
    }
}

ECode CursorJoiner::GetNext(
    /* [out] */ CursorJoinerResult* result)
{
    Boolean hasNext;
    if (HasNext(&hasNext), !hasNext) {
        // throw new IllegalStateException("you must only call next() when hasNext() is true");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    IncrementCursors();
    HasNext(&hasNext);
    assert(hasNext);

    Boolean isAfterLast;
    mCursorLeft->IsAfterLast(&isAfterLast);
    Boolean hasLeft = !isAfterLast;
    mCursorRight->IsAfterLast(&isAfterLast);
    Boolean hasRight = !isAfterLast;

    if (hasLeft && hasRight) {
        assert(mValues != NULL && mColumnsLeft != NULL && mColumnsRight != NULL);
        PopulateValues(*mValues, mCursorLeft, *mColumnsLeft, 0 /* start filling at index 0 */);
        PopulateValues(*mValues, mCursorRight, *mColumnsRight, 1 /* start filling at index 1 */);
        Int32 result;
        FAIL_RETURN(CompareStrings(*mValues, &result));
        switch (result) {
            case -1:
                mCompareResult = CursorJoinerResult_LEFT;
                break;
            case 0:
                mCompareResult = CursorJoinerResult_BOTH;
                break;
            case 1:
                mCompareResult = CursorJoinerResult_RIGHT;
                break;
        }
    }
    else if (hasLeft) {
        mCompareResult = CursorJoinerResult_LEFT;
    }
    else  {
        assert(hasRight);
        mCompareResult = CursorJoinerResult_RIGHT;
    }
    mCompareResultIsValid = TRUE;
    *result = mCompareResult;
    return NOERROR;
}

ECode CursorJoiner::Remove()
{
//    throw new UnsupportedOperationException("not implemented");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

void CursorJoiner::PopulateValues(
    /* [in] */ ArrayOf<String>& values,
    /* [in] */ ICursor* cursor,
    /* [in] */ const ArrayOf<Int32>& columnIndicies,
    /* [in] */ Int32 startingIndex)
{
    assert(startingIndex == 0 || startingIndex == 1);
    for (Int32 i = 0; i < columnIndicies.GetLength(); i++) {
        String str;
        cursor->GetString(columnIndicies[i], &str);
        values[startingIndex + i * 2] =  str;
    }
}

void CursorJoiner::IncrementCursors()
{
    if(mCompareResultIsValid) {
        Boolean succeeded;
        switch(mCompareResult) {
            case CursorJoinerResult_LEFT:
                ASSERT_SUCCEEDED(mCursorLeft->MoveToNext(&succeeded));
                break;
            case CursorJoinerResult_RIGHT:
                ASSERT_SUCCEEDED(mCursorRight->MoveToNext(&succeeded));
                break;
            case CursorJoinerResult_BOTH:
                ASSERT_SUCCEEDED(mCursorLeft->MoveToNext(&succeeded));
                ASSERT_SUCCEEDED(mCursorRight->MoveToNext(&succeeded));
                break;
        }
        mCompareResultIsValid = FALSE;
    }
}

ECode CursorJoiner::CompareStrings(
    /* [in] */ const ArrayOf<String>& values,
    /* [out] */ Int32* result)
{
    if ((values.GetLength() % 2) != 0) {
        // throw new IllegalArgumentException("you must specify an even number of values");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    for (Int32 index = 0; index < values.GetLength(); index += 2) {
        if (values[index].IsNull()) {
            if (values[index + 1].IsNull()) continue;
            *result = -1;
            return NOERROR;
        }

        if (values[index + 1].IsNull()) {
            *result = 1;
            return NOERROR;
        }

        Int32 comp = values[index].Compare(values[index + 1]);
        if (comp != 0) {
            *result = comp < 0 ? -1 : 1;
            return NOERROR;
        }
    }

    *result = 0;
    return NOERROR;
}
