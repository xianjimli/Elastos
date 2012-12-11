#include "database/AbstractCursor.h"
#include <Logger.h>


using namespace Elastos::Utility::Logging;

const String AbstractCursor::TAG = String("Cursor");
AbstractCursor::AbstractCursor()
{
    mPos = -1;
    mRowIdColumnIndex = -1;
    mClosed = FALSE;
    mCurrentRowID = NULL;
    mUpdatedRows = new HashMap<Int64, Map<String, AutoPtr<IInterface> > > ();
}

AbstractCursor::~AbstractCursor()
{
    if (mSelfObserver != NULL && mSelfObserverRegistered == TRUE) {
//        mContentResolver->UnregisterContentObserver(mSelfObserver);
    }
}

ECode AbstractCursor::GetBlob(
        /* [in] */ Int32 column,
        /* [out] */ ArrayOf<Byte>** blob)
{
//    throw new UnsupportedOperationException("getBlob is not supported");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode AbstractCursor::GetWindow(
        /* [out] */ ICursorWindow** window)
{
    *window = NULL;
    return NOERROR;
}

ECode AbstractCursor::GetColumnCount(
        /* [out] */ Int32* count)
{
    assert(count != NULL);
    ArrayOf<String> *names;
    FAIL_RETURN(GetColumnNames(&names));
    *count = names->GetLength();
    return NOERROR;
}

ECode AbstractCursor::Deactivate()
{
    FAIL_RETURN(DeactivateInternal());
    return NOERROR;
}

ECode AbstractCursor::DeactivateInternal()
{
    if (mSelfObserver != NULL) {
//        mContentResolver->UnregisterContentObserver(mSelfObserver);
        mSelfObserverRegistered = FALSE;
    }
    mDataSetObservable->NotifyInvalidated();
    return NOERROR;
}

Boolean AbstractCursor::Requery()
{
    if (mSelfObserver != NULL) {
//        mContentResolver->RegisterContentObserver(mNotifyUri, TRUE, mSelfObserver);
        mSelfObserverRegistered = TRUE;
    }
    mDataSetObservable->NotifyChanged();
    return TRUE;
}

Boolean AbstractCursor::IsClosed()
{
    return mClosed;
}

ECode AbstractCursor::Close()
{
    mClosed = TRUE;
    FAIL_RETURN(mContentObservable->UnregisterAll());
    FAIL_RETURN(DeactivateInternal());
    return NOERROR;
}

ECode AbstractCursor::OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean* value)
{
    assert(value != NULL);
    *value = TRUE;
    return NOERROR;
}

ECode AbstractCursor::CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ CharArrayBuffer* buffer)
{
    String result = String::FromInt32(columnIndex);
    if (!result.IsNull()) {
        ArrayOf<Char8>* data = buffer->data;
        char* cstr = (char*)&result;
        if (data == NULL || data->GetLength() < (Int32)result.GetLength()) {
            buffer->data = ArrayOf<Char8>::Alloc(cstr, strlen(cstr));
        } else {
            for(Int32 i = 0; i < (Int32)strlen(cstr); i++) {
                (*data)[i] = *cstr;
                cstr++;
            }
        }
        buffer->sizeCopied = (Int32)result.GetLength();
    }
    return NOERROR;
}

ECode AbstractCursor::GetPosition(
        /* [out] */ Int32* position)
{
    assert(position != NULL);
    *position = mPos;
    return NOERROR;
}

Boolean AbstractCursor::MoveToPosition(
    /* [in] */ Int32 position)
{
    // Make sure position isn't past the end of the cursor
    Int32 cnt;
    FAIL_RETURN(GetCount(&cnt));
    const Int32 count = cnt;
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

    Boolean result;
    FAIL_RETURN(OnMove(mPos, position, &result));
    if(result == FALSE) {
        mPos = -1;
    } else {
        mPos = position;
        if(mRowIdColumnIndex != -1) {
            mCurrentRowID = mRowIdColumnIndex;
        }
    }
    return result;
}

