
#ifndef __CURSORMACRO_H__
#define __CURSORMACRO_H__

#define ICURSOR_METHODS_DECL()                                     \
    CARAPI GetCount(                                               \
        /* [out] */ Int32* count);                                 \
                                                                   \
    CARAPI GetPosition(                                            \
        /* [out] */ Int32* position);                              \
                                                                   \
    CARAPI Move(                                                   \
        /* [in] */ Int32 offset,                                   \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI MoveToPosition(                                         \
        /* [in] */ Int32 position,                                 \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI MoveToFirst(                                            \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI MoveToLast(                                             \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI MoveToNext(                                             \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI MoveToPrevious(                                         \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI IsFirst(                                                \
        /* [out] */ Boolean* isFirst);                             \
                                                                   \
    CARAPI IsLast(                                                 \
        /* [out] */ Boolean* isLast);                              \
                                                                   \
    CARAPI IsBeforeFirst(                                          \
        /* [out] */ Boolean* isBeforeFirst);                       \
                                                                   \
    CARAPI IsAfterLast(                                            \
        /* [out] */ Boolean* isAfterLast);                         \
                                                                   \
    CARAPI DeleteRow(                                              \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI GetColumnIndex(                                         \
        /* [in] */ const String& columnName,                       \
        /* [out] */ Int32* columnIndex);                           \
                                                                   \
    CARAPI GetColumnIndexOrThrow(                                  \
        /* [in] */ const String& columnName,                       \
        /* [out] */ Int32* columnIndex);                           \
                                                                   \
    CARAPI GetColumnName(                                          \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ String* columnName);                           \
                                                                   \
    CARAPI GetColumnNames(                                         \
        /* [out, callee] */ ArrayOf<String>** columnNames);        \
                                                                   \
    CARAPI GetColumnCount(                                         \
        /* [out] */ Int32* columnCount);                           \
                                                                   \
    CARAPI GetBlob(                                                \
        /* [in] */ Int32 columnIndex,                              \
        /* [out, callee] */ ArrayOf<Byte>** blob);                 \
                                                                   \
    CARAPI GetString(                                              \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ String* columnValue);                          \
                                                                   \
    CARAPI CopyStringToBuffer(                                     \
        /* [in] */ Int32 columnIndex,                              \
        /* [in] */ ICharArrayBuffer* buf);                         \
                                                                   \
    CARAPI GetInt16(                                               \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ Int16* columnValue);                           \
                                                                   \
    CARAPI GetInt32(                                               \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ Int32* columnValue);                           \
                                                                   \
    CARAPI GetInt64(                                               \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ Int64* columnValue);                           \
                                                                   \
    CARAPI GetFloat(                                               \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ Float* columnValue);                           \
                                                                   \
    CARAPI GetDouble(                                              \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ Double* columnValue);                          \
                                                                   \
    CARAPI IsNull(                                                 \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ Boolean* isNull);                              \
                                                                   \
    CARAPI SupportsUpdates(                                        \
        /* [out] */ Boolean* result);                              \
                                                                   \
    CARAPI HasUpdates(                                             \
        /* [out] */ Boolean* result);                              \
                                                                   \
    CARAPI UpdateBlob(                                             \
        /* [in] */ Int32 columnIndex,                              \
        /* [in] */ const ArrayOf<Byte>& value,                     \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI UpdateString(                                           \
        /* [in] */ Int32 columnIndex,                              \
        /* [in] */ const String& value,                            \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI UpdateInt16(                                            \
        /* [in] */ Int32 columnIndex,                              \
        /* [in] */ Int16 value,                                    \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI UpdateInt32(                                            \
        /* [in] */ Int32 columnIndex,                              \
        /* [in] */ Int32 value,                                    \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI UpdateInt64(                                            \
        /* [in] */ Int32 columnIndex,                              \
        /* [in] */ Int64 value,                                    \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI UpdateFloat(                                            \
        /* [in] */ Int32 columnIndex,                              \
        /* [in] */ Float value,                                    \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI UpdateDouble(                                           \
        /* [in] */ Int32 columnIndex,                              \
        /* [in] */ Double value,                                   \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI UpdateToNull(                                           \
        /* [in] */ Int32 columnIndex,                              \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI CommitUpdates(                                          \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI AbortUpdates();                                         \
                                                                   \
    CARAPI Deactivate();                                           \
                                                                   \
    CARAPI Requery(                                                \
        /* [out] */ Boolean* succeeded);                           \
                                                                   \
    CARAPI Close();                                                \
                                                                   \
    CARAPI IsClosed(                                               \
        /* [out] */ Boolean* isClosed);                            \
                                                                   \
    CARAPI RegisterContentObserver(                                \
        /* [in] */ ILocalContentObserver* observer);               \
                                                                   \
    CARAPI UnregisterContentObserver(                              \
        /* [in] */ ILocalContentObserver* observer);               \
                                                                   \
    CARAPI RegisterDataSetObserver(                                \
        /* [in] */ IDataSetObserver* observer);                    \
                                                                   \
    CARAPI UnregisterDataSetObserver(                              \
        /* [in] */ IDataSetObserver* observer);                    \
                                                                   \
    CARAPI SetNotificationUri(                                     \
        /* [in] */ IContentResolver* cr,                           \
        /* [in] */ IUri* uri);                                     \
                                                                   \
    CARAPI GetWantsAllOnMoveCalls(                                 \
        /* [out] */ Boolean* result);                              \
                                                                   \
    CARAPI GetExtras(                                              \
        /* [out] */ IBundle** extras);                             \
                                                                   \
    CARAPI Respond(                                                \
        /* [in] */ IBundle* extras,                                \
        /* [out] */ IBundle** result);

#define ICURSOR_METHODS_IMPL(className, superClass, overRideClass) \
ECode className::GetCount(                                         \
    /* [out] */ Int32* count)                                      \
{                                                                  \
    VALIDATE_NOT_NULL(count);                                      \
    *count = superClass::GetCount();                               \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::GetPosition(                                      \
    /* [out] */ Int32* position)                                   \
{                                                                  \
    VALIDATE_NOT_NULL(position);                                   \
    *position = superClass::GetPosition();                         \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::Move(                                             \
    /* [in] */ Int32 offset,                                       \
    /* [out] */ Boolean* succeeded)                                \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::Move(offset);                         \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::MoveToPosition(                                   \
    /* [in] */ Int32 position,                                     \
    /* [out] */ Boolean* succeeded)                                \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::MoveToPosition(position);             \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::MoveToFirst(                                      \
    /* [out] */ Boolean* succeeded)                                \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::MoveToFirst();                        \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::MoveToLast(                                       \
    /* [out] */ Boolean* succeeded)                                \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::MoveToLast();                         \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::MoveToNext(                                       \
    /* [out] */ Boolean* succeeded)                                \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::MoveToNext();                         \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::MoveToPrevious(                                   \
    /* [out] */ Boolean* succeeded)                                \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::MoveToPrevious();                     \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::IsFirst(                                          \
    /* [out] */ Boolean* isFirst)                                  \
{                                                                  \
    VALIDATE_NOT_NULL(isFirst);                                    \
    *isFirst = superClass::IsFirst();                              \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::IsLast(                                           \
    /* [out] */ Boolean* isLast)                                   \
{                                                                  \
    VALIDATE_NOT_NULL(isLast);                                     \
    *isLast = superClass::IsLast();                                \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::IsBeforeFirst(                                    \
    /* [out] */ Boolean* isBeforeFirst)                            \
{                                                                  \
    VALIDATE_NOT_NULL(isBeforeFirst);                              \
    *isBeforeFirst = superClass::IsBeforeFirst();                  \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::IsAfterLast(                                      \
    /* [out] */ Boolean* isAfterLast)                              \
{                                                                  \
    VALIDATE_NOT_NULL(isAfterLast);                                \
    *isAfterLast = superClass::IsAfterLast();                      \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::DeleteRow(                                        \
    /* [out] */ Boolean* succeeded)                                \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    return superClass::DeleteRow(succeeded);                          \
}                                                                  \
                                                                   \
ECode className::GetColumnIndex(                                   \
    /* [in] */ const String& columnName,                           \
    /* [out] */ Int32* columnIndex)                               \
{                                                                  \
    VALIDATE_NOT_NULL(columnIndex);                                \
    *columnIndex = superClass::GetColumnIndex(columnName);         \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::GetColumnIndexOrThrow(                            \
    /* [in] */ const String& columnName,                           \
    /* [out] */ Int32* columnIndex)                               \
{                                                                  \
    VALIDATE_NOT_NULL(columnIndex);                                \
    return superClass::GetColumnIndexOrThrow(columnName, columnIndex);  \
}                                                                  \
                                                                   \
ECode className::GetColumnName(                                    \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ String* columnName)                               \
{                                                                  \
    VALIDATE_NOT_NULL(columnName);                                 \
    *columnName = superClass::GetColumnName(columnIndex);          \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::GetColumnNames(                                   \
    /* [out, callee] */ ArrayOf<String>** columnNames)            \
{                                                                  \
    VALIDATE_NOT_NULL(columnNames);                                \
    return superClass::GetColumnNames(columnNames);                \
}                                                                  \
                                                                   \
ECode className::GetColumnCount(                                   \
    /* [out] */ Int32* columnCount)                               \
{                                                                  \
    VALIDATE_NOT_NULL(columnCount);                                \
    *columnCount = superClass::GetColumnCount();                   \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::GetBlob(                                          \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out, callee] */ ArrayOf<Byte>** blob)                     \
{                                                                  \
    VALIDATE_NOT_NULL(blob);                                       \
    return superClass::GetBlob(columnIndex, blob);                 \
}                                                                  \
                                                                   \
ECode className::GetString(                                        \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ String* columnValue)                              \
{                                                                  \
    VALIDATE_NOT_NULL(columnValue);                                \
    return superClass::GetString(columnIndex, columnValue);             \
}                                                                  \
                                                                   \
ECode className::CopyStringToBuffer(                               \
    /* [in] */ Int32 columnIndex,                                  \
    /* [in] */ ICharArrayBuffer* buf)                             \
{                                                                  \
    return superClass::CopyStringToBuffer(columnIndex, buf);       \
}                                                                  \
                                                                   \
ECode className::GetInt16(                                         \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ Int16* columnValue)                               \
{                                                                  \
    VALIDATE_NOT_NULL(columnValue);                                \
    return superClass::GetInt16(columnIndex, columnValue);                         \
}                                                                  \
                                                                   \
ECode className::GetInt32(                                         \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ Int32* columnValue)                               \
{                                                                  \
    VALIDATE_NOT_NULL(columnValue);                                \
    return superClass::GetInt32(columnIndex, columnValue);                         \
}                                                                  \
                                                                   \
ECode className::GetInt64(                                         \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ Int64* columnValue)                               \
{                                                                  \
    VALIDATE_NOT_NULL(columnValue);                                \
    return superClass::GetInt64(columnIndex, columnValue);                         \
}                                                                  \
                                                                   \
ECode className::GetFloat(                                         \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ Float* columnValue)                               \
{                                                                  \
    VALIDATE_NOT_NULL(columnValue);                                \
    return superClass::GetFloat(columnIndex, columnValue);                         \
}                                                                  \
                                                                   \
ECode className::GetDouble(                                        \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ Double* columnValue)                              \
{                                                                  \
    VALIDATE_NOT_NULL(columnValue);                                \
    return superClass::GetDouble(columnIndex, columnValue);                        \
}                                                                  \
                                                                   \
ECode className::IsNull(                                           \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ Boolean* isNull)                                  \
{                                                                  \
    VALIDATE_NOT_NULL(isNull);                                     \
    return superClass::IsNull(columnIndex, isNull);                                \
}                                                                  \
                                                                   \
ECode className::SupportsUpdates(                                  \
    /* [out] */ Boolean* result)                                  \
{                                                                  \
    VALIDATE_NOT_NULL(result);                                     \
    *result = superClass::SupportsUpdates();                       \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::HasUpdates(                                       \
    /* [out] */ Boolean* result)                                  \
{                                                                  \
    VALIDATE_NOT_NULL(result);                                     \
    *result = superClass::HasUpdates();                            \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::UpdateBlob(                                       \
    /* [in] */ Int32 columnIndex,                                  \
    /* [in] */ const ArrayOf<Byte>& value,                         \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::UpdateBlob(columnIndex, value);       \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::UpdateString(                                     \
    /* [in] */ Int32 columnIndex,                                  \
    /* [in] */ const String& value,                                \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::UpdateString(columnIndex, value);     \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::UpdateInt16(                                      \
    /* [in] */ Int32 columnIndex,                                  \
    /* [in] */ Int16 value,                                        \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::UpdateInt16(columnIndex, value);      \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::UpdateInt32(                                      \
    /* [in] */ Int32 columnIndex,                                  \
    /* [in] */ Int32 value,                                        \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::UpdateInt32(columnIndex, value);      \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::UpdateInt64(                                      \
    /* [in] */ Int32 columnIndex,                                  \
    /* [in] */ Int64 value,                                        \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::UpdateInt64(columnIndex, value);      \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::UpdateFloat(                                      \
    /* [in] */ Int32 columnIndex,                                  \
    /* [in] */ Float value,                                        \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::UpdateFloat(columnIndex, value);      \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::UpdateDouble(                                     \
    /* [in] */ Int32 columnIndex,                                  \
    /* [in] */ Double value,                                       \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::UpdateDouble(columnIndex, value);     \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::UpdateToNull(                                     \
    /* [in] */ Int32 columnIndex,                                  \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    *succeeded = superClass::UpdateToNull(columnIndex);            \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::CommitUpdates(                                    \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    return superClass::CommitUpdates(succeeded);                      \
}                                                                  \
                                                                   \
ECode className::AbortUpdates()                                   \
{                                                                  \
    return superClass::AbortUpdates();                             \
}                                                                  \
                                                                   \
ECode className::Deactivate()                                     \
{                                                                  \
    return superClass::Deactivate();                               \
}                                                                  \
                                                                   \
ECode className::Requery(                                          \
    /* [out] */ Boolean* succeeded)                               \
{                                                                  \
    VALIDATE_NOT_NULL(succeeded);                                  \
    return superClass::Requery(succeeded);                            \
}                                                                  \
                                                                   \
ECode className::Close()                                          \
{                                                                  \
    return superClass::Close();                                    \
}                                                                  \
                                                                   \
ECode className::IsClosed(                                         \
    /* [out] */ Boolean* isClosed)                                \
{                                                                  \
    VALIDATE_NOT_NULL(isClosed);                                   \
    *isClosed = superClass::IsClosed();                            \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::RegisterContentObserver(                          \
    /* [in] */ ILocalContentObserver* observer)                   \
{                                                                  \
    return superClass::RegisterContentObserver(observer);          \
}                                                                  \
                                                                   \
ECode className::UnregisterContentObserver(                        \
    /* [in] */ ILocalContentObserver* observer)                   \
{                                                                  \
    return superClass::UnregisterContentObserver(observer);        \
}                                                                  \
                                                                   \
ECode className::RegisterDataSetObserver(                          \
    /* [in] */ IDataSetObserver* observer)                        \
{                                                                  \
    return superClass::RegisterDataSetObserver(observer);          \
}                                                                  \
                                                                   \
ECode className::UnregisterDataSetObserver(                        \
    /* [in] */ IDataSetObserver* observer)                        \
{                                                                  \
    return superClass::UnregisterDataSetObserver(observer);        \
}                                                                  \
                                                                   \
ECode className::SetNotificationUri(                               \
    /* [in] */ IContentResolver* cr,                               \
    /* [in] */ IUri* uri)                                         \
{                                                                  \
    return superClass::SetNotificationUri(cr, uri);                \
}                                                                  \
                                                                   \
ECode className::GetWantsAllOnMoveCalls(                           \
    /* [out] */ Boolean* result)                                  \
{                                                                  \
    VALIDATE_NOT_NULL(result);                                     \
    *result = superClass::GetWantsAllOnMoveCalls();                \
                                                                   \
    return NOERROR;                                                \
}                                                                  \
                                                                   \
ECode className::GetExtras(                                        \
    /* [out] */ IBundle** extras)                                 \
{                                                                  \
    VALIDATE_NOT_NULL(extras);                                     \
    return superClass::GetExtras(extras);            \
}                                                                  \
                                                                   \
ECode className::Respond(                                          \
    /* [in] */ IBundle* extras,                                    \
    /* [out] */ IBundle** result)                                 \
{                                                                  \
    VALIDATE_NOT_NULL(result);                                     \
    return superClass::Respond(extras, result);            \
}


#endif //__CURSORMACRO_H__
