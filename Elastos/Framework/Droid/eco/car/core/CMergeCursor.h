
#ifndef __CMERGECURSOR_H__
#define __CMERGECURSOR_H__

#include "_CMergeCursor.h"
#include "database/MergeCursor.h"
CarClass(CMergeCursor), public MergeCursor
{
public:
    CARAPI GetCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetPosition(
        /* [out] */ Int32 * pPosition);

    CARAPI Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean * pValue);

    CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean * pValue);

    CARAPI MoveToFirst(
        /* [out] */ Boolean * pValue);

    CARAPI MoveToLast(
        /* [out] */ Boolean * pValue);

    CARAPI MoveToNext(
        /* [out] */ Boolean * pValue);

    CARAPI MoveToPrevious(
        /* [out] */ Boolean * pValue);

    CARAPI IsFirst(
        /* [out] */ Boolean * pValue);

    CARAPI IsLast(
        /* [out] */ Boolean * pValue);

    CARAPI IsBeforeFirst(
        /* [out] */ Boolean * pValue);

    CARAPI IsAfterLast(
        /* [out] */ Boolean * pValue);

    CARAPI DeleteRow(
        /* [out] */ Boolean * pResult);

    CARAPI GetColumnIndex(
        /* [in] */ const String& columnName,
        /* [out] */ Int32 * pColumnIndex);

    CARAPI GetColumnIndexOrThrow(
        /* [in] */ const String& columnName,
        /* [out] */ Int32 * pColumnIndex);

    CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String * pColumnName);

    CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String> ** ppColumnNames);

    CARAPI GetColumnCount(
        /* [out] */ Int32 * pColumnCount);

    CARAPI GetBlob(
        /* [in] */ Int32 columnIndex,
        /* [out, callee] */ ArrayOf<Byte> ** ppBlob);

    CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String * pColumnValue);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer * pBuf);

    CARAPI GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16 * pColumnValue);

    CARAPI GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32 * pColumnValue);

    CARAPI GetInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64 * pColumnValue);

    CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float * pColumnValue);

    CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double * pColumnValue);

    CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean * pValue);

    CARAPI SupportsUpdates(
        /* [out] */ Boolean * pResult);

    CARAPI HasUpdates(
        /* [out] */ Boolean * pResult);

    CARAPI UpdateBlob(
        /* [in] */ Int32 columnIndex,
        /* [in] */ const ArrayOf<Byte> & value,
        /* [out] */ Boolean * pSucceeded);

    CARAPI UpdateString(
        /* [in] */ Int32 columnIndex,
        /* [in] */ const String& value,
        /* [out] */ Boolean * pSucceeded);

    CARAPI UpdateInt16(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int16 value,
        /* [out] */ Boolean * pSucceeded);

    CARAPI UpdateInt32(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 value,
        /* [out] */ Boolean * pSucceeded);

    CARAPI UpdateInt64(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int64 value,
        /* [out] */ Boolean * pSucceeded);

    CARAPI UpdateFloat(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Float value,
        /* [out] */ Boolean * pSucceeded);

    CARAPI UpdateDouble(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Double value,
        /* [out] */ Boolean * pSucceeded);

    CARAPI UpdateToNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean * pSucceeded);

    CARAPI CommitUpdates(
        /* [out] */ Boolean * pSucceeded);

    CARAPI AbortUpdates();

    CARAPI Deactivate();

    CARAPI Requery(
        /* [out] */ Boolean * pSucceeded);

    CARAPI Close();

    CARAPI IsClosed(
        /* [out] */ Boolean * pClosed);

    CARAPI RegisterContentObserver(
        /* [in] */ ILocalContentObserver * pObserver);

    CARAPI UnregisterContentObserver(
        /* [in] */ ILocalContentObserver * pObserver);

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver * pObserver);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver * pObserver);

    CARAPI SetNotificationUri(
        /* [in] */ IContentResolver * pCr,
        /* [in] */ IUri * pUri);

    CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean * pResult);

    CARAPI GetExtras(
        /* [out] */ IBundle ** ppExtras);

    CARAPI Respond(
        /* [in] */ IBundle * pExtras,
        /* [out] */ IBundle ** ppResult);

    CARAPI GetWindow(
        /* [out] */ ICursorWindow ** ppWindow);

    CARAPI FillWindow(
        /* [in] */ Int32 pos,
        /* [in] */ ICursorWindow * pWindow);

    CARAPI OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean * pSucceeded);

    CARAPI NotifyDataSetChange();

    CARAPI GetDataSetObservable(
        /* [out] */ IDataSetObservable ** ppDso);

    CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    CARAPI IsFieldUpdated(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean * pRst);

    CARAPI GetUpdatedField(
        /* [in] */ Int32 columnIndex,
        /* [out] */ IInterface ** ppObj);

    CARAPI CheckPosition();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const ArrayOf<ICursor *> & cursors);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMERGECURSOR_H__