ECode AbstractCursor::FillWindow(
        /* [in] */ Int32 position,
        /* [in] */ ICursorWindow* window)
{
    Int32 cnt;
    FAIL_RETURN(GetCount(&cnt));
    if(position < 0 || position > cnt) {
        window = NULL;
        return NOERROR;
    }
    FAIL_RETURN(window->AcquireReference());

    Int32 oldpos = mPos;
    mPos = position - 1;
    FAIL_RETURN(window->Clear());
    Int32 columnNum;
    FAIL_RETURN(GetColumnCount(&columnNum));
    Boolean r1, r2, r3;
    FAIL_RETURN(window->SetNumColumns(columnNum, &r1));

    FAIL_RETURN(MoveToNext(&r2));
    FAIL_RETURN(window->AllocRow(&r3));
    while(r2 && r3) {
        for(Int32 i = 0; i < columnNum; i++) {
            String field = String::FromInt32(i);
            if(!field.IsNull()) {
                Boolean rst;
                FAIL_RETURN(window->PutString(field, mPos, i, &rst));
                if(!rst) {
                    FAIL_RETURN(window->FreeLastRow());
                    break;
                }
            } else {
                Boolean rst;
                FAIL_RETURN(window->PutNull(mPos, i, &rst));
                if(!rst) {
                    FAIL_RETURN(window->FreeLastRow());
                    break;
                }
            }
        }
    }
    mPos = oldpos;
    return NOERROR;
}

ECode AbstractCursor::Move(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    *rst = MoveToPosition(mPos + offset);
    return NOERROR;
}

ECode AbstractCursor::MoveToFirst(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    *rst = MoveToPosition(0);
    return NOERROR;
}

ECode AbstractCursor::MoveToLast(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    Int32 cnt;
    FAIL_RETURN(GetCount(&cnt));
    *rst = MoveToPosition(cnt - 1);
    return NOERROR;
}

ECode AbstractCursor::MoveToNext(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    *rst = MoveToPosition(mPos + 1);
    return NOERROR;
}

ECode AbstractCursor::MoveToPrevious(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    *rst = MoveToPosition(mPos - 1);
    return NOERROR;
}

ECode AbstractCursor::IsFirst(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    Int32 cnt;
    FAIL_RETURN(GetCount(&cnt));
    *rst = (mPos == 0 && cnt != 0) ? TRUE : FALSE;
    return NOERROR;
}

ECode AbstractCursor::IsLast(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    Int32 cnt;
    FAIL_RETURN(GetCount(&cnt));
    *rst = (mPos == (cnt - 1) && cnt != 0) ? TRUE : FALSE;
    return NOERROR;
}

ECode AbstractCursor::IsBeforeFirst(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    Int32 cnt;
    FAIL_RETURN(GetCount(&cnt));
    if (cnt == 0) {
        *rst = TRUE;
        return NOERROR;
    }
    *rst = (mPos == -1) ? TRUE : FALSE;
    return NOERROR;
}

ECode AbstractCursor::IsAfterLast(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    Int32 cnt;
    FAIL_RETURN(GetCount(&cnt));
    if (cnt == 0) {
        *rst = TRUE;
        return NOERROR;
    }
    *rst = (mPos == cnt) ? TRUE : FALSE;
    return NOERROR;
}

