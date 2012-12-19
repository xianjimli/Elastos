
#include "database/AbstractCursor.h"
#include "database/CDataSetObservable.h"
#include "database/CContentObservable.h"
#include "utils/AutoStringArray.h"
#include <Logger.h>


using namespace Elastos::Utility::Logging;

const CString AbstractCursor::TAG = "Cursor";

AbstractCursor::AbstractCursor()
    : mRowIdColumnIndex(-1)
    , mPos(-1)
    , mCurrentRowID(-1)
    , mClosed(FALSE)
{
    CDataSetObservable::New((IDataSetObservable**)&mDataSetObservable);
    CContentObservable::New((IContentObservable**)&mContentObservable);
}

AbstractCursor::~AbstractCursor()
{
    if (mSelfObserver != NULL && mSelfObserverRegistered) {
        // mContentResolver->UnregisterContentObserver(mSelfObserver);
    }
    mUpdatedRows.Clear();
}

// TODO implement getBlob in all cursor types
ECode AbstractCursor::GetBlob(
    /* [in] */ Int32 column,
    /* [out] */ ArrayOf<Byte>** blob)
{
//    throw new UnsupportedOperationException("getBlob is not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

AutoPtr<ICursorWindow> AbstractCursor::GetWindow()
{
    return NULL;
}

Int32 AbstractCursor::GetColumnCount()
{
    ArrayOf<String>* names;
    ASSERT_SUCCEEDED(GetColumnNames(&names));
    Int32 count = names->GetLength();
    for (Int32 i = 0; i < count; ++i) {
        (*names)[i] = NULL;
    }
    ArrayOf<String>::Free(names);
    return count;
}

ECode AbstractCursor::Deactivate()
{
    return DeactivateInternal();
}

ECode AbstractCursor::DeactivateInternal()
{
    if (mSelfObserver != NULL) {
        // mContentResolver->UnregisterContentObserver(mSelfObserver);
        mSelfObserverRegistered = FALSE;
    }
    return mDataSetObservable->NotifyInvalidated();
}

ECode AbstractCursor::Requery(
    /* [out] */ Boolean* result)
{
    if (mSelfObserver != NULL && !mSelfObserverRegistered) {
        // mContentResolver->RegisterContentObserver(mNotifyUri, TRUE, mSelfObserver);
        mSelfObserverRegistered = TRUE;
    }
    mDataSetObservable->NotifyChanged();
    *result = TRUE;
    return NOERROR;
}

Boolean AbstractCursor::IsClosed()
{
    return mClosed;
}

ECode AbstractCursor::Close()
{
    mClosed = TRUE;
    FAIL_RETURN(mContentObservable->UnregisterAll());
    return DeactivateInternal();
}

// /**
//  * @hide
//  * @deprecated
//  */
// public boolean commitUpdates(Map<? extends Long,? extends Map<String,Object>> values) {
//     return false;
// }

ECode AbstractCursor::DeleteRow(
    /* [out] */ Boolean* succeeded)
{
    *succeeded = FALSE;
    return NOERROR;
}

Boolean AbstractCursor::OnMove(
    /* [in] */ Int32 oldPosition,
    /* [in] */ Int32 newPosition)
{
    return TRUE;
}

ECode AbstractCursor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buffer)
{
    // Default implementation, uses getString
    String result;
    GetString(columnIndex, &result);
    if (!result.IsNull()) {
        // char[] data = buffer.data;
        // if (data == null || data.length < result.length()) {
        //     buffer.data = result.toCharArray();
        // } else {
        //     result.getChars(0, result.length(), data, 0);
        // }
        // buffer.sizeCopied = result.length();
    }
    return NOERROR;
}

Int32 AbstractCursor::GetPosition()
{
    return mPos;
}

