#include "database/MergeCursor.h"

void MergeCursor::Init()
{
    mObserver = new SubDataSetObserver();
}

MergeCursor::MergeCursor()
{
    Init();
}

MergeCursor::MergeCursor(
        /* [in] */ ArrayOf<AutoPtr<ICursor> >* cursors)
{
    Init();
    mCursors = cursors;
    mCursor = (*cursors)[0];
//    for (Int32 i = 0; i < mCursors->GetLength(); i++) {
//        if ((*mCursors)[i] == NULL) {
//            continue;
//        }
//        (*mCursors)[i]->RegisterDataSetObserver(mObserver);
//    }
}

ECode MergeCursor::GetCount(
        /* [out] */ Int32* cnt)
{
    assert(cnt != NULL);
    Int32 count = 0;
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0; i < length; i++) {
        if ((*mCursors)[i] != NULL) {
            Int32 c;
            (*mCursors)[i]->GetCount(&c);
            count += c;
        }
    }
    *cnt = count;
    return NOERROR;
}

ECode MergeCursor::OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean* value)
{
    assert(value != NULL);
    mCursor = NULL;
    Int32 cursorStartPos = 0;
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0 ; i < length; i++) {
        if ((*mCursors)[i] == NULL) {
            continue;
        }

        Int32 c;
        (*mCursors)[i]->GetCount(&c);

        if (newPosition < (cursorStartPos + c)) {
            mCursor = (*mCursors)[i];
            break;
        }

        cursorStartPos += c;
    }

    /* Move it to the right position */
    if (mCursor != NULL) {
        Boolean ret;
        mCursor->MoveToPosition(newPosition - cursorStartPos, &ret);
        *value = ret;
        return NOERROR;
    }
    *value = FALSE;
    return NOERROR;
}

ECode MergeCursor::GetString(
        /* [in] */ Int32 column,
        /* [out] */ String* value)
{
    mCursor->GetString(column, value);
    return NOERROR;
}

ECode MergeCursor::GetInt16(
        /* [in] */ Int32 column,
        /* [out] */ Int16* value)
{
    mCursor->GetInt16(column, value);
    return NOERROR;
}

ECode MergeCursor::GetInt32(
        /* [in] */ Int32 column,
        /* [out] */ Int32* value)
{
    mCursor->GetInt32(column, value);
    return NOERROR;
}

ECode MergeCursor::GetInt64(
        /* [in] */ Int32 column,
        /* [out] */ Int64* value)
{
    mCursor->GetInt64(column, value);
    return NOERROR;
}

ECode MergeCursor::GetFloat(
        /* [in] */ Int32 column,
        /* [out] */ Float* value)
{
    mCursor->GetFloat(column, value);
    return NOERROR;
}

ECode MergeCursor::GetDouble(
        /* [in] */ Int32 column,
        /* [out] */ Double* value)
{
    mCursor->GetDouble(column, value);
    return NOERROR;
}

ECode MergeCursor::Deactivate()
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0 ; i < length; i++) {
        if ((*mCursors)[i] == NULL) {
            (*mCursors)[i]->Deactivate();
        }
    }
    AbstractCursor::Deactivate();
    return NOERROR;
}

ECode MergeCursor::Close()
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0 ; i < length; i++) {
        if ((*mCursors)[i] == NULL) {
            (*mCursors)[i]->Close();
        }
    }
    AbstractCursor::Close();
    return NOERROR;
}

ECode MergeCursor::Requery(
        /* [out] */ Boolean* value)
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0 ; i < length; i++) {
        if ((*mCursors)[i] == NULL) {
            continue;
        }
        Boolean v;
        (*mCursors)[i]->Requery(&v);
        if (v == FALSE) {
            *value = FALSE;
            return NOERROR;
        }
    }
    *value = TRUE;
    return NOERROR;
}