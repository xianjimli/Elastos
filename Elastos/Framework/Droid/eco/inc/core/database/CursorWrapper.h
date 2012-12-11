
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
    CursorWrapper();

    CARAPI Init(
        /* [in] */ ICursor* cursor);

    ~CursorWrapper();

    /**
     * @hide
     * @deprecated
     */
//    public void abortUpdates() {
//        mCursor.abortUpdates();
//    }

    CARAPI Close();

    CARAPI IsClosed(
        /* [out] */ Boolean* res);

    /**
     * @hide
     * @deprecated
     */
//    public boolean commitUpdates() {
//        return mCursor.commitUpdates();
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean commitUpdates(
//            Map<? extends Long, ? extends Map<String, Object>> values) {
//        return mCursor.commitUpdates(values);
//    }

    CARAPI GetCount(
        /* [out] */ Int32* cnt);

    CARAPI Deactivate();

    /**
     * @hide
     * @deprecated
     */
//    public boolean deleteRow() {
//        return mCursor.deleteRow();
//    }

    CARAPI MoveToFirst(
        /* [out] */ Boolean* rst);
        
    CARAPI GetColumnCount(
        /* [out] */ Int32* cnt);
        
    CARAPI GetColumnIndex(
        /* [in] */ String columnName,
        /* [out] */ Int32* index);
        
    CARAPI GetColumnIndexOrThrow(
        /* [in] */  String columnName,
        /* [out] */ Int32* columnIndex);
        
    CARAPI GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* name);
        
    CARAPI GetColumnNames(
        /* [out,callee] */ ArrayOf<String>** columnNames);

    CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* value);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* value);

    CARAPI GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* value);

    CARAPI GetInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64* value);

    CARAPI GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* value);

    CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* value);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer* buffer);

    CARAPI GetBlob(
            /* [in] */  Int32 columnIndex,
            /* [out,callee] */ ArrayOf<Byte>** blob);


    CARAPI GetWantsAllOnMoveCalls(
            /* [out] */ Boolean* value);

    /**
     * @hide
     * @deprecated
     */
//    public boolean hasUpdates() {
//        return mCursor.hasUpdates();
//    }

    CARAPI IsAfterLast(
        /* [out] */ Boolean* rst);

    CARAPI IsBeforeFirst(
        /* [out] */ Boolean* rst);

    CARAPI IsFirst(
        /* [out] */ Boolean* rst);

    CARAPI IsLast(
        /* [out] */ Boolean* rst);

    CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* rst);

    CARAPI MoveToLast(
        /* [out] */ Boolean* rst);

    CARAPI Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* rst);

    CARAPI MoveToPosition(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* rst);

    CARAPI MoveToNext(
        /* [out] */ Boolean* rst);

    CARAPI GetPosition(
        /*[out]*/ Int32* position);

    CARAPI MoveToPrevious(
        /* [out] */ Boolean* rst);

    CARAPI RegisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    CARAPI RegisterDataSetObserver(
        /* [in] */IDataSetObserver* observer);

    CARAPI Requery(
        /* [out] */ Boolean* rst);

    CARAPI Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** bundle);

    CARAPI SetNotificationUri(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IUri* uri);

    /**
     * @hide
     * @deprecated
     */
//    public boolean supportsUpdates() {
//        return mCursor.supportsUpdates();
//    }

    CARAPI UnregisterContentObserver(
        /* [in] */ ILocalContentObserver* observer);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    /**
     * @hide
     * @deprecated
     */
//    public boolean updateDouble(int columnIndex, double value) {
//        return mCursor.updateDouble(columnIndex, value);
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean updateFloat(int columnIndex, float value) {
//        return mCursor.updateFloat(columnIndex, value);
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean updateInt(int columnIndex, int value) {
//        return mCursor.updateInt(columnIndex, value);
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean updateLong(int columnIndex, long value) {
//        return mCursor.updateLong(columnIndex, value);
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean updateShort(int columnIndex, short value) {
//        return mCursor.updateShort(columnIndex, value);
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean updateString(int columnIndex, String value) {
//        return mCursor.updateString(columnIndex, value);
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean updateBlob(int columnIndex, byte[] value) {
//        return mCursor.updateBlob(columnIndex, value);
//    }

    /**
     * @hide
     * @deprecated
     */
//    public boolean updateToNull(int columnIndex) {
//        return mCursor.updateToNull(columnIndex);
//    }

private:
    AutoPtr<ICursor> mCursor;
};
#endif //_CURSORWRAPPER_H_
