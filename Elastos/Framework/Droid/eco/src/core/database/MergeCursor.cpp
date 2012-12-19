#include "database/MergeCursor.h"


MergeCursor::MyDataSetObserver::MyDataSetObserver()
{}

MergeCursor::MyDataSetObserver::~MyDataSetObserver()
{}

PInterface MergeCursor::MyDataSetObserver::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IDataSetObserver*)this;
    }
    else if (riid == EIID_IDataSetObserver) {
        return (IDataSetObserver*)this;
    }

    return NULL;
}

UInt32 MergeCursor::MyDataSetObserver::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MergeCursor::MyDataSetObserver::Release()
{
    return ElRefBase::Release();
}

ECode MergeCursor::MyDataSetObserver::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IDataSetObserver*)this) {
        *pIID = EIID_IDataSetObserver;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MergeCursor::MyDataSetObserver::OnChanged()
{
    //mPos = -1;
    return NOERROR;
}

ECode MergeCursor::MyDataSetObserver::OnInvalidated()
{
    //mPos = -1;
    return NOERROR;
}


MergeCursor::MergeCursor()
    : mCursors(NULL)
{
    mObserver = new MyDataSetObserver();
}

MergeCursor::~MergeCursor()
{
    if (mCursors != NULL) {
        for (Int32 i = 0; i < mCursors->GetLength(); ++i) {
            (*mCursors)[i] = NULL;
        }
        ArrayOf< AutoPtr<ICursor> >::Free(mCursors);
    }
}

ECode MergeCursor::Init(
    /* [in] */ ArrayOf<ICursor*>* cursors)
{
    assert(mCursors == NULL);
    mCursors = (ArrayOf< AutoPtr<ICursor> >*)cursors;
    mCursor = (*cursors)[0];
    for (Int32 i = 0; i < mCursors->GetLength(); i++) {
        if ((*mCursors)[i] == NULL) continue;
        (*mCursors)[i]->RegisterDataSetObserver(mObserver.Get());
    }
    return NOERROR;
}

Int32 MergeCursor::GetCount()
{
    Int32 count = 0;
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0; i < length; i++) {
        if ((*mCursors)[i] != NULL) {
            Int32 c;
            (*mCursors)[i]->GetCount(&c);
            count += c;
        }
    }
    return count;
}

Boolean MergeCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition)
{
    /* Find the right cursor */
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
        return ret;
    }
    return FALSE;
}

ECode MergeCursor::DeleteRow(
    /* [out] */ Boolean* succeeded)
{
    return mCursor->DeleteRow(succeeded);
}

ECode MergeCursor::CommitUpdates(
    /* [out] */ Boolean* succeeded)
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0 ; i < length ; i++) {
        if ((*mCursors)[i] != NULL) {
            FAIL_RETURN((*mCursors)[i]->CommitUpdates(succeeded));
        }
    }
    OnChange(TRUE);
    *succeeded = TRUE;
    return NOERROR;
}

ECode MergeCursor::GetString(
    /* [in] */ Int32 column,
    /* [out] */ String* str)
{
    return mCursor->GetString(column, str);
}

ECode MergeCursor::GetInt16(
    /* [in] */ Int32 column,
    /* [out] */ Int16* value)
{
    return mCursor->GetInt16(column, value);
}

ECode MergeCursor::GetInt32(
    /* [in] */ Int32 column,
    /* [out] */ Int32* value)
{
    return mCursor->GetInt32(column, value);
}

ECode MergeCursor::GetInt64(
    /* [in] */ Int32 column,
    /* [out] */ Int64* value)
{
    return mCursor->GetInt64(column, value);
}

ECode MergeCursor::GetFloat(
    /* [in] */ Int32 column,
    /* [out] */ Float* value)
{
    return mCursor->GetFloat(column, value);
}

ECode MergeCursor::GetDouble(
    /* [in] */ Int32 column,
    /* [out] */ Double* value)
{
    return mCursor->GetDouble(column, value);
}

ECode MergeCursor::IsNull(
    /* [in] */ Int32 column,
    /* [out] */ Boolean* value)
{
    return mCursor->IsNull(column, value);
}

ECode MergeCursor::GetBlob(
    /* [in] */ Int32 column,
    /* [out] */ ArrayOf<Byte>** blob)
{
    return mCursor->GetBlob(column, blob);
}

ECode MergeCursor::GetColumnNames(
    /* [out] */ ArrayOf<String>** names)
{
    if (mCursor != NULL) {
        return mCursor->GetColumnNames(names);
    }
    else {
        *names = ArrayOf<String>::Alloc(1);
        return NOERROR;
    }
}

ECode MergeCursor::Deactivate()
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0 ; i < length; i++) {
        if ((*mCursors)[i] != NULL) {
            (*mCursors)[i]->Deactivate();
        }
    }
    return AbstractCursor::Deactivate();
}

ECode MergeCursor::Close()
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0 ; i < length; i++) {
        if ((*mCursors)[i] == NULL) continue;
        (*mCursors)[i]->Close();
    }
    return AbstractCursor::Close();
}

ECode MergeCursor::RegisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0; i < length; i++) {
        if ((*mCursors)[i] != NULL) {
            (*mCursors)[i]->RegisterContentObserver(observer);
        }
    }
    return NOERROR;
}

ECode MergeCursor::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0; i < length; i++) {
        if ((*mCursors)[i] != NULL) {
            (*mCursors)[i]->UnregisterContentObserver(observer);
        }
    }
    return NOERROR;
}

ECode MergeCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0; i < length; i++) {
        if ((*mCursors)[i] != NULL) {
            (*mCursors)[i]->RegisterDataSetObserver(observer);
        }
    }
    return NOERROR;
}

ECode MergeCursor::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    Int32 length = mCursors->GetLength();
    for (Int32 i = 0; i < length; i++) {
        if ((*mCursors)[i] != NULL) {
            (*mCursors)[i]->UnregisterDataSetObserver(observer);
        }
    }
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
