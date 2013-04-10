#include "Blob2.h"
#include <sqlite3.h>

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

/* internal handle for SQLite database */

typedef struct {
    void *sqlite;       /* SQLite handle */
    int ver;            /* version code */
//    jobject bh;         /* BusyHandler object */
//    jobject cb;         /* Callback object */
//    jobject ai;         /* Authorizer object */
//    jobject tr;         /* Trace object */
//    jobject ph;         /* ProgressHandler object */
//    JNIEnv *env;        /* Java environment for callbacks */
    int row1;           /* true while processing first row */
    // int haveutf;        /* true for SQLite UTF-8 support */
//    jstring enc;        /* encoding or 0 */
    struct hfunc *funcs;    /* SQLite user defined function handles */
#if HAVE_SQLITE_COMPILE
    struct hvm *vms;        /* Compiled SQLite VMs */
#endif
    sqlite3_stmt *stmt;     /* For callback() */
    struct hbl *blobs;        /* SQLite3 blob handles */
} handle;

/* internal handle for sqlite3_blob */

typedef struct hbl {
    struct hbl *next;       /* next blob handle */
    sqlite3_blob *blob;     /* SQLite3 blob */
    handle *h;          /* SQLite database handle */
} hbl;

ECode Blob2::Doblobfinal()
{
    hbl *bl = (hbl*)mHandle;

    if (bl) {
        if (bl->h) {
            handle *h = bl->h;
            hbl *blc, **blp;

            blp = &h->blobs;
            blc = *blp;
            while (blc) {
            if (blc == bl) {
                *blp = blc->next;
                break;
            }
            blp = &blc->next;
            blc = *blp;
            }
        }
        if (bl->blob) {
            sqlite3_blob_close(bl->blob);
        }
        bl->blob = 0;
        free(bl);
        mHandle = 0;
        mSize = 0;
    }
    return NOERROR;
}

ECode Blob2::Close()
{
    return Doblobfinal();
}

ECode Blob2::Write(
    /** [in] **/Int32 off, 
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 len,
    /** [in] **/const ArrayOf<Byte>& b, 
    /** [out] **/Int32* result)
{
    hbl *bl = (hbl*)mHandle;

    if (bl && bl->h && bl->blob) {
        Byte *buf;
        Int32 ret;

        if (len <= 0) {
            *result = 0;
            return NOERROR;
        }
        buf = (Byte *)malloc(len);
        if (!buf) {
            //throwoom(env, "out of buffer space for blob");
            *result = 0;
            return E_SQLITE_EXCEPTION;
        }
        //(*env)->GetByteArrayRegion(env, b, off, len, buf);
        Byte *bt = b.GetPayload();
        bt = bt+off;
        for(Int32 i=0;i<len;i++)
        {
            buf[i] = *bt;
        }

        ret = sqlite3_blob_write(bl->blob, buf, len, pos);
        free(buf);
        if (ret != SQLITE_OK) {
            //throwioex(env, "blob write error");
            *result = 0;
            return E_SQLITE_EXCEPTION;
        }
        *result = len;
    }
    //throwex(env, "blob already closed");
    *result = 0;
    return E_SQLITE_EXCEPTION;
}

ECode Blob2::Read(
    /** [in] **/Int32 off, 
    /** [in] **/Int32 pos, 
    /** [in] **/Int32 len,
    /** [out] **/ArrayOf<Byte>* b, 
    /** [out] **/Int32* result)
{
    hbl *bl = (hbl*)mHandle;

    if (bl && bl->h && bl->blob) {
        Byte *buf,*tmp;
        Int32 ret;

        if (len <= 0) {
            *result = 0;
            return NOERROR;
        }
        buf = (Byte *)malloc(len);
        if (!buf) {
            //throwoom(env, "out of buffer space for blob");
            *result = 0;
            return E_SQLITE_EXCEPTION;
        }
        ret = sqlite3_blob_read(bl->blob, buf, len, pos);
        if (ret != SQLITE_OK) {
            free(buf);
            //throwioex(env, "blob read error");
            *result = 0;
            return E_SQLITE_EXCEPTION;
        }
        //(*env)->SetByteArrayRegion(env, b, off, len, buf);
        tmp = buf + off;
        for(Int32 i=0;i<len;i++){
            (*b)[i] = *tmp;
        }

        free(buf);
        *result = len;
        return NOERROR;
    }
    //throwex(env, "blob already closed");
    *result = 0;
    return E_SQLITE_EXCEPTION;
}

ECode Blob2::Finalize()
{
    return Doblobfinal();
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