Boolean AbstractCursor::MoveToPosition(
    /* [in] */ Int32 position)
{
    // Make sure position isn't past the end of the cursor
    const Int32 count = GetCount();
    if(position >= count) {
        mPos = count;
        return FALSE;
    }

    // Make sure position isn't before the beginning of the cursor
    if(position < 0) {
        mPos = -1;
        return FALSE;
    }

    // Check for no-op moves, and skip the rest of the work for them
    if(position == mPos) {
        return TRUE;
    }

    Boolean result = OnMove(mPos, position);
    if(result == FALSE) {
        mPos = -1;
    }
    else {
        mPos = position;
        if(mRowIdColumnIndex != -1) {
            GetInt64(mRowIdColumnIndex, &mCurrentRowID);
        }
    }

    return result;
}

ECode AbstractCursor::FillWindow(
    /* [in] */ Int32 position,
    /* [in] */ ICursorWindow* window)
{
    if(position < 0 || position > GetCount()) {
        return NOERROR;
    }
    window->AcquireReference();
    // try {
    Int32 oldpos = mPos;
    mPos = position - 1;
    window->Clear();
    window->SetStartPosition(position);
    Int32 columnNum = GetColumnCount();
    Boolean result;
    window->SetNumColumns(columnNum, &result);
    while(MoveToNext() && (window->AllocRow(&result), result)) {
        for(Int32 i = 0; i < columnNum; i++) {
            String field;
            GetString(i, &field);
            if(!field.IsNull()) {
                window->PutString(field, mPos, i, &result);
                if(!result) {
                    window->FreeLastRow();
                    break;
                }
            }
            else {
                window->PutNull(mPos, i, &result);
                if(!result) {
                    window->FreeLastRow();
                    break;
                }
            }
        }
    }

    mPos = oldpos;
    // } catch (IllegalStateException e){
    //     // simply ignore it
    // } finally {
    window->ReleaseReference();
    // }
    return NOERROR;
}

Boolean AbstractCursor::Move(
    /* [in] */ Int32 offset)
{
    return MoveToPosition(mPos + offset);
}

Boolean AbstractCursor::MoveToFirst()
{
    return MoveToPosition(0);
}

Boolean AbstractCursor::MoveToLast()
{
    return MoveToPosition(GetCount() - 1);
}

Boolean AbstractCursor::MoveToNext()
{
    return MoveToPosition(mPos + 1);
}

Boolean AbstractCursor::MoveToPrevious()
{
    return MoveToPosition(mPos - 1);
}

Boolean AbstractCursor::IsFirst()
{
    return mPos == 0 && GetCount() != 0;
}

Boolean AbstractCursor::IsLast()
{
    Int32 cnt = GetCount();
    return mPos == (cnt - 1) && cnt != 0;
}

Boolean AbstractCursor::IsBeforeFirst()
{
    if (GetCount() == 0) {
        return TRUE;
    }
    return mPos == -1;
}

Boolean AbstractCursor::IsAfterLast()
{
    if (GetCount() == 0) {
        return TRUE;
    }
    return mPos == GetCount();
}

Int32 AbstractCursor::GetColumnIndex(
    /* [in] */ const String& _columnName)
{
    String columnName = _columnName;
    const Int32 periodIndex = columnName.LastIndexOf('.');
    if (periodIndex != -1) {
        //Exception e = new Exception();
        //Log.e(TAG, "requesting column name with table name -- " + columnName, e);
        columnName = columnName.Substring(periodIndex + 1);
    }

    AutoStringArray columnNames;
    GetColumnNames((ArrayOf<String>**)&columnNames);
    Int32 length = columnNames->GetLength();
    for(Int32 i = 0; i < length; i++) {
        if((*columnNames)[i].EqualsIgnoreCase(columnName)) {
            return i;
        }
    }

    /*
    if (Config::LOGV) {
        Int32 count;
        GetCount(&count);
        if (count > 0) {
            Logger::W("AbstractCursor", "Unknown column " + columnName);
        }
    }*/
    return -1;
}

