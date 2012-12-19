
#include "ext/frameworkdef.h"
#include "database/CCursorWrapper.h"


ECode CCursorWrapper::constructor(
    /* [in] */ ICursor* cursor)
{
    return CursorWrapper::Init(cursor);
}

ECode CCursorWrapper::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    return CursorWrapper::GetCount(count);
}

ECode CCursorWrapper::GetPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);

    return CursorWrapper::GetPosition(position);
}

ECode CCursorWrapper::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::Move(offset, succeeded);
}

ECode CCursorWrapper::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::MoveToPosition(position, succeeded);
}

ECode CCursorWrapper::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::MoveToFirst(succeeded);
}

ECode CCursorWrapper::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::MoveToLast(succeeded);
}

ECode CCursorWrapper::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::MoveToNext(succeeded);
}

ECode CCursorWrapper::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::MoveToPrevious(succeeded);
}

ECode CCursorWrapper::IsFirst(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::IsFirst(result);
}

ECode CCursorWrapper::IsLast(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::IsLast(result);
}

ECode CCursorWrapper::IsBeforeFirst(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::IsBeforeFirst(result);
}

ECode CCursorWrapper::IsAfterLast(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::IsAfterLast(result);
}

ECode CCursorWrapper::DeleteRow(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::DeleteRow(result);
}

ECode CCursorWrapper::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex);

    return CursorWrapper::GetColumnIndex(columnName, columnIndex);
}

ECode CCursorWrapper::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex);

    return CursorWrapper::GetColumnIndexOrThrow(columnName, columnIndex);
}

ECode CCursorWrapper::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnName)
{
    VALIDATE_NOT_NULL(columnName);

    return CursorWrapper::GetColumnName(columnIndex, columnName);
}

ECode CCursorWrapper::GetColumnNames(
    /* [out, callee] */ ArrayOf<String>** columnNames)
{
    VALIDATE_NOT_NULL(columnNames);

    return CursorWrapper::GetColumnNames(columnNames);
}

ECode CCursorWrapper::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    VALIDATE_NOT_NULL(columnCount);

    return CursorWrapper::GetColumnCount(columnCount);
}

ECode CCursorWrapper::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out, callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob);

    return CursorWrapper::GetBlob(columnIndex, blob);
}

ECode CCursorWrapper::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnValue)
{
    VALIDATE_NOT_NULL(columnValue);

    return CursorWrapper::GetString(columnIndex, columnValue);
}

ECode CCursorWrapper::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buffer)
{
    return CursorWrapper::CopyStringToBuffer(columnIndex, buffer);
}

ECode CCursorWrapper::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* columnValue)
{
    VALIDATE_NOT_NULL(columnValue);

    return CursorWrapper::GetInt16(columnIndex, columnValue);
}

ECode CCursorWrapper::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* columnValue)
{
    VALIDATE_NOT_NULL(columnValue);

    return CursorWrapper::GetInt32(columnIndex, columnValue);
}

ECode CCursorWrapper::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* columnValue)
{
    VALIDATE_NOT_NULL(columnValue);

    return CursorWrapper::GetInt64(columnIndex, columnValue);
}

ECode CCursorWrapper::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* columnValue)
{
    VALIDATE_NOT_NULL(columnValue);

    return CursorWrapper::GetFloat(columnIndex, columnValue);
}

ECode CCursorWrapper::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* columnValue)
{
    VALIDATE_NOT_NULL(columnValue);

    return CursorWrapper::GetDouble(columnIndex, columnValue);
}

ECode CCursorWrapper::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::IsNull(columnIndex, result);
}

ECode CCursorWrapper::SupportsUpdates(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::SupportsUpdates(result);
}

ECode CCursorWrapper::HasUpdates(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::HasUpdates(result);
}

ECode CCursorWrapper::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::UpdateBlob(columnIndex, value, succeeded);
}

ECode CCursorWrapper::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::UpdateString(columnIndex, value, succeeded);
}

ECode CCursorWrapper::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::UpdateInt16(columnIndex, value, succeeded);
}

ECode CCursorWrapper::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::UpdateInt32(columnIndex, value, succeeded);
}

ECode CCursorWrapper::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::UpdateInt64(columnIndex, value, succeeded);
}

ECode CCursorWrapper::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::UpdateFloat(columnIndex, value, succeeded);
}

ECode CCursorWrapper::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::UpdateDouble(columnIndex, value, succeeded);
}

ECode CCursorWrapper::UpdateToNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::UpdateToNull(columnIndex, succeeded);
}

ECode CCursorWrapper::CommitUpdates(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::CommitUpdates(succeeded);
}

ECode CCursorWrapper::AbortUpdates()
{
    return CursorWrapper::AbortUpdates();
}

ECode CCursorWrapper::Deactivate()
{
    return CursorWrapper::Deactivate();
}

ECode CCursorWrapper::Requery(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);

    return CursorWrapper::Requery(succeeded);
}

ECode CCursorWrapper::Close()
{
    return CursorWrapper::Close();
}

ECode CCursorWrapper::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);

    return CursorWrapper::IsClosed(isClosed);
}

ECode CCursorWrapper::RegisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    return CursorWrapper::RegisterContentObserver(observer);
}

ECode CCursorWrapper::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    return CursorWrapper::UnregisterContentObserver(observer);
}

ECode CCursorWrapper::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return CursorWrapper::RegisterDataSetObserver(observer);
}

ECode CCursorWrapper::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return CursorWrapper::UnregisterDataSetObserver(observer);
}

ECode CCursorWrapper::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return CursorWrapper::SetNotificationUri(cr, uri);
}

ECode CCursorWrapper::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::GetWantsAllOnMoveCalls(result);
}

ECode CCursorWrapper::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);

    return CursorWrapper::GetExtras(extras);
}

ECode CCursorWrapper::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result);

    return CursorWrapper::Respond(extras, result);
}

