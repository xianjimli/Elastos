#include "cmdef.h"
#include "ByteArrayOutputStream.h"

ByteArrayOutputStream::ByteArrayOutputStream()
    : mBuf(NULL)
    , mCount(0)
{
}

ByteArrayOutputStream::~ByteArrayOutputStream()
{
    ArrayOf<Byte>::Free(mBuf);
}

ECode ByteArrayOutputStream::Init(
    /* [in] */ Int32 size)
{
    if (size < 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = ArrayOf<Byte>::Alloc(size);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    return NOERROR;
}

ECode ByteArrayOutputStream::Close()
{
    /**
     * Although the spec claims "A closed stream cannot perform output
     * operations and cannot be reopened.", this implementation must do
     * nothing.
     */
    return OutputStream::Close();
}

ECode ByteArrayOutputStream::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode ByteArrayOutputStream::ToStringEx(
    /* [in] */ const String& enc,
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode ByteArrayOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    if (mCount == mBuf->GetLength()){
        Expand(1);
    }
    (*mBuf)[mCount++] = (Byte)oneByte;

    return NOERROR;
}

ECode ByteArrayOutputStream::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, made implicit null check explicit,
    // used (offset | len) < 0 instead of (offset < 0) || (len < 0)
    // to safe one operation
    if ((offset | count) < 0 || count > buffer.GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    if (count == 0) {
        return NOERROR;
    }
    /* Expand if necessary */
    Expand(count);
    for (Int32 i = offset,j = mCount; j < mCount+count; i++,j++) {
        (*mBuf)[j] = buffer[i];
    }
    mCount += count;
    return NOERROR;
}

ECode ByteArrayOutputStream::Expand(
    /* [in] */ Int32 i)
{
    /* Can the buffer handle @i more bytes, if not expand it */
    if (mCount + i <= mBuf->GetLength()){
        return NOERROR;
    }
    ArrayOf<Byte>* newbuf = ArrayOf<Byte>::Alloc((mCount + i) * 2);
    if (newbuf == NULL) return E_OUT_OF_MEMORY_ERROR;
    for (Int32 i = 0; i < mCount; i++) {
        (*newbuf)[i] = (*mBuf)[i];
    }
    ArrayOf<Byte>::Free(mBuf);
    mBuf = newbuf;
    return NOERROR;
}

ECode ByteArrayOutputStream::Reset()
{
    mCount = 0;
    return NOERROR;
}

ECode ByteArrayOutputStream::GetSize(
    /* [out] */ Int32* size)
{
    assert(size != NULL);
    *size = mCount;
    return NOERROR;
}

ECode ByteArrayOutputStream::ToByteArray(
    /* [out, callee] */ ArrayOf<Byte>** byteArray)
{
    assert(byteArray != NULL);
    ArrayOf<Byte>* newArray = ArrayOf<Byte>::Alloc(mCount);
    if (newArray == NULL) return E_OUT_OF_MEMORY_ERROR;
    for (Int32 i = 0; i < mCount; i++) {
        (*newArray)[i] = (*mBuf)[i];
    }
    *byteArray = newArray;
    return NOERROR;
}

ECode ByteArrayOutputStream::WriteTo(
    /* [in] */ IOutputStream* out)
{
    return out->WriteBufferEx(0, mCount, *mBuf);
}