ECode AbstractCursor::GetColumnIndex(
        /* [in] */ String columnName,
        /* [out] */ Int32* index)
{
    assert(index != NULL);
    const Int32 periodIndex = columnName.LastIndexOf('.');
    if (periodIndex != -1) {
        //Exception e = new Exception();
        //Log.e(TAG, "requesting column name with table name -- " + columnName, e);
        columnName = columnName.Substring(periodIndex + 1);
    }
    ArrayOf<String>* columnNames;
    GetColumnNames(&columnNames);
    Int32 length = columnNames->GetLength();

    for(Int32 i = 0; i < length; i++) {
        if((*columnNames)[i].EqualsIgnoreCase(columnName)) {
            *index = i;
            return NOERROR;
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
    *index = -1;
    return NOERROR;
}

ECode AbstractCursor::GetColumnIndexOrThrow(
        /* [in] */ String columnName,
        /* [out] */ Int32* i)
{
    Int32 j;
    FAIL_RETURN(GetColumnIndex(columnName, &j));
    const Int32 index = j;
    if (index < 0) {
//        throw new IllegalArgumentException("column '" + columnName + "' does not exist");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *i = index;
    return NOERROR;
}

ECode AbstractCursor::GetColumnName(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* name)
{
    assert(name != NULL);
    ArrayOf<String>* columnNames;
    GetColumnNames(&columnNames);
    *name = (*columnNames)[columnIndex];
    return NOERROR;
}

ECode AbstractCursor::RegisterContentObserver(
        /* [in] */ ILocalContentObserver* observer)
{
    FAIL_RETURN(mContentObservable->RegisterObserver(observer));
    return NOERROR;
}

ECode AbstractCursor::UnregisterContentObserver(
        /* [in] */ ILocalContentObserver* observer)
{
    if(!mClosed) {
        FAIL_RETURN(mContentObservable->UnregisterObserver(observer));
    }
    return NOERROR;
}

ECode AbstractCursor::RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer)
{
    FAIL_RETURN(mDataSetObservable->RegisterObserver(observer));
    return NOERROR;
}

ECode AbstractCursor::UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer)
{
    FAIL_RETURN(mDataSetObservable->UnregisterObserver(observer));
    return NOERROR;
}

ECode AbstractCursor::SetNotificationUri(
         /* [in] */ IContentResolver* cr,
         /* [in] */ IUri* notifyUri)
{
    Mutex::Autolock lock(mSelfObserverLock);
//    mNotifyUri = notifyUri;
//    mContentResolver = cr;
//    if (mSelfObserver != NULL) {
//        mContentResolver->UnregisterContentObserver(mSelfObserver);
//    }
//    mSelfObserver = new SelfContentObserver(this);
//    mContentResolver.registerContentObserver(mNotifyUri, true, mSelfObserver);
//    mSelfObserverRegistered = true;
    return E_NOT_IMPLEMENTED;
}

ECode AbstractCursor::GetWantsAllOnMoveCalls(
        /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode AbstractCursor::GetExtras(
        /* [out] */ IBundle** extras)
{
//    return Bundle.EMPTY;
    return E_NOT_IMPLEMENTED;
}

ECode AbstractCursor::Respond(
        /* [in] */ IBundle* extras,
        /* [out] */ IBundle** result)
{
//    return Bundle.EMPTY;
    return E_NOT_IMPLEMENTED;
}

ECode AbstractCursor::NotifyDataSetChange()
{
    FAIL_RETURN(mDataSetObservable->NotifyChanged());
    return NOERROR;
}

ECode AbstractCursor::GetDataSetObservable(
        /* [out] */ DataSetObservable** dso)
{
    *dso = mDataSetObservable;
    return NOERROR;
}

ECode AbstractCursor::OnChange(
        /* [in] */ Boolean selfChange)
{
    Mutex::Autolock lock(mSelfObserverLock);
    mContentObservable->DispatchChange(selfChange);
    if (mNotifyUri != NULL && selfChange) {
//        mContentResolver.notifyChange(mNotifyUri, mSelfObserver);
    }
    return E_NOT_IMPLEMENTED;
}

ECode AbstractCursor::IsFieldUpdated(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    if (mRowIdColumnIndex != -1 && mUpdatedRows->GetSize() > 0) {
        HashMap<Int64, Map<String, AutoPtr<IInterface> > >::Iterator find = mUpdatedRows->Find(mCurrentRowID);
        Map<String, AutoPtr<IInterface> > updates;
        if (find != mUpdatedRows->End()) {
            updates = find->mSecond;
        }
        ArrayOf<String>* names;
        FAIL_RETURN(GetColumnNames(&names));
        Map<String, AutoPtr<IInterface> >::Iterator secFind = updates.Find((*names)[columnIndex]);
        Boolean result = (secFind != updates.End()) ? TRUE : FALSE;
        if (!updates.IsEmpty() && result) {
            *rst = TRUE;
            return NOERROR;
        }
    }
    *rst = FALSE;
    return NOERROR;
}

ECode AbstractCursor::GetUpdatedField(
        /* [in] */ Int32 columnIndex,
        /* [out] */ IInterface** obj)
{
    HashMap<Int64, Map<String, AutoPtr<IInterface> > >::Iterator find = mUpdatedRows->Find(mCurrentRowID);
    Map<String, AutoPtr<IInterface> > updates;
    if (find != mUpdatedRows->End()) {
        updates = find->mSecond;
    }
    ArrayOf<String>* names;
    FAIL_RETURN(GetColumnNames(&names));
    Map<String, AutoPtr<IInterface> >::Iterator secFind = updates.Find((*names)[columnIndex]);
    if (secFind != updates.End()) {
        *obj = secFind->mSecond;
    }
    return NOERROR;
}

ECode AbstractCursor::CheckPosition()
{
    Int32 cnt;
    FAIL_RETURN(GetCount(&cnt));
    if (-1 == mPos || cnt == mPos) {
        //throw new CursorIndexOutOfBoundsException(mPos, getCount());
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}
