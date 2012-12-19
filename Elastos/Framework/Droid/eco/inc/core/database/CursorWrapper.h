
#ifndef _CURSORWRAPPER_H_
#define _CURSORWRAPPER_H_

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * Wrapper class for Cursor that delegates all calls to the actual cursor object
 */
class CursorWrapper
{
public:
    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI AbortUpdates();

    virtual CARAPI Close();

    virtual CARAPI IsClosed(
        /* [out] */ Boolean* isClosed);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI CommitUpdates(
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
//    public boolean commitUpdates(
//            Map<? extends Long, ? extends Map<String, Object>> values) {
//        return mCursor.commitUpdates(values);
//    }

    virtual CARAPI GetCount(
        /* [out] */ Int32* count);

    virtual CARAPI Deactivate();

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI DeleteRow(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI MoveToFirst(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI GetColumnCount(
        /* [out] */ Int32* count);

    virtual CARAPI GetColumnIndex(
        /* [in] */ const String& columnName,
        /* [out] */ Int32* index);

    virtual CARAPI GetColumnIndexOrThrow(
        /* [in] */  const String& columnName,
        /* [out] */ Int32* columnIndex);

    virtual CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* name);

    virtual CARAPI GetColumnNames(
        /* [out, callee] */ ArrayOf<String>** columnNames);

    virtual CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* value);

    virtual CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    virtual CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* value);

    virtual CARAPI GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* value);

    virtual CARAPI GetInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64* value);

    virtual CARAPI GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* value);

    virtual CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* value);

    virtual CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer* buffer);

    virtual CARAPI GetBlob(
        /* [in] */  Int32 columnIndex,
        /* [out,callee] */ ArrayOf<Byte>** blob);

    virtual CARAPI GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* value);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI HasUpdates(
        /* [out] */ Boolean* result);

    virtual CARAPI IsAfterLast(
        /* [out] */ Boolean* result);

    virtual CARAPI IsBeforeFirst(
        /* [out] */ Boolean* result);

    virtual CARAPI IsFirst(
        /* [out] */ Boolean* result);

    virtual CARAPI IsLast(
        /* [out] */ Boolean* result);

    virtual CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* result);

    virtual CARAPI MoveToLast(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* succeeded);

    virtual CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* succeeded);

    virtual CARAPI MoveToNext(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI GetPosition(
        /*[out]*/ Int32* position);

    virtual CARAPI MoveToPrevious(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI RegisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    virtual CARAPI RegisterDataSetObserver(
        /* [in] */IDataSetObserver* observer);

    virtual CARAPI Requery(
        /* [out] */ Boolean* succeeded);

    virtual CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** bundle);

    virtual CARAPI SetNotificationUri(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI SupportsUpdates(
        /* [out] */ Boolean* result);

    virtual CARAPI UnregisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    virtual CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI UpdateDouble(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Double value,
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI UpdateFloat(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Float value,
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI UpdateInt32(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 value,
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI UpdateInt64(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int64 value,
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI UpdateInt16(
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int16 value,
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI UpdateString(
        /* [in] */ Int32 columnIndex,
        /* [in] */ const String& value,
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI UpdateBlob(
        /* [in] */ Int32 columnIndex,
        /* [in, out] */ const ArrayOf<Byte>& value,
        /* [out] */ Boolean* succeeded);

    /**
     * @hide
     * @deprecated
     */
    virtual CARAPI UpdateToNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* succeeded);

protected:
    CursorWrapper();

    CARAPI Init(
        /* [in] */ ICursor* cursor);

private:
    AutoPtr<ICursor> mCursor;
};

#endif //_CURSORWRAPPER_H_
