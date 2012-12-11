
#include "database/sqlite/CSQLiteCursor.h"


ECode CSQLiteCursor::constructor(
    /* [in] */ ISQLiteDatabase* db,
    /* [in] */ ISQLiteCursorDriver* driver,
    /* [in] */ const String& editTable,
    /* [in] */ ISQLiteQuery* query)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetCount(
    /* [out] */ Int32* count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetPosition(
    /* [out] */ Int32* position)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::MoveToFirst(
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::MoveToLast(
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::MoveToNext(
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::MoveToPrevious(
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsFirst(
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsLast(
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsBeforeFirst(
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsAfterLast(
    /* [out] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::DeleteRow(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetColumnNames(
    /* [out,callee] */ ArrayOf<String>** columnNames)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetColumnCount(
    /* [out] */ Int32* columnCount)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out,callee] */ ArrayOf<Byte>** blob)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* columnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in, out] */ ICharArrayBuffer* buf)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* columnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* columnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* columnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* columnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* columnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [o ut] */ Boolean* value)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::SupportsUpdates(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::HasUpdates(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const ArrayOf<Byte>& value,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//Deprecated
ECode CSQLiteCursor::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::UpdateToNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//@Deprecated
ECode CSQLiteCursor::CommitUpdates(
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

//        @Deprecated
//        boolean commitUpdates(Map<? extends Long,
//                ? extends Map<String,Object>> values);

//@Deprecated
ECode CSQLiteCursor::AbortUpdates()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::Deactivate()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::Requery(
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::IsClosed(
    /* [out] */ Boolean* closed)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::RegisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetExtras(
    /* [out] */ IBundle** extras)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** result)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetWindow(
    /* [out] */ ICursorWindow** window)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::FillWindow(
    /* [in] */ Int32 pos,
    /* [in] */ ICursorWindow* window)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition,
    /* [out] */ Boolean* succeeded)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::SetLoadStyle(
    /* [in] */ Int32 initialRead,
    /* [in] */ Int32 maxRead)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::GetDatabase(
    /* [out] */ ISQLiteDatabase** database)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CSQLiteCursor::SetSelectionArguments(
    /* [in] */ ArrayOf<String>* selectionArgs)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

