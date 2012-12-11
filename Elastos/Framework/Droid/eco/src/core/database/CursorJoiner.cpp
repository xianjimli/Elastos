
#include "database/CursorJoiner.h"

CursorJoiner::CursorJoiner()
{
}

CursorJoiner::~CursorJoiner()
{
}

ECode CursorJoiner::Init(
        /* [in] */ ICursor* cursorLeft,
        /* [in] */ const ArrayOf<String>& columnNamesLeft,
        /* [in] */ ICursor* cursorRight,
        /* [in] */ const ArrayOf<String>& columnNamesRight)
{
    if(columnNamesLeft.GetLength() != columnNamesRight.GetLength()) {
        //return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCursorLeft = cursorLeft;
    mCursorRight = cursorRight;

    Boolean v1,v2;
    mCursorLeft->MoveToFirst(&v1);
    mCursorRight->MoveToFirst(&v2);

    mCompareResultIsValid = FALSE;
    BuildColumnIndiciesArray(cursorLeft, &columnNamesLeft, &mColumnsLeft);
    BuildColumnIndiciesArray(cursorRight, &columnNamesRight, &mColumnsRight);
    mValues = ArrayOf<String>::Alloc(mColumnsLeft->GetLength() * 2);
    return NOERROR;
}

ECode CursorJoiner::BuildColumnIndiciesArray(
        /* [in] */ ICursor* cursor,
        /* [in] */ const ArrayOf<String>* columnNames,
        /* [out] */ ArrayOf<Int32>** columns)
{
    Int32 length = columnNames->GetLength();

    ArrayOf<Int32>* columnsvalue = ArrayOf<Int32>::Alloc(length);
    for (Int32 i = 0; i < length; i++) {
        cursor->GetColumnIndexOrThrow((*columnNames)[i], &(*columnsvalue)[i]);
    }
    columns = &columnsvalue;
    return NOERROR;
}

ECode CursorJoiner::HasNext(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    Boolean v1,v2;
    if(mCompareResultIsValid) {
        switch(mCompareResult) {
            case BOTH:
                FAIL_RETURN(mCursorLeft->IsLast(&v1));
                FAIL_RETURN(mCursorRight->IsLast(&v2));
                *rst = !v1 || !v2;
                return NOERROR;
            case LEFT:
                FAIL_RETURN(mCursorLeft->IsLast(&v1));
                FAIL_RETURN(mCursorRight->IsAfterLast(&v2));
                *rst = !v1 || !v2;
                return NOERROR;
            case RIGHT:
                FAIL_RETURN(mCursorLeft->IsAfterLast(&v1));
                FAIL_RETURN(mCursorRight->IsLast(&v2));
                *rst = !v1 || !v2;
                return NOERROR;
        }
    } else {
        FAIL_RETURN(mCursorLeft->IsAfterLast(&v1));
        FAIL_RETURN(mCursorRight->IsAfterLast(&v2));
        *rst = !v1 || !v2;
        return NOERROR;
    }
    return NOERROR;
}

ECode CursorJoiner::Remove()
{
//    throw new UnsupportedOperationException("not implemented");
    return E_NOT_SUPPORTED;
}

ECode CursorJoiner::PopulateValues(
        /* [in] */ ArrayOf<String>* values, 
        /* [in] */ ICursor* cursor, 
        /* [in] */ ArrayOf<Int32>* columnIndicies,
        /* [in] */ Int32 startingIndex)
{
    assert(startingIndex == 0 || startingIndex == 1);
    Int32 length = columnIndicies->GetLength();
    for (Int32 i = 0; i < length; i++) {
        cursor->GetString((*columnIndicies)[i], &(*values)[startingIndex + i * 2]);
    }
    return NOERROR;
}

ECode CursorJoiner::IncrementCursors()
{
    if(mCompareResultIsValid) {
        Boolean v1, v2;
        switch(mCompareResult) {
            case LEFT:
                FAIL_RETURN(mCursorLeft->MoveToNext(&v1));
                break;
            case RIGHT:
                FAIL_RETURN(mCursorRight->MoveToNext(&v1));
                break;
            case BOTH:
                FAIL_RETURN(mCursorLeft->MoveToNext(&v1));
                FAIL_RETURN(mCursorRight->MoveToNext(&v2));
                break;
        }
        mCompareResultIsValid = FALSE;
        return NOERROR;
    }
    return NOERROR;
}
