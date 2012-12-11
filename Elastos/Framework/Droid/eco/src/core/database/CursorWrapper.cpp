
#include "database/CursorWrapper.h"

CursorWrapper::CursorWrapper()
{
}

CursorWrapper::~CursorWrapper()
{
}

ECode CursorWrapper::Init(
        /* [in] */ ICursor* cursor)
{
    mCursor = cursor;
    return NOERROR;
}

ECode CursorWrapper::Close()
{
    FAIL_RETURN(mCursor->Close());
    return NOERROR;
}

ECode CursorWrapper::IsClosed(
        /* [out] */ Boolean* res)
{
    assert(res != NULL);
    FAIL_RETURN(mCursor->IsClosed(res));
    return NOERROR;
}

ECode CursorWrapper::GetCount(
        /* [out] */ Int32* cnt)
{
    assert(cnt != NULL);
    FAIL_RETURN(mCursor->GetCount(cnt));
    return NOERROR;
}

ECode CursorWrapper::Deactivate()
{
    FAIL_RETURN(mCursor->Deactivate());
    return NOERROR;
}

ECode CursorWrapper::MoveToFirst(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->MoveToFirst(rst));
    return NOERROR;
}

ECode CursorWrapper::GetColumnCount(
        /* [out] */ Int32* cnt)
{
    assert(cnt != NULL);
    FAIL_RETURN(mCursor->GetColumnCount(cnt));
    return NOERROR;
}

ECode CursorWrapper::GetColumnIndex(
        /* [in] */ String columnName,
        /* [out] */ Int32* index)
{
    assert(index != NULL);
    FAIL_RETURN(mCursor->GetColumnIndex(columnName, index));
    return NOERROR;
}

ECode CursorWrapper::GetColumnIndexOrThrow(
        /* [in] */ String columnName,
        /* [out] */ Int32* index)
{
    assert(index != NULL);
    FAIL_RETURN(mCursor->GetColumnIndexOrThrow(columnName, index));
    return NOERROR;
}

ECode CursorWrapper::GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* name)
{
    assert(name != NULL);
    FAIL_RETURN(mCursor->GetColumnName(columnIndex, name));
    return NOERROR;
}

ECode CursorWrapper::GetColumnNames(
        /* [out,callee] */ ArrayOf<String>** columnNames)
{
    FAIL_RETURN(mCursor->GetColumnNames(columnNames));
    return NOERROR;
}

ECode CursorWrapper::GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* value)
{
    assert(value != NULL);
    FAIL_RETURN(mCursor->GetDouble(columnIndex, value));
    return NOERROR;
}

ECode CursorWrapper::GetExtras(
    /* [out] */ IBundle** extras)
{
    FAIL_RETURN(mCursor->GetExtras(extras));
    return NOERROR;
}

ECode CursorWrapper::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* value)
{
    assert(value != NULL);
    FAIL_RETURN(mCursor->GetFloat(columnIndex, value));
    return NOERROR;
}

ECode CursorWrapper::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    FAIL_RETURN(mCursor->GetInt32(columnIndex, value));
    return NOERROR;
}

ECode CursorWrapper::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* value)
{
    assert(value != NULL);
    FAIL_RETURN(mCursor->GetInt64(columnIndex, value));
    return NOERROR;
}

ECode CursorWrapper::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* value)
{
    assert(value != NULL);
    FAIL_RETURN(mCursor->GetInt16(columnIndex, value));
    return NOERROR;
}

ECode CursorWrapper::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* value)
{
    assert(value != NULL);
    FAIL_RETURN(mCursor->GetString(columnIndex, value));
    return NOERROR;
}

ECode CursorWrapper::CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer* buffer)
{
    FAIL_RETURN(mCursor->CopyStringToBuffer(columnIndex, buffer));
    return NOERROR;
}

 ECode CursorWrapper::GetBlob(
             /* [in] */  Int32 columnIndex,
             /* [out,callee] */ ArrayOf<Byte>** blob)
{
    FAIL_RETURN(mCursor->GetBlob(columnIndex, blob));
    return NOERROR;
}

ECode CursorWrapper::IsAfterLast(
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->IsAfterLast(rst));
    return NOERROR;
}

ECode CursorWrapper::IsBeforeFirst(
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->IsBeforeFirst(rst));
    return NOERROR;
}

ECode CursorWrapper::GetWantsAllOnMoveCalls(
            /* [out] */ Boolean* value)
{
    assert(value != NULL);
    FAIL_RETURN(mCursor->GetWantsAllOnMoveCalls(value));
    return NOERROR;
}

ECode CursorWrapper::IsFirst(
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->IsFirst(rst));
    return NOERROR;
}

ECode CursorWrapper::IsLast(
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->IsLast(rst));
    return NOERROR;
}

ECode CursorWrapper::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->IsNull(columnIndex, rst));
    return NOERROR;
}

ECode CursorWrapper::MoveToLast(
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->MoveToLast(rst));
    return NOERROR;
}

ECode CursorWrapper::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->Move(offset, rst));
    return NOERROR;
}

ECode CursorWrapper::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->MoveToPosition(position, rst));
    return NOERROR;
}

ECode CursorWrapper::MoveToNext(
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->MoveToNext(rst));
    return NOERROR;
}

ECode CursorWrapper::GetPosition(
    /*[out]*/ Int32* position)
{
    assert(position != NULL);
    FAIL_RETURN(mCursor->GetPosition(position));
    return NOERROR;
}

ECode CursorWrapper::MoveToPrevious(
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->MoveToPrevious(rst));
    return NOERROR;
}

ECode CursorWrapper::RegisterContentObserver(
        /* [in] */ ILocalContentObserver* observer)
{
    FAIL_RETURN(mCursor->RegisterContentObserver(observer));
    return NOERROR;
}

ECode CursorWrapper::RegisterDataSetObserver(
        /* [in] */IDataSetObserver* observer)
{
    FAIL_RETURN(mCursor->RegisterDataSetObserver(observer));
    return NOERROR;
}

ECode CursorWrapper::Requery(
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    FAIL_RETURN(mCursor->Requery(rst));
    return NOERROR;
}

ECode CursorWrapper::Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** bundle)
{
    FAIL_RETURN(mCursor->Respond(extras, bundle));
    return NOERROR;
}

ECode CursorWrapper::SetNotificationUri(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri)
{
    FAIL_RETURN(mCursor->SetNotificationUri(cr, uri));
    return NOERROR;
}

ECode CursorWrapper::UnregisterContentObserver(
        /* [in] */ ILocalContentObserver* observer)
{
    FAIL_RETURN(mCursor->UnregisterContentObserver(observer));
    return NOERROR;
}

ECode CursorWrapper::UnregisterDataSetObserver(
        /* [in] */IDataSetObserver* observer)
{
    FAIL_RETURN(mCursor->UnregisterDataSetObserver(observer));
    return NOERROR;
}
