#include "Blob2.h"

Blob2::Blob2()
    :mHandle(0)
    ,mSize(0)
{
}

ECode Blob2::GetInputStream(
    /** [out] **/IInputStream** stream)
{
    //return (InputStream) new BlobR(this);
/*
    AutoPtr<BlobR> res = new BlobR(this);
    *stream = (IInputStream*)res.Get();
    (*stream)->AddRef();*/
    return NOERROR;
}

ECode Blob2::GetOutputStream(
    /** [out] **/IOutputStream** stream)
{
    //return (OutputStream) new BlobW(this);
/*
    return (OutputStream) new BlobW(this);*/
    return NOERROR;
}

ECode Blob2::Close()
{/*
    doblobfinal(env, obj);*/
    return NOERROR;
}

ECode Blob2::Write(
    /** [in] **/Int32 off, 
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 len,
    /** [in] **/const ArrayOf<Byte>& b, 
    /** [out] **/Int32* result)
{/*
    hbl *bl = gethbl(env, obj);

    if (bl && bl->h && bl->blob) {
    jbyte *buf;
    jthrowable exc;
    int ret;

    if (len <= 0) {
        return 0;
    }
    buf = malloc(len);
    if (!buf) {
        throwoom(env, "out of buffer space for blob");
        return 0;
    }
    (*env)->GetByteArrayRegion(env, b, off, len, buf);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        free(buf);
        return 0;
    }
    ret = sqlite3_blob_write(bl->blob, buf, len, pos);
    free(buf);
    if (ret != SQLITE_OK) {
        throwioex(env, "blob write error");
        return 0;
    }
    return len;
    }
    throwex(env, "blob already closed");
    return 0;*/
    return NOERROR;
}

ECode Blob2::Read(
    /** [in] **/Int32 off, 
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 len,
    /** [out] **/ArrayOf<Byte>* b, 
    /** [out] **/Int32* result)
{/*
    hbl *bl = gethbl(env, obj);

    if (bl && bl->h && bl->blob) {
    jbyte *buf;
    jthrowable exc;
    int ret;

    if (len <= 0) {
        return 0;
    }
    buf = malloc(len);
    if (!buf) {
        throwoom(env, "out of buffer space for blob");
        return 0;
    }
    ret = sqlite3_blob_read(bl->blob, buf, len, pos);
    if (ret != SQLITE_OK) {
        free(buf);
        throwioex(env, "blob read error");
        return 0;
    }
    (*env)->SetByteArrayRegion(env, b, off, len, buf);
    free(buf);
    exc = (*env)->ExceptionOccurred(env);
    if (exc) {
        return 0;
    }
    return len;
    }
    throwex(env, "blob already closed");
    return 0;*/
    return NOERROR;
}

ECode Blob2::Finalize()
{/*
    doblobfinal(env, obj);*/
    return NOERROR;
}

ECode Blob2::Internal_init()
{
    return NOERROR;
}

BlobR::BlobR(
    /** [in] **/Blob2 blob)
    :mBlob(blob)
    ,mPos(0)
{
}

ECode BlobR::Available(
    /* [out] */ Int32* number)
{
    assert(number!=NULL);
    Int32 ret = mBlob.mSize - mPos;
    *number = (ret < 0) ? 0 : ret;
    return NOERROR;
}

ECode BlobR::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode BlobR::Reset()
{
    return NOERROR;
}

ECode BlobR::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported!=NULL);
    *supported = FALSE;
    return NOERROR;
}

ECode BlobR::Close()
{
    mBlob.Close();
    //mBlob = NULL;
    mPos = 0;
    return NOERROR;
}

ECode BlobR::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number!=NULL);
    Int64 ret = mPos + count;
    if (ret < 0) {
        ret = 0;
        mPos = 0;
    } else if (ret > mBlob.mSize) {
        ret = mBlob.mSize;
        mPos = mBlob.mSize;
    } else {
        mPos = (Int32) ret;
    }
    *number = ret;

    return NOERROR;
}

ECode BlobR::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);
    ArrayOf<Byte>* mBuf = ArrayOf<Byte>::Alloc(1);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    Int32 n; 
    mBlob.Read(0, mPos, mBuf->GetLength(), mBuf, &n);
    if (n > 0) {
        mPos += n;
        *value = (*mBuf)[0];
        ArrayOf<Byte>::Free(mBuf);
        return NOERROR;
    }
    *value = -1;
    ArrayOf<Byte>::Free(mBuf);
    return NOERROR;
}

ECode BlobR::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    Int32 n;
    assert(number != NULL);
    mBlob.Read(0, mPos, buffer->GetLength(), buffer, &n);
    if (n > 0) {
        mPos += n;
        *number = n;
        return NOERROR;
    }
    *number = -1;
    return NOERROR;
}

ECode BlobR::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    if (offset + length > buffer->GetLength()) {
        length = buffer->GetLength() - offset;
    }
    if (length < 0) {
        *number = -1;
        return NOERROR;
    }
    if (length == 0) {
        *number = 0;
        return NOERROR;
    }
    Int32 n;
    mBlob.Read(offset, mPos, length, buffer, &n);
    if (n > 0) {
        mPos += n;
        *number = n;
        return NOERROR;
    }
    *number = -1;
    return NOERROR;
}

BlobW::BlobW(Blob2 blob)
    :mBlob(blob)
    ,mPos(0)
{
}

ECode BlobW::Flush()
{
    return NOERROR;
}

ECode BlobW::Close()
{
    mBlob.Close();
    //blob = null;
    mPos = 0;
    return NOERROR;
}

ECode BlobW::Write(
    /* [in] */ Int32 oneByte)
{
    Int32 result;
    ArrayOf<Byte>* mBuf = ArrayOf<Byte>::Alloc(1);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    (*mBuf)[0] = (byte) oneByte;
    mBlob.Write(0, mPos, 1, (*mBuf), &result);
    mPos += result;
    ArrayOf<Byte>::Free(mBuf);
    return NOERROR;
}

ECode BlobW::WriteBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    if (buffer.GetLength() > 0) {
        Int32 result;
        mBlob.Write(0, mPos, buffer.GetLength(), buffer, &result);
        mPos += result;
    }
    return NOERROR;
}

ECode BlobW::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    if (offset + count > buffer.GetLength()) {
        count = buffer.GetLength() - offset;
    }
    if (count <= 0) {
        return NOERROR;
    }
    Int32 result;
    mBlob.Write(offset, mPos, count, buffer, &result);
    mPos += result;
    
    return NOERROR;
}