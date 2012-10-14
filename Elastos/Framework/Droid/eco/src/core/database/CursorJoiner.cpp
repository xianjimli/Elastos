
#include "database/CursorJoiner.h"

CursorJoiner::CursorJoiner()
{
}

CursorJoiner::~CursorJoiner()
{
}

ECode CursorJoiner::Init(
        /* [in] */ ICursor* cursorLeft,
        /* [in] */ ArrayOf<String>* columnNamesLeft,
        /* [in] */ ICursor* cursorRight,
        /* [in] */ ArrayOf<String>* columnNamesRight)
{
    if(columnNamesLeft->GetLength() != columnNamesRight->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCursorLeft = cursorLeft;
    mCursorRight = cursorRight;

    Boolean v1,v2;
    FAIL_RETURN(mCursorLeft->MoveToFirst(&v1));
    FAIL_RETURN(mCursorRight->MoveToFirst(&v2));

    mCompareResultIsValid = FALSE;
//    mColumnsLeft = buildColumnIndiciesArray(cursorLeft, columnNamesLeft);
//    mColumnsRight = buildColumnIndiciesArray(cursorRight, columnNamesRight);
    mValues = ArrayOf<String>::Alloc(mColumnsLeft->GetLength() * 2);
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
