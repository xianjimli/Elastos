
#include "database/CMergeCursor.h"
ECode CMergeCursor::GetCount(
    /* [out] */ Int32 * pCount)
{
    MergeCursor::GetCount(pCount);
    return NOERROR;
}

ECode CMergeCursor::GetPosition(
    /* [out] */ Int32 * pPosition)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::MoveToFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::MoveToLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::MoveToNext(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::MoveToPrevious(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::IsFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::IsLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::IsBeforeFirst(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::IsAfterLast(
    /* [out] */ Boolean * pValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::DeleteRow(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32 * pColumnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetColumnNames(
    /* [out, callee] */ ArrayOf<String> ** ppColumnNames)
{
    MergeCursor::GetColumnNames(ppColumnNames);
    return NOERROR;
}

ECode CMergeCursor::GetColumnCount(
    /* [out] */ Int32 * pColumnCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte> ** ppBlob)
{
    MergeCursor::GetBlob(columnIndex, ppBlob);
    return NOERROR;
}

ECode CMergeCursor::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String * pColumnValue)
{
    MergeCursor::GetString(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMergeCursor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer * pBuf)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16 * pColumnValue)
{
    MergeCursor::GetInt16(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMergeCursor::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32 * pColumnValue)
{
    MergeCursor::GetInt32(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMergeCursor::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64 * pColumnValue)
{
    MergeCursor::GetInt64(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMergeCursor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float * pColumnValue)
{
    MergeCursor::GetFloat(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMergeCursor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double * pColumnValue)
{
    MergeCursor::GetDouble(columnIndex, pColumnValue);
    return NOERROR;
}

ECode CMergeCursor::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pValue)
{
    MergeCursor::IsNull(columnIndex, pValue);
    return NOERROR;
}

ECode CMergeCursor::SupportsUpdates(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::HasUpdates(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const ArrayOf<Byte> & value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::UpdateToNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::CommitUpdates(
    /* [out] */ Boolean * pSucceeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::AbortUpdates()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::Deactivate()
{
    MergeCursor::Deactivate();
    return NOERROR;
}

ECode CMergeCursor::Requery(
    /* [out] */ Boolean * pSucceeded)
{
    MergeCursor::Requery(pSucceeded);
    return NOERROR;
}

ECode CMergeCursor::Close()
{
    MergeCursor::Close();
    return NOERROR;
}

ECode CMergeCursor::IsClosed(
    /* [out] */ Boolean * pClosed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::RegisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    MergeCursor::RegisterContentObserver(pObserver);
    return NOERROR;
}

ECode CMergeCursor::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver * pObserver)
{
    MergeCursor::UnregisterContentObserver(pObserver);
    return NOERROR;
}

ECode CMergeCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    MergeCursor::RegisterDataSetObserver(pObserver);
    return NOERROR;
}

ECode CMergeCursor::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver * pObserver)
{
    MergeCursor::UnregisterDataSetObserver(pObserver);
    return NOERROR;
}

ECode CMergeCursor::SetNotificationUri(
    /* [in] */ IContentResolver * pCr,
    /* [in] */ IUri * pUri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean * pResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetExtras(
    /* [out] */ IBundle ** ppExtras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::Respond(
    /* [in] */ IBundle * pExtras,
    /* [out] */ IBundle ** ppResult)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetWindow(
    /* [out] */ ICursorWindow ** ppWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow * pWindow)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean * pSucceeded)
{
    MergeCursor::OnMove(oldPosition, newPosition, pSucceeded);
    return NOERROR;
}

ECode CMergeCursor::NotifyDataSetChange()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetDataSetObservable(
    /* [out] */ IDataSetObservable ** ppDso)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::OnChange(
    /* [in] */ Boolean selfChange)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::IsFieldUpdated(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean * pRst)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::GetUpdatedField(
    /* [in] */ Int32 columnIndex,
    /* [out] */ IInterface ** ppObj)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::CheckPosition()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CMergeCursor::constructor()
{
    MergeCursor::MergeCursor();
    return NOERROR;
}

ECode CMergeCursor::constructor(
    /* [in] */ const ArrayOf<ICursor *> & cursors)
{
    MergeCursor::Init(cursors);
    return NOERROR;
}

