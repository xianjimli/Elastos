
#include "database/CursorWindow.h"

CursorWindow::CursorWindow()
{
}

CursorWindow::~CursorWindow()
{
}

ECode CursorWindow::OnAllReferencesReleased()
{
    return E_NOT_IMPLEMENTED;
}

CARAPI CursorWindow::Init(
        /* [in] */ Boolean localWindow)
{
    mStartPos = 0;
    Native_Init(localWindow);
    return NOERROR;
}

CARAPI CursorWindow::Init(
        /* [in] */ IParcel* source)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::GetStartPosition(
        /* [out] */ Int32* pos)
{
    assert(pos != NULL);
    *pos = mStartPos;
    return NOERROR;
}

ECode CursorWindow::SetStartPosition(
        /* [in] */ Int32 pos)
{
    mStartPos = pos;
    return NOERROR;
}

ECode CursorWindow::GetNumRows(
        /* [out] */ Int32* num)
{
    assert(num != NULL);
    AcquireReference();
    //try {
        GetNumRows_native(num);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;    
}

ECode CursorWindow::SetNumColumns(
        /* [in] */ Int32 columnNum,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        SetNumColumns_native(columnNum, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::AllocRow(
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        AllocRow_native(rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::FreeLastRow()
{
    AcquireReference();
    //try {
        FreeLastRow_native();
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::PutBlob(
        /* [in] */ const ArrayOf<Byte> & value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        PutBlob_native(value, row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::PutString(
        /* [in] */ String value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        PutString_native(value, row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::PutInt64(
        /* [in] */ Int64 value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        PutInt64_native(value, row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::PutDouble(
        /* [in] */ Double value,
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        PutDouble_native(value, row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::PutNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        PutNull_native(row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::IsNull(
        /* [in] */ Int32 row,
        /* [in] */ Int32 col,
        /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        IsNull_native(row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::GetBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ ArrayOf<Byte>** blob)
{
    AcquireReference();
    //try {
        GetBlob_native(row - mStartPos, col, blob);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::IsBlob(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        IsBlob_native(row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;  
}

ECode CursorWindow::IsInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        IsInteger_native(row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::IsFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        IsFloat_native(row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::IsString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        IsString_native(row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::GetString(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ String* rst)
{
    assert(rst != NULL);
    AcquireReference();
    //try {
        GetString_native(row - mStartPos, col, rst);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::CopyStringToBuffer(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ ICharArrayBuffer* buffer)
{
    if (buffer == NULL) {
        //throw new IllegalArgumentException("CharArrayBuffer should not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    ArrayOf<Char8>* data;
    buffer->GetData(&data);
    if (data == NULL) {
        data = ArrayOf<Char8>::Alloc(64);
    }
    AcquireReference();
    //try {
        ArrayOf<Char8>* newbuf;
        CopyStringToBuffer_native(row - mStartPos, col, data->GetLength(), buffer, &newbuf);
        if (newbuf != NULL) {
            buffer->SetData(*newbuf);
        }
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::GetInt64(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int64* value)
{
    assert(value != NULL);
    AcquireReference();
    //try {
        GetInt64_native(row - mStartPos, col, value);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::GetDouble(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Double* value)
{
    assert(value != NULL);
    AcquireReference();
    //try {
        GetDouble_native(row - mStartPos, col, value);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::GetInt16(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int16* value)
{
    assert(value != NULL);
    AcquireReference();
    //try {
        GetInt64_native(row - mStartPos, col, (Int64*)value);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::GetInt32(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    AcquireReference();
    //try {
        GetInt64_native(row - mStartPos, col, (Int64*)value);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::GetFloat(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Float* value)
{
    assert(value != NULL);
    AcquireReference();
    //try {
        GetDouble_native(row - mStartPos, col, (Double*)value);
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::Clear()
{
    AcquireReference();
    //try {
        mStartPos = 0;        
        Clear_native();
    //} finally {
    //    releaseReference();
    //}
    return NOERROR;
}

ECode CursorWindow::Close()
{
    FAIL_RETURN(ReleaseReference());
    return NOERROR;
}

ECode CursorWindow::NewFromParcel(
    /* [in] */ IParcel* p,
    /* [out] */ ICursorWindow** cw)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::DescribeContents(
        /* [out] */ Int32* value)
{
    assert(value != NULL);
    *value = 0;
    return NOERROR;
}

ECode CursorWindow::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::GetNumRows_native(
    /* [out] */ Int32* num)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::SetNumColumns_native(
    /* [in] */ Int32 columnNum,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::AllocRow_native(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::FreeLastRow_native()
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::PutBlob_native(
    /* [in] */ const ArrayOf<Byte> & value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::PutString_native(
    /* [in] */ String value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::PutInt64_native(
    /* [in] */ Int64 value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::PutDouble_native(
    /* [in] */ Double value,
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::PutNull_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::IsNull_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::GetBlob_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ ArrayOf<Byte>** blob)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::IsBlob_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::IsInteger_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::IsFloat_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::IsString_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Boolean* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::GetString_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ String* rst)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::CopyStringToBuffer_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ ICharArrayBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::CopyStringToBuffer_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [in] */ Int32 bufferSize,
    /* [in] */ ICharArrayBuffer* buffer,
    /* [out, callee] */ ArrayOf<Char8>** data)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::GetInt64_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Int64* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::GetDouble_native(
    /* [in] */ Int32 row,
    /* [in] */ Int32 col,
    /* [out] */ Double* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::Clear_native()
{
    return E_NOT_IMPLEMENTED;
}

ECode CursorWindow::Close_native()
{
    return E_NOT_IMPLEMENTED;
}

/** Get the Binder for the native side of the window */
ECode CursorWindow::Native_GetBinder(
    /* [out] */ IBinder** binder)
{
    return E_NOT_IMPLEMENTED;
}

/** Does the native side initialization for an empty window */
ECode CursorWindow::Native_Init(
    /* [in] */ Boolean localOnly)
{
    return E_NOT_IMPLEMENTED;
}

/** Does the native side initialization with an existing binder from another process */
ECode CursorWindow::Native_Init(
    /* [in] */ IBinder* nativeBinder)
{
    return E_NOT_IMPLEMENTED;
}