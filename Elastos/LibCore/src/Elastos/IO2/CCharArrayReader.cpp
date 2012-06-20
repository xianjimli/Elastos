#include "cmdef.h"
#include "CCharArrayReader.h"

ECode CCharArrayReader::Close()
{
    return CharArrayReader::Close();
}

ECode CCharArrayReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return CharArrayReader::Mark(readLimit);
}

ECode CCharArrayReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return CharArrayReader::IsMarkSupported(supported);
}

ECode CCharArrayReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return CharArrayReader::Read(value);
}

ECode CCharArrayReader::ReadBuffer(
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return CharArrayReader::ReadBuffer(buffer, number);
}

ECode CCharArrayReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return CharArrayReader::ReadBufferEx(offset, length, buffer, number);
}

ECode CCharArrayReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);

    return CharArrayReader::IsReady(ready);
}

ECode CCharArrayReader::Reset()
{
    return CharArrayReader::Reset();
}

ECode CCharArrayReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return CharArrayReader::Skip(count, number);
}

ECode CCharArrayReader::constructor(
    /* [in] */ const BufferOf<Byte>& buf)
{
    return CharArrayReader::Init(buf);
}

ECode CCharArrayReader::constructor(
    /* [in] */ const BufferOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return CharArrayReader::Init(buf, offset, length);
}

Mutex* CCharArrayReader::GetSelfLock()
{
    return &_m_syncLock;
}
