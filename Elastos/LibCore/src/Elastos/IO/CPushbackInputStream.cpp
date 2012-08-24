#include "cmdef.h"
#include "CPushbackInputStream.h"

ECode CPushbackInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return PushbackInputStream::Available(number);
}

ECode CPushbackInputStream::Close()
{
    return PushbackInputStream::Close();
}

ECode CPushbackInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return PushbackInputStream::Mark(readLimit);
}

ECode CPushbackInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return PushbackInputStream::IsMarkSupported(supported);
}

ECode CPushbackInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return PushbackInputStream::Read(value);
}

ECode CPushbackInputStream::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return PushbackInputStream::ReadBuffer(buffer, number);
}

ECode CPushbackInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return PushbackInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CPushbackInputStream::Reset()
{
    return PushbackInputStream::Reset();
}

ECode CPushbackInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return PushbackInputStream::Skip(count, number);
}

ECode CPushbackInputStream::UnRead(
    /* [in] */ Int32 oneByte)
{
    return PushbackInputStream::UnRead(oneByte);
}

ECode CPushbackInputStream::UnReadBuffer(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return PushbackInputStream::UnReadBuffer(buffer);
}

ECode CPushbackInputStream::UnReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return PushbackInputStream::UnReadBufferEx(offset, length, buffer);
}

ECode CPushbackInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return PushbackInputStream::Init(is);
}

ECode CPushbackInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 size)
{
    return PushbackInputStream::Init(is, size);
}

Mutex* CPushbackInputStream::GetSelfLock()
{
    return &_m_syncLock;
}
