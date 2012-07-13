#include "cmdef.h"
#include "CPushbackReader.h"

ECode CPushbackReader::Close()
{
    return PushbackReader::Close();
}

ECode CPushbackReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return PushbackReader::Mark(readLimit);
}

ECode CPushbackReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return PushbackReader::IsMarkSupported(supported);
}

ECode CPushbackReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return PushbackReader::Read(value);
}

ECode CPushbackReader::ReadBuffer(
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(number);

    return PushbackReader::ReadBuffer(buffer, number);
}

ECode CPushbackReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(number);

    return PushbackReader::ReadBufferEx(offset, count, buffer, number);
}

ECode CPushbackReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);

    return PushbackReader::IsReady(ready);
}

ECode CPushbackReader::Reset()
{
    return PushbackReader::Reset();
}

ECode CPushbackReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return PushbackReader::Skip(count, number);
}

ECode CPushbackReader::UnRead(
    /* [in] */ Int32 oneChar)
{
    return PushbackReader::UnRead(oneChar);
}

ECode CPushbackReader::UnReadBuffer(
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    return PushbackReader::UnReadBuffer(buffer);
}

ECode CPushbackReader::UnReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    return PushbackReader::UnReadBufferEx(offset, length, buffer);
}

ECode CPushbackReader::constructor(
    /* [in] */ IReader* in)
{
    return PushbackReader::Init(in);
}

ECode CPushbackReader::constructor(
    /* [in] */ IReader* in,
    /* [in] */ Int32 size)
{
    return PushbackReader::Init(in, size);
}

Mutex* CPushbackReader::GetSelfLock()
{
    return &_m_syncLock;
}