
#include "cmdef.h"
#include "BufferedOutputStream.h"

BufferedOutputStream::BufferedOutputStream()
    : mBuf(NULL)
    , mcount(0)
{
}

BufferedOutputStream::~BufferedOutputStream()
{
}

ECode BufferedOutputStream::Init(
    /* [in] */ IOutputStream* outs,
    /* [in] */ Int32 size)
{
    FAIL_RETURN(FilterOutputStream::Init(outs));
    if (size <= 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = BufferOf<Byte>::Alloc(size);
    if (mBuf == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    return NOERROR;
}

ECode BufferedOutputStream::Close()
{
    if (mBuf != NULL) {
        FilterOutputStream::Close();
        BufferOf<Byte>::Free(mBuf);
        mBuf = NULL;
    }
    return NOERROR;
}

ECode BufferedOutputStream::Flush()
{
    FAIL_RETURN(CheckNotClosed());
    FAIL_RETURN(FlushInternal());
    return mOut->Flush();
}

ECode BufferedOutputStream::Write(
        /* [in] */ Int32 oneByte)
{
    FAIL_RETURN(CheckNotClosed());

    if (mcount == mBuf->GetCapacity()) {
        mOut->WriteBufferEx(0, mcount, *mBuf);
        mcount = 0;
    }
    (*mBuf)[mcount++] = (Byte)oneByte;
    return NOERROR;
}

ECode BufferedOutputStream::WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const BufferOf<Byte> & buffer)
{
    FAIL_RETURN(CheckNotClosed());

    BufferOf<Byte>* localBuf = mBuf;
    if (count >= localBuf->GetCapacity()) {
        FlushInternal();
        return mOut->WriteBufferEx(offset, count, buffer);
    }

    if (offset < 0 || offset > buffer.GetCapacity() - count) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (count < 0) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    // flush the internal buffer first if we have not enough space left
    if (count >= (localBuf->GetCapacity() - mcount)) {
        FlushInternal();
    }

    // the length is always less than (internalBuffer.length - count) here so arraycopy is safe
    for (Int32 i = offset,j = mcount; j < count; i++,j++) {
        (*localBuf)[j] = buffer[i];
    }
    mcount += count;

    return NOERROR;
}

ECode BufferedOutputStream::FlushInternal()
{
    if (mcount > 0) {
        FAIL_RETURN(mOut->WriteBufferEx(0, mcount, *mBuf));
        mcount = 0;
    }
    return NOERROR;
}

ECode BufferedOutputStream::CheckNotClosed()
{
    if(mBuf == NULL)
        return E_IO_EXCEPTION;
    return NOERROR;
}