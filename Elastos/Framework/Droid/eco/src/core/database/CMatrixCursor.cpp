
#include "database/CMatrixCursor.h"
ECode CMatrixCursor::GetCount(
    /* [out] */ Int32 * pCount)
{
    MatrixCursor::GetCount(pCount);
    return NOERROR;
}

ECode CMatrixCursor::GetPosition(
    /* [out] */ Int32 * pPosition)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::MoveToFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::MoveToLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::MoveToNext(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::MoveToPrevious(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::IsFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::IsLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::IsBeforeFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::IsAfterLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::DeleteRow(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetColumnNames(
    /* [out, callee] */ ArrayOf<String> ** ppColumnNames)
{
    MatrixCursor::GetColumnNames(ppColumnNames);
    return NOERROR;
}

ECode CMatrixCursor::GetColumnCount(
    /* [out] */ Int32 * pColumnCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte> ** ppBlob)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnValue)
{
    MatrixCursor::GetString(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMatrixCursor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer * pBuf)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16 * pColumnValue)
{
    MatrixCursor::GetInt16(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMatrixCursor::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32 * pColumnValue)
{
    MatrixCursor::GetInt32(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMatrixCursor::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64 * pColumnValue)
{
    MatrixCursor::GetInt64(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMatrixCursor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float * pColumnValue)
{
    MatrixCursor::GetFloat(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMatrixCursor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double * pColumnValue)
{
    MatrixCursor::GetDouble(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMatrixCursor::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    MatrixCursor::IsNull(columnIndex, pValue);
    return NOERROR;
}

ECode CMatrixCursor::SupportsUpdates(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::HasUpdates(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const ArrayOf<Byte> & value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UpdateToNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::CommitUpdates(
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::AbortUpdates()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::Deactivate()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::Requery(
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::IsClosed(
    /* [out] */ Boolean * pClosed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::RegisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::SetNotificationUri(
    /* [in] */ IContentResolver * pCr,
    /* [in] */ IUri * pUri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetExtras(
    /* [out] */ IBundle ** ppExtras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::Respond(
    /* [in] */ IBundle * pExtras,
    /* [out] */ IBundle ** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetWindow(
    /* [out] */ ICursorWindow ** ppWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow * pWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::NotifyDataSetChange()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetDataSetObservable(
    /* [out] */ IDataSetObservable ** ppDso)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::OnChange(
    /* [in] */ Boolean selfChange)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::IsFieldUpdated(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pRst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::GetUpdatedField(
    /* [in] */ Int32 columnIndex,
    /* [out] */ IInterface ** ppObj)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::CheckPosition()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMatrixCursor::NewRow(
    /* [out] */ IRowBuilder ** ppBuilder)
{
    MatrixCursor::NewRow(ppBuilder);
    return NOERROR;
}

ECode CMatrixCursor::AddRow(
    /* [in] */ const ArrayOf<IInterface *> & columnValues)
{
    MatrixCursor::AddRow(columnValues);
    return NOERROR;
}

ECode CMatrixCursor::AddRowEx(
    /* [in] */ IObjectContainer * pColumnValues)
{
    MatrixCursor::AddRowEx(pColumnValues);
    return NOERROR;
}

ECode CMatrixCursor::constructor()
{
    MatrixCursor::MatrixCursor();
    return NOERROR;
}

ECode CMatrixCursor::constructor(
    /* [in] */ const ArrayOf<String> & columnNames,
    /* [in] */ Int32 initialCapacity)
{
    MatrixCursor::Init(columnNames, initialCapacity);
    return NOERROR;
}

ECode CMatrixCursor::constructor(
    /* [in] */ const ArrayOf<String> & columnNames)
{
    MatrixCursor::Init(columnNames);
    return NOERROR;
}

