
#include "database/CursorWrapper.h"

CursorWrapper::CursorWrapper()
{}

ECode CursorWrapper::Init(
    /* [in] */ ICursor* cursor)
{
    mCursor = cursor;
    return NOERROR;
}

ECode CursorWrapper::AbortUpdates()
{
    return mCursor->AbortUpdates();
}

ECode CursorWrapper::Close()
{
    return mCursor->Close();
}

ECode CursorWrapper::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    return mCursor->IsClosed(isClosed);
}

ECode CursorWrapper::CommitUpdates(
    /* [out] */ Boolean* succeeded)
{
    return mCursor->CommitUpdates(succeeded);
}

/**
 * @hide
 * @deprecated
 */
// public boolean commitUpdates(
//         Map<? extends Long, ? extends Map<String, Object>> values) {
//     return mCursor.commitUpdates(values);
// }

ECode CursorWrapper::GetCount(
    /* [out] */ Int32* count)
{
    return mCursor->GetCount(count);
}

ECode CursorWrapper::Deactivate()
{
    return mCursor->Deactivate();
}

ECode CursorWrapper::DeleteRow(
    /* [out] */ Boolean* succeeded)
{
    return mCursor->DeleteRow(succeeded);
}

ECode CursorWrapper::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    return mCursor->MoveToFirst(succeeded);
}

ECode CursorWrapper::GetColumnCount(
    /* [out] */ Int32* count)
{
    return mCursor->GetColumnCount(count);
}

ECode CursorWrapper::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* index)
{
    return mCursor->GetColumnIndex(columnName, index);
}

ECode CursorWrapper::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* index)
{
    return mCursor->GetColumnIndexOrThrow(columnName, index);
}

ECode CursorWrapper::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* name)
{
    return mCursor->GetColumnName(columnIndex, name);
}

ECode CursorWrapper::GetColumnNames(
    /* [out,callee] */ ArrayOf<String>** columnNames)
{
    return mCursor->GetColumnNames(columnNames);
}

ECode CursorWrapper::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* value)
{
    return mCursor->GetDouble(columnIndex, value);
}

ECode CursorWrapper::GetExtras(
    /* [out] */ IBundle** extras)
{
    return mCursor->GetExtras(extras);
}

ECode CursorWrapper::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* value)
{
    return mCursor->GetFloat(columnIndex, value);
}

ECode CursorWrapper::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* value)
{
    return mCursor->GetInt32(columnIndex, value);
}

ECode CursorWrapper::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* value)
{
    return mCursor->GetInt64(columnIndex, value);
}

ECode CursorWrapper::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* value)
{
    return mCursor->GetInt16(columnIndex, value);
}

ECode CursorWrapper::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* value)
{
    return mCursor->GetString(columnIndex, value);
}

ECode CursorWrapper::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buffer)
{
    return mCursor->CopyStringToBuffer(columnIndex, buffer);
}

ECode CursorWrapper::GetBlob(
    /* [in] */  Int32 columnIndex,
    /* [out,callee] */ ArrayOf<Byte>** blob)
{
    return mCursor->GetBlob(columnIndex, blob);
}

ECode CursorWrapper::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* value)
{
    return mCursor->GetWantsAllOnMoveCalls(value);
}

ECode CursorWrapper::HasUpdates(
    /* [out] */ Boolean* result)
{
    return mCursor->HasUpdates(result);
}

ECode CursorWrapper::IsAfterLast(
    /* [out] */ Boolean* result)
{
    return mCursor->IsAfterLast(result);
}

ECode CursorWrapper::IsBeforeFirst(
    /* [out] */ Boolean* result)
{
    return mCursor->IsBeforeFirst(result);
}

ECode CursorWrapper::IsFirst(
    /* [out] */ Boolean* result)
{
    return mCursor->IsFirst(result);
}

ECode CursorWrapper::IsLast(
    /* [out] */ Boolean* result)
{
    return mCursor->IsLast(result);
}

ECode CursorWrapper::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    return mCursor->IsNull(columnIndex, result);
}

ECode CursorWrapper::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    return mCursor->MoveToLast(succeeded);
}

ECode CursorWrapper::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->Move(offset, succeeded);
}

ECode CursorWrapper::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->MoveToPosition(position, succeeded);
}

ECode CursorWrapper::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    return mCursor->MoveToNext(succeeded);
}

ECode CursorWrapper::GetPosition(
    /*[out]*/ Int32* position)
{
    return mCursor->GetPosition(position);
}

ECode CursorWrapper::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    return mCursor->MoveToPrevious(succeeded);
}

ECode CursorWrapper::RegisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    return mCursor->RegisterContentObserver(observer);
}

ECode CursorWrapper::RegisterDataSetObserver(
    /* [in] */IDataSetObserver* observer)
{
    return mCursor->RegisterDataSetObserver(observer);
}

ECode CursorWrapper::Requery(
    /* [out] */ Boolean* succeeded)
{
    return mCursor->Requery(succeeded);
}

ECode CursorWrapper::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    return mCursor->Respond(extras, bundle);
}

ECode CursorWrapper::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return mCursor->SetNotificationUri(cr, uri);
}

ECode CursorWrapper::SupportsUpdates(
    /* [out] */ Boolean* result)
{
    return mCursor->SupportsUpdates(result);
}

ECode CursorWrapper::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    return mCursor->UnregisterContentObserver(observer);
}

ECode CursorWrapper::UnregisterDataSetObserver(
    /* [in] */IDataSetObserver* observer)
{
    return mCursor->UnregisterDataSetObserver(observer);
}

ECode CursorWrapper::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->UpdateDouble(columnIndex, value, succeeded);
}

ECode CursorWrapper::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->UpdateFloat(columnIndex, value, succeeded);
}

ECode CursorWrapper::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->UpdateInt32(columnIndex, value, succeeded);
}

ECode CursorWrapper::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->UpdateInt64(columnIndex, value, succeeded);
}

ECode CursorWrapper::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->UpdateInt16(columnIndex, value, succeeded);
}

ECode CursorWrapper::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->UpdateString(columnIndex, value, succeeded);
}

ECode CursorWrapper::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in, out] */ const ArrayOf<Byte>& value,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->UpdateBlob(columnIndex, value, succeeded);
}

ECode CursorWrapper::UpdateToNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* succeeded)
{
    return mCursor->UpdateToNull(columnIndex, succeeded);
}
