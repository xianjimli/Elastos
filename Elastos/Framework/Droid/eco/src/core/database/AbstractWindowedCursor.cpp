
#include "database/AbstractWindowedCursor.h"

ECode AbstractWindowedCursor::GetBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ ArrayOf<Byte>** blob)
{
    CheckPosition();

    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
        GetUpdatedField(columnIndex, (IInterface**)blob);
        return NOERROR;
    }
    mWindow->GetBlob(mPos, columnIndex, blob);
    return NOERROR;
}

ECode AbstractWindowedCursor::GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* value)
{
    assert(value != NULL);

    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
        GetUpdatedField(columnIndex, (IInterface**)&value);
        return NOERROR;
    }
    mWindow->GetString(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ CharArrayBuffer* buffer)
{
    FAIL_RETURN(CheckPosition());
    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
    	AbstractCursor::CopyStringToBuffer(columnIndex, buffer);
    }
    mWindow->CopyStringToBuffer(mPos, columnIndex, (ICharArrayBuffer*)buffer);
    return NOERROR;
}

ECode AbstractWindowedCursor::GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
        if (rst) {
            GetUpdatedField(columnIndex, (IInterface**)value);
            value = (Int16*)value;
            return NOERROR;
        }

    mWindow->GetInt32(mPos, columnIndex, (Int32*)value);
    return NOERROR;
}

ECode AbstractWindowedCursor::GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
        GetUpdatedField(columnIndex, (IInterface**)value);
        value = (Int32*)value;
        return NOERROR;
    }

    mWindow->GetInt32(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
            GetUpdatedField(columnIndex, (IInterface**)value);
            value = (Int64*)value;
            return NOERROR;
        }

    mWindow->GetInt64(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
            GetUpdatedField(columnIndex, (IInterface**)value);
            value = (Float*)value;
            return NOERROR;
        }

    mWindow->GetFloat(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* value)
{
    FAIL_RETURN(CheckPosition());

    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
            GetUpdatedField(columnIndex, (IInterface**)value);
            value = (Double*)value;
            return NOERROR;
        }

    mWindow->GetDouble(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::CheckPosition()
{
    FAIL_RETURN(CheckPosition());
    if (mWindow == NULL) {
//        throw new StaleDataException("Access closed cursor");
    	return NULL;
    }
    return NOERROR;
}

ECode AbstractWindowedCursor::IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value)
{
    FAIL_RETURN(CheckPosition());
    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
    	AutoPtr<IInterface> i;
    	GetUpdatedField(columnIndex, (IInterface**)&i);
    	*value = (i == NULL) ? TRUE : FALSE;
    	return NOERROR;
    }
    mWindow->IsNull(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::IsBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value)
{
    FAIL_RETURN(CheckPosition());
    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
    	AutoPtr<IInterface> object;
    	GetUpdatedField(columnIndex, (IInterface**)&object);
//    	*value = ((object == NULL) || (object->Probe(EIID_ArrayOf<Byte>))) ? TRUE : FALSE;
    	return NOERROR;
    }
    mWindow->IsBlob(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::IsString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value)
{
    FAIL_RETURN(CheckPosition());
    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
    	AutoPtr<IInterface> object;
    	GetUpdatedField(columnIndex, (IInterface**)&object);
//    	*value = ((object == NULL) || (object->Probe(EIID_String))) ? TRUE : FALSE;
    	return NOERROR;
    }
    mWindow->IsString(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::IsInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value)
{
    FAIL_RETURN(CheckPosition());
    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
    	AutoPtr<IInterface> object;
    	GetUpdatedField(columnIndex, (IInterface**)&object);
//    	*value = ((object == NULL) || ((object->Probe(EIID_Int32)) || (object->Probe(EIID_Int64)))) ? TRUE : FALSE;
    	return NOERROR;
    }
    mWindow->IsInt64(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::IsFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value)
{
    FAIL_RETURN(CheckPosition());
    Mutex::Autolock lock(mUpdatedRowsLock);
    Boolean rst;
    IsFieldUpdated(columnIndex, &rst);
    if (rst) {
    	AutoPtr<IInterface> object;
    	GetUpdatedField(columnIndex, (IInterface**)&object);
//    	*value = ((object == NULL) || ((object->Probe(EIID_Float)) || (object->Probe(EIID_Double)))) ? TRUE : FALSE;
    	return NOERROR;
    }
    mWindow->IsFloat(mPos, columnIndex, value);
    return NOERROR;
}

ECode AbstractWindowedCursor::GetWindow(
        /* [out] */ ICursorWindow** window)
{
    *window = mWindow;
    return NOERROR;
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

ECode AbstractWindowedCursor::HasWindow(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    *rst = (mWindow != NULL) ? TRUE : FALSE;
    return NOERROR;
}
