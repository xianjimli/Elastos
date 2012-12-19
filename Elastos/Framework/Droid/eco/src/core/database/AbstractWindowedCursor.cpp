
#include "ext/frameworkdef.h"
#include "database/AbstractWindowedCursor.h"


ECode AbstractWindowedCursor::GetBlob(
    /* [in] */ Int32 columnIndex,
    /* [out] */ ArrayOf<Byte>** blob)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // return (byte[])getUpdatedField(columnIndex);
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->GetBlob(mPos, columnIndex, blob);
}

ECode AbstractWindowedCursor::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* str)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // return (String)getUpdatedField(columnIndex);
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->GetString(mPos, columnIndex, str);
}

ECode AbstractWindowedCursor::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buffer)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
    	   AbstractCursor::CopyStringToBuffer(columnIndex, buffer);
    }

    return mWindow->CopyStringToBuffer(mPos, columnIndex, buffer);
}

ECode AbstractWindowedCursor::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Number value = (Number)getUpdatedField(columnIndex);
        // return value.shortValue();
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->GetInt16(mPos, columnIndex, value);
}

ECode AbstractWindowedCursor::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Number value = (Number)getUpdatedField(columnIndex);
        // return value.intValue();
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->GetInt32(mPos, columnIndex, value);
}

ECode AbstractWindowedCursor::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Number value = (Number)getUpdatedField(columnIndex);
        // return value.longValue();
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->GetInt64(mPos, columnIndex, value);
}

ECode AbstractWindowedCursor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Number value = (Number)getUpdatedField(columnIndex);
        // return value.floatValue();
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->GetFloat(mPos, columnIndex, value);
}

ECode AbstractWindowedCursor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Number value = (Number)getUpdatedField(columnIndex);
        // return value.doubleValue();
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->GetDouble(mPos, columnIndex, value);
}

ECode AbstractWindowedCursor::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isNull)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        	// return getUpdatedField(columnIndex) == null;
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->IsNull(mPos, columnIndex, isNull);
}

ECode AbstractWindowedCursor::IsBlob(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isBlob)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Object object = getUpdatedField(columnIndex);
        // return object == null || object instanceof byte[];
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->IsBlob(mPos, columnIndex, isBlob);
}

ECode AbstractWindowedCursor::IsString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isString)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Object object = getUpdatedField(columnIndex);
        // return object == null || object instanceof String;
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->IsString(mPos, columnIndex, isString);
}

ECode AbstractWindowedCursor::IsInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isInt64)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Object object = getUpdatedField(columnIndex);
        // return object != null && (object instanceof Integer || object instanceof Long);
        return E_NOT_IMPLEMENTED;
    }

    return mWindow->IsInt64(mPos, columnIndex, isInt64);
}

ECode AbstractWindowedCursor::IsFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* isFloat)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);

    if (IsFieldUpdated(columnIndex)) {
        // Object object = getUpdatedField(columnIndex);
        // return object != null && (object instanceof Float || object instanceof Double);
    }

    return mWindow->IsFloat(mPos, columnIndex, isFloat);
}

ECode AbstractWindowedCursor::CheckPosition()
{
    FAIL_RETURN(AbstractCursor::CheckPosition());

    if (mWindow == NULL) {
//        throw new StaleDataException("Access closed cursor");
        return E_STALE_DATA_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<ICursorWindow> AbstractWindowedCursor::GetWindow()
{
    return mWindow;
}

ECode AbstractWindowedCursor::SetWindow(
    /* [in] */ ICursorWindow* window)
{
    if (mWindow != NULL) {
        mWindow->Close();
    }
    mWindow = window;
    return NOERROR;
}

Boolean AbstractWindowedCursor::HasWindow()
{
    return mWindow != NULL;
}
