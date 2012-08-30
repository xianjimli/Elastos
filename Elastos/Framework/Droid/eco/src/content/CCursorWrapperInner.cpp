
#include "content/CCursorWrapperInner.h"

ECode CCursorWrapperInner::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [out] */ ICharArrayBuffer** buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::Deactivate()
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte>** blob)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetColumnNames(
    /* [out] */ ArrayOf<String>** columnNames)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetCount(
    /* [out] */ Int32* count)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* columnValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetExtras(
    /* [out] */ IBundle** extras)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* columnValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* columnValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* columnValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetPosition(
    /* [out] */ Int32* position)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* columnValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnValue)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::IsAfterLast(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::IsBeforeFirst(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::IsClosed(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::IsFirst(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::IsLast(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::MoveToFirst(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::MoveToLast(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::MoveToNext(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::MoveToPrevious(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::RegisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::Requery(
    /* [out] */ Boolean* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::DeleteRow(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::SupportsUpdates(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::HasUpdates(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::UpdateToNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::CommitUpdates(
    /* [out] */ Boolean* succeeded)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::AbortUpdates()
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner:: Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCursorWrapperInner::constructor(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContentProvider* provider)
{
    return E_NOT_IMPLEMENTED;
}
