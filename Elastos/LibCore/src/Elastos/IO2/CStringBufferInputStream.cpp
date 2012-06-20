#include "cmdef.h"
#include "CStringBufferInputStream.h"

ECode CStringBufferInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    return StringBufferInputStream::Available(number);
}

ECode CStringBufferInputStream::Close()
{
    return StringBufferInputStream::Close();
}

ECode CStringBufferInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return StringBufferInputStream::Mark(readLimit);
}

ECode CStringBufferInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return StringBufferInputStream::IsMarkSupported(supported);
}

ECode CStringBufferInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return StringBufferInputStream::Read(value);
}

ECode CStringBufferInputStream::ReadBuffer(
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return StringBufferInputStream::ReadBuffer(buffer, number);
}

ECode CStringBufferInputStream::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ BufferOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return StringBufferInputStream::ReadBufferEx(offset, length, buffer, number);
}

ECode CStringBufferInputStream::Reset()
{
    return StringBufferInputStream::Reset();
}

ECode CStringBufferInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return StringBufferInputStream::Skip(count, number);
}

ECode CStringBufferInputStream::constructor(
    /* [in] */ String str)
{
    return StringBufferInputStream::Init(str);
}

Mutex* CStringBufferInputStream::GetSelfLock()
{
    return &_m_syncLock;
}