ECode AbstractCursor::GetColumnIndexOrThrow(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* i)
{
    const Int32 index = GetColumnIndex(columnName);
    if (index < 0) {
//        throw new IllegalArgumentException("column '" + columnName + "' does not exist");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *i = index;
    return NOERROR;
}

String AbstractCursor::GetColumnName(
    /* [in] */ Int32 columnIndex)
{
    AutoStringArray columnNames;
    GetColumnNames((ArrayOf<String>**)&columnNames);
    return (*columnNames)[columnIndex];
}

/**
 * @hide
 * @deprecated
 */
Boolean AbstractCursor::UpdateBlob(
    /* [in] */ Int32 columnIndex,
    /* [in, out] */ const ArrayOf<Byte>& value)
{
    // return update(columnIndex, value);
    return FALSE;
}

/**
 * @hide
 * @deprecated
 */
Boolean AbstractCursor::UpdateString(
    /* [in] */ Int32 columnIndex,
    /* [in] */ const String& value)
{
    // return update(columnIndex, value);
    return FALSE;
}

/**
 * @hide
 * @deprecated
 */
Boolean AbstractCursor::UpdateInt16(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int16 value)
{
    // return update(columnIndex, Short.valueOf(value));
    return FALSE;
}

/**
 * @hide
 * @deprecated
 */
Boolean AbstractCursor::UpdateInt32(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int32 value)
{
    // return update(columnIndex, Integer.valueOf(value));
    return FALSE;
}

/**
 * @hide
 * @deprecated
 */
Boolean AbstractCursor::UpdateInt64(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Int64 value)
{
    // return update(columnIndex, Long.valueOf(value));
    return FALSE;
}

/**
 * @hide
 * @deprecated
 */
Boolean AbstractCursor::UpdateFloat(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Float value)
{
    // return update(columnIndex, Float.valueOf(value));
    return FALSE;
}

/**
 * @hide
 * @deprecated
 */
Boolean AbstractCursor::UpdateDouble(
    /* [in] */ Int32 columnIndex,
    /* [in] */ Double value)
{
    // return update(columnIndex, Double.valueOf(value));
    return FALSE;
}

/**
 * @hide
 * @deprecated
 */
Boolean AbstractCursor::UpdateToNull(
    /* [in] */ Int32 columnIndex)
{
    // return update(columnIndex, null);
    return FALSE;
}

/**
 * @hide
 * @deprecated
 */
// public boolean update(int columnIndex, Object obj) {
//     if (!supportsUpdates()) {
//         return false;
//     }

//     // Long.valueOf() returns null sometimes!
// //        Long rowid = Long.valueOf(getLong(mRowIdColumnIndex));
//     Long rowid = new Long(getLong(mRowIdColumnIndex));
//     if (rowid == null) {
//         throw new IllegalStateException("null rowid. mRowIdColumnIndex = " + mRowIdColumnIndex);
//     }

//     synchronized(mUpdatedRows) {
//         Map<String, Object> row = mUpdatedRows.get(rowid);
//         if (row == null) {
//             row = new HashMap<String, Object>();
//             mUpdatedRows.put(rowid, row);
//         }
//         row.put(getColumnNames()[columnIndex], obj);
//     }

//     return true;
// }

Boolean AbstractCursor::HasUpdates()
{
    Mutex::Autolock lock(mUpdatedRowsLock);

    return mUpdatedRows.Begin() != mUpdatedRows.End();
}

ECode AbstractCursor::AbortUpdates()
{
    Mutex::Autolock lock(mUpdatedRowsLock);

    mUpdatedRows.Clear();
    return NOERROR;
}

ECode AbstractCursor::CommitUpdates(
    /* [out] */ Boolean* succeeded)
{
    // return commitUpdates(null);
    return E_NOT_IMPLEMENTED;
}

Boolean AbstractCursor::SupportsUpdates()
{
    return mRowIdColumnIndex != -1;
}

ECode AbstractCursor::RegisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    return mContentObservable->RegisterObserver(observer);
}

