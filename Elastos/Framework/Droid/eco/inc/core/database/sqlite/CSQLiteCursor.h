
#ifndef __CSQLITECURSOR_H__
#define __CSQLITECURSOR_H__

#include "_CSQLiteCursor.h"

CarClass(CSQLiteCursor)
{
public:
    CARAPI constructor(
        /* [in] */ ISQLiteDatabase* db,
        /* [in] */ ISQLiteCursorDriver* driver,
        /* [in] */ const String& editTable,
        /* [in] */ ISQLiteQuery* query);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI GetPosition(
        /* [out] */ Int32* position);

    CARAPI Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* value);

    CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* value);

    CARAPI MoveToFirst(
        /* [out] */ Boolean* value);

    CARAPI MoveToLast(
        /* [out] */ Boolean* value);

    CARAPI MoveToNext(
        /* [out] */ Boolean* value);

    CARAPI MoveToPrevious(
        /* [out] */ Boolean* value);

    CARAPI IsFirst(
        /* [out] */ Boolean* value);

    CARAPI IsLast(
        /* [out] */ Boolean* value);

    CARAPI IsBeforeFirst(
        /* [out] */ Boolean* value);

    CARAPI IsAfterLast(
        /* [out] */ Boolean* value);

    //@Deprecated
    CARAPI DeleteRow(
        /* [out] */ Boolean* result);

    CARAPI GetColumnIndex(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* columnIndex);

    CARAPI GetColumnIndexOrThrow(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* columnIndex);

    CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* columnName);

    CARAPI GetColumnNames(
        /* [out,callee] */ ArrayOf<String>** columnNames);

    CARAPI GetColumnCount(
        /* [out] */ Int32* columnCount);

    CARAPI GetBlob(
        /* [in] */ Int32 columnIndex,
        /* [out,callee] */ ArrayOf<Byte>** blob);

    CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* columnValue);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in, out] */ ICharArrayBuffer* buf);

    CARAPI GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* columnValue);

    CARAPI GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* columnValue);

    CARAPI GetInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64* columnValue);

    CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* columnValue);

    CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* columnValue);

    CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [o ut] */ Boolean* value);

    //@Deprecated
    CARAPI SupportsUpdates(
        /* [out] */ Boolean* result);

    //@Deprecated
    CARAPI HasUpdates(
        /* [out] */ Boolean* result);

    //@Deprecated
    CARAPI UpdateBlob(
        /* [in] */ Int32 columnIndex,
        /* [in] */ const ArrayOf<Byte>& value,
        /* [out] */ Boolean* succeeded);

    //@Deprecated
    CARAPI UpdateString(
        /* [in] */ Int32 columnIndex,
        /* [in] */ const String& value,
        /* [out] */ Boolean* succeeded);

    //@Deprecated
    CARAPI UpdateInt16(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int16 value,
        /* [out] */ Boolean* succeeded);

    //@Deprecated
    CARAPI UpdateInt32(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 value,
        /* [out] */ Boolean* succeeded);

    //Deprecated
    CARAPI UpdateInt64(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int64 value,
        /* [out] */ Boolean* succeeded);

    //@Deprecated
    CARAPI UpdateFloat(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Float value,
        /* [out] */ Boolean* succeeded);

    //@Deprecated
    CARAPI UpdateDouble(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Double value,
        /* [out] */ Boolean* succeeded);

    //@Deprecated
    CARAPI UpdateToNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* succeeded);

    //@Deprecated
    CARAPI CommitUpdates(
        /* [out] */ Boolean* succeeded);

//        @Deprecated
//        boolean commitUpdates(Map<? extends Long,
//                ? extends Map<String,Object>> values);

    //@Deprecated
    CARAPI AbortUpdates();

    CARAPI Deactivate();

    CARAPI Requery(
        /* [out] */ Boolean* succeeded);

    CARAPI Close();

    CARAPI IsClosed(
        /* [out] */ Boolean* closed);

    CARAPI RegisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    CARAPI UnregisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI SetNotificationUri(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri);

    CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** result);

    CARAPI GetWindow(
        /* [out] */ ICursorWindow** window);

    CARAPI FillWindow(
        /* [in] */ Int32 pos,
        /* [in] */ ICursorWindow* window);

    CARAPI OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean* succeeded);

    CARAPI SetLoadStyle(
        /* [in] */ Int32 initialRead,
        /* [in] */ Int32 maxRead);

    CARAPI GetDatabase(
        /* [out] */ ISQLiteDatabase** database);

    CARAPI SetSelectionArguments(
        /* [in] */ ArrayOf<String>* selectionArgs);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSQLITECURSOR_H__
