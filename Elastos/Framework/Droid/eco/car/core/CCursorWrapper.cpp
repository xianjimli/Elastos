
#include "database/CCursorWrapper.h"
ECode CCursorWrapper::GetCount(
    /* [out] */ Int32 * pCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetPosition(
    /* [out] */ Int32 * pPosition)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::MoveToFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::MoveToLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::MoveToNext(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::MoveToPrevious(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::IsFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::IsLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::IsBeforeFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::IsAfterLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetColumnNames(
    /* [out, callee] */ ArrayOf<String> ** ppColumnNames)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetColumnCount(
    /* [out] */ Int32 * pColumnCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte> ** ppBlob)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer * pBuf)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16 * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32 * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64 * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
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
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::Requery(
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::IsClosed(
    /* [out] */ Boolean * pClosed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::RegisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::SetNotificationUri(
    /* [in] */ IContentResolver * pCr,
    /* [in] */ IUri * pUri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::GetExtras(
    /* [out] */ IBundle ** ppExtras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::Respond(
    /* [in] */ IBundle * pExtras,
    /* [out] */ IBundle ** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapper::constructor(
    /* [in] */ ICursor * pCursor)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