ECode AbstractCursor::UnregisterContentObserver(
    /* [in] */ ILocalContentObserver* observer)
{
    // cursor will unregister all observers when it close
    if(!mClosed) {
        return mContentObservable->UnregisterObserver(observer);
    }
    return NOERROR;
}

void AbstractCursor::NotifyDataSetChange()
{
    mDataSetObservable->NotifyChanged();
}

AutoPtr<IDataSetObservable> AbstractCursor::GetDataSetObservable()
{
    return mDataSetObservable;
}

ECode AbstractCursor::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return mDataSetObservable->RegisterObserver(observer);
}

ECode AbstractCursor::UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer)
{
    return mDataSetObservable->UnregisterObserver(observer);
}

void AbstractCursor::OnChange(
    /* [in] */ Boolean selfChange)
{
    Mutex::Autolock lock(mSelfObserverLock);

    mContentObservable->DispatchChange(selfChange);
    if (mNotifyUri != NULL && selfChange) {
        // mContentResolver->NotifyChange(mNotifyUri, mSelfObserver);
    }
}

ECode AbstractCursor::SetNotificationUri(
     /* [in] */ IContentResolver* cr,
     /* [in] */ IUri* notifyUri)
{
    Mutex::Autolock lock(mSelfObserverLock);

    mNotifyUri = notifyUri;
    mContentResolver = cr;
    if (mSelfObserver != NULL) {
       // mContentResolver->UnregisterContentObserver(mSelfObserver);
    }
    // mSelfObserver = new SelfContentObserver(this);
    // mContentResolver->RegisterContentObserver(mNotifyUri, TRUE, mSelfObserver);
    mSelfObserverRegistered = TRUE;
    return NOERROR;
}

Boolean AbstractCursor::GetWantsAllOnMoveCalls()
{
    return FALSE;
}

ECode AbstractCursor::GetExtras(
    /* [out] */ IBundle** extras)
{
//    return Bundle.EMPTY;
    return E_NOT_IMPLEMENTED;
}

ECode AbstractCursor::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** v)
{
//    return Bundle.EMPTY;
    return E_NOT_IMPLEMENTED;
}

Boolean AbstractCursor::IsFieldUpdated(
    /* [in] */ Int32 columnIndex)
{
    if (mRowIdColumnIndex != -1 && mUpdatedRows.Begin() != mUpdatedRows.End()) {
        HashMap<Int64, Map<String, AutoPtr<IInterface> >* >::Iterator it = mUpdatedRows.Find(mCurrentRowID);
        if (it != mUpdatedRows.End()) {
            Map<String, AutoPtr<IInterface> >* updates = it->mSecond;
            AutoStringArray names;
            GetColumnNames((ArrayOf<String>**)&names);
            if (updates->Find((*names)[columnIndex]) != updates->End()) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

AutoPtr<IInterface> AbstractCursor::GetUpdatedField(
    /* [in] */ Int32 columnIndex)
{
    HashMap<Int64, Map<String, AutoPtr<IInterface> >* >::Iterator it = mUpdatedRows.Find(mCurrentRowID);
    assert(it != mUpdatedRows.End());
    Map<String, AutoPtr<IInterface> >* updates = it->mSecond;
    AutoStringArray names;
    GetColumnNames((ArrayOf<String>**)&names);
    Map<String, AutoPtr<IInterface> >::Iterator it2 = updates->Find((*names)[columnIndex]);
    if (it2 == updates->End()) return NULL;
    return it2->mSecond;
}

ECode AbstractCursor::CheckPosition()
{
    if (-1 == mPos || GetCount() == mPos) {
        //throw new CursorIndexOutOfBoundsException(mPos, getCount());
        return E_CURSOR_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}
