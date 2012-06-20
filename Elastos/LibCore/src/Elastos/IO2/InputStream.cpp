
#include "InputStream.h"

InputStream::InputStream()
{
}

InputStream::~InputStream()
{
}

ECode InputStream::Available(
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    *number = 0;
    return NOERROR;
}

ECode InputStream::Close()
{
    return NOERROR;
}

ECode InputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode InputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);
    *supported = FALSE;
    return NOERROR;
}

ECode InputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return ReadBufferEx(0, buffer->GetCapacity(), buffer, number);
}

ECode InputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    // Force null check for b first!
    if (offset > buffer->GetCapacity() || offset < 0) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetCapacity() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    for (Int32 i = 0; i < length; i++) {
        Int32 c;
        ECode ec = Read(&c);
        if (ec == E_IO_EXCEPTION) {
            if (i != 0) {
                *number = i;
                return NOERROR;
            }
            return ec;
        }
        if (c == -1) {
            *number = i == 0? -1 : i;
            return NOERROR;
        }
        (*buffer)[offset + i] = (Byte)c;
    }
    *number = length;
    return NOERROR;
}

ECode InputStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode InputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
//  return Streams.skipByReading(this, byteCount);
    return E_NOT_IMPLEMENTED;
}