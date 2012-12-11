
#include "database/CCursorWrapper.h"
ECode CCursorWrapper::GetCount(
    /* [out] */ Int32 * pCount)
{
    CursorWrapper::GetCount(pCount);
    return NOERROR;
}

ECode CCursorWrapper::GetPosition(
    /* [out] */ Int32 * pPosition)
{
    CursorWrapper::GetPosition(pPosition);
    return NOERROR;
}

ECode CCursorWrapper::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::Move(offset, pValue);
    return NOERROR;
}

ECode CCursorWrapper::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::MoveToPosition(position, pValue);
    return NOERROR;
}

ECode CCursorWrapper::MoveToFirst(
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::MoveToFirst(pValue);
    return NOERROR;
}

ECode CCursorWrapper::MoveToLast(
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::MoveToLast(pValue);
    return NOERROR;
}

ECode CCursorWrapper::MoveToNext(
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::MoveToNext(pValue);
    return NOERROR;
}

ECode CCursorWrapper::MoveToPrevious(
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::MoveToPrevious(pValue);
    return NOERROR;
}

ECode CCursorWrapper::IsFirst(
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::IsFirst(pValue);
    return NOERROR;
}

ECode CCursorWrapper::IsLast(
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::IsLast(pValue);
    return NOERROR;
}

ECode CCursorWrapper::IsBeforeFirst(
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::IsBeforeFirst(pValue);
    return NOERROR;
}

ECode CCursorWrapper::IsAfterLast(
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::IsAfterLast(pValue);
    return NOERROR;
}

ECode CCursorWrapper::DeleteRow(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    CursorWrapper::GetColumnIndex(columnName, pColumnIndex);
    return NOERROR;
}

ECode CCursorWrapper::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    CursorWrapper::GetColumnIndexOrThrow(columnName, pColumnIndex);
    return NOERROR;
}

ECode CCursorWrapper::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnName)
{
    CursorWrapper::GetColumnName(columnIndex, pColumnName);
    return NOERROR;
}

ECode CCursorWrapper::GetColumnNames(
    /* [out, callee] */ ArrayOf<String> ** ppColumnNames)
{
    CursorWrapper::GetColumnNames(ppColumnNames);
    return NOERROR;
}

ECode CCursorWrapper::GetColumnCount(
    /* [out] */ Int32 * pColumnCount)
{
    CursorWrapper::GetColumnCount(pColumnCount);
    return NOERROR;
}

ECode CCursorWrapper::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte> ** ppBlob)
{
    CursorWrapper::GetBlob(columnIndex, ppBlob);
    return NOERROR;
}

ECode CCursorWrapper::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnValue)
{
    CursorWrapper::GetString(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CCursorWrapper::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer * pBuf)
{
    CursorWrapper::CopyStringToBuffer(columnIndex, pBuf);
    return NOERROR;
}

ECode CCursorWrapper::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16 * pColumnValue)
{
    CursorWrapper::GetInt16(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CCursorWrapper::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32 * pColumnValue)
{
    CursorWrapper::GetInt32(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CCursorWrapper::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64 * pColumnValue)
{
    CursorWrapper::GetInt64(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CCursorWrapper::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float * pColumnValue)
{
    CursorWrapper::GetFloat(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CCursorWrapper::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double * pColumnValue)
{
    CursorWrapper::GetDouble(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CCursorWrapper::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    CursorWrapper::IsNull(columnIndex, pValue);
    return NOERROR;
}

ECode CCursorWrapper::SupportsUpdates(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::HasUpdates(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const ArrayOf<Byte> & value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UpdateToNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::CommitUpdates(
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::AbortUpdates()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::Deactivate()
{
    CursorWrapper::Deactivate();
    return NOERROR;
}

ECode CCursorWrapper::Requery(
    /* [out] */ Boolean * pSucceeded)
{
    CursorWrapper::Requery(pSucceeded);
    return NOERROR;
}

ECode CCursorWrapper::Close()
{
    CursorWrapper::Close();
    return NOERROR;
}

ECode CCursorWrapper::IsClosed(
    /* [out] */ Boolean * pClosed)
{
    CursorWrapper::IsClosed(pClosed);
    return NOERROR;
}

ECode CCursorWrapper::RegisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    CursorWrapper::RegisterContentObserver(pObserver);
    return NOERROR;
}

ECode CCursorWrapper::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    CursorWrapper::UnregisterContentObserver(pObserver);
    return NOERROR;
}

ECode CCursorWrapper::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    CursorWrapper::RegisterDataSetObserver(pObserver);
    return NOERROR;
}

ECode CCursorWrapper::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    CursorWrapper::UnregisterDataSetObserver(pObserver);
    return NOERROR;
}

ECode CCursorWrapper::SetNotificationUri(
    /* [in] */ IContentResolver * pCr,
    /* [in] */ IUri * pUri)
{
    CursorWrapper::SetNotificationUri(pCr, pUri);
    return NOERROR;
}

ECode CCursorWrapper::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean * pResult)
{
    CursorWrapper::GetWantsAllOnMoveCalls(pResult);
    return NOERROR;
}

ECode CCursorWrapper::GetExtras(
    /* [out] */ IBundle ** ppExtras)
{
    CursorWrapper::GetExtras(ppExtras);
    return NOERROR;
}

ECode CCursorWrapper::Respond(
    /* [in] */ IBundle * pExtras,
    /* [out] */ IBundle ** ppResult)
{
    CursorWrapper::Respond(pExtras, ppResult);
    return NOERROR;
}

ECode CCursorWrapper::constructor()
{
    CursorWrapper::CursorWrapper();
    return NOERROR;
}

ECode CCursorWrapper::constructor(
    /* [in] */ ICursor * pCursor)
{
    CursorWrapper::Init(pCursor);
    return NOERROR;
}

