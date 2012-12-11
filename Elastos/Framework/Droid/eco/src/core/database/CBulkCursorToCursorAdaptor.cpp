
#include "database/CBulkCursorToCursorAdaptor.h"
ECode CBulkCursorToCursorAdaptor::GetCount(
    /* [out] */ Int32 * pCount)
{
    BulkCursorToCursorAdaptor::GetCount(pCount);
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::GetPosition(
    /* [out] */ Int32 * pPosition)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::MoveToFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::MoveToLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::MoveToNext(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::MoveToPrevious(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsBeforeFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsAfterLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::DeleteRow(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetColumnNames(
    /* [out, callee] */ ArrayOf<String> ** ppColumnNames)
{
    BulkCursorToCursorAdaptor::GetColumnNames(ppColumnNames);
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::GetColumnCount(
    /* [out] */ Int32 * pColumnCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte> ** ppBlob)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer * pBuf)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16 * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32 * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64 * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double * pColumnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::SupportsUpdates(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::HasUpdates(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const ArrayOf<Byte> & value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UpdateToNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::CommitUpdates(
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::AbortUpdates()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::Deactivate()
{
    BulkCursorToCursorAdaptor::Deactivate();
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::Requery(
    /* [out] */ Boolean * pSucceeded)
{
    BulkCursorToCursorAdaptor::Requery(pSucceeded);
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::Close()
{
    BulkCursorToCursorAdaptor::Close();
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::IsClosed(
    /* [out] */ Boolean * pClosed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::RegisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::SetNotificationUri(
    /* [in] */ IContentResolver * pCr,
    /* [in] */ IUri * pUri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetExtras(
    /* [out] */ IBundle ** ppExtras)
{
    BulkCursorToCursorAdaptor::GetExtras(ppExtras);
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::Respond(
    /* [in] */ IBundle * pExtras,
    /* [out] */ IBundle ** ppResult)
{
    BulkCursorToCursorAdaptor::Respond(pExtras, ppResult);
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::GetWindow(
    /* [out] */ ICursorWindow ** ppWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow * pWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean * pSucceeded)
{
    BulkCursorToCursorAdaptor::OnMove(oldPosition, newPosition, pSucceeded);
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::NotifyDataSetChange()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetDataSetObservable(
    /* [out] */ IDataSetObservable ** ppDso)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::OnChange(
    /* [in] */ Boolean selfChange)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsFieldUpdated(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pRst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::GetUpdatedField(
    /* [in] */ Int32 columnIndex,
    /* [out] */ IInterface ** ppObj)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::CheckPosition()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsBlob(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::IsFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::SetWindow(
    /* [in] */ ICursorWindow * pWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::HasWindow(
    /* [out] */ Boolean * pRst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CBulkCursorToCursorAdaptor::Set(
    /* [in] */ IBulkCursor * pBulkCursor)
{
    BulkCursorToCursorAdaptor::Set(pBulkCursor);
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::SetEx(
    /* [in] */ IBulkCursor * pBulkCursor,
    /* [in] */ Int32 count,
    /* [in] */ Int32 idIndex)
{
    BulkCursorToCursorAdaptor::SetEx(pBulkCursor, count, idIndex);
    return NOERROR;
}

ECode CBulkCursorToCursorAdaptor::GetObserver(
    /* [out] */ IContentObserver ** ppObserver)
{
    BulkCursorToCursorAdaptor::GetObserver(ppObserver);
    return NOERROR;
}